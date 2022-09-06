// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// Class: dviRenderer
//
// Class for rendering TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// SPDX-FileCopyrightText: 2001-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include "debug_dvi.h"
#include "dviFile.h"
#include "dviRenderer.h"
#include "dvisourcesplitter.h"
#include "hyperlink.h"
#include "psgs.h"
//#include "renderedDviPagePixmap.h"

#include <KConfig>
#include <KLocalizedString>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTime>
#include <QVBoxLayout>
#include <math.h>

#include <QApplication>
#include <QCheckBox>
#include <QEventLoop>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <QRegularExpression>

//------ now comes the dviRenderer class implementation ----------

dviRenderer::dviRenderer(bool useFontHinting)
    : dviFile(nullptr)
    , font_pool(useFontHinting)
    , resolutionInDPI(0)
    , embedPS_progress(nullptr)
    , embedPS_numOfProgressedFiles(0)
    , shrinkfactor(3)
    , source_href(nullptr)
    , HTML_href(nullptr)
    , editorCommand(QLatin1String(""))
    , PostScriptOutPutString(nullptr)
    , PS_interface(new ghostscript_interface)
    , _postscript(true)
    , line_boundary_encountered(false)
    , word_boundary_encountered(false)
    , current_page(0)
    , penWidth_in_mInch(0)
    , number_of_elements_in_path(0)
    , currentlyDrawnPage(nullptr)
    , m_eventLoop(nullptr)
    , foreGroundPainter(nullptr)
    , fontpoolLocateFontsDone(false)
{
#ifdef DEBUG_DVIRENDERER
    // qCDebug(OkularDviDebug) << "dviRenderer( parent=" << par << " )";
#endif

    connect(&font_pool, &fontPool::error, this, &dviRenderer::error);
    connect(&font_pool, &fontPool::warning, this, &dviRenderer::warning);
    connect(PS_interface, &ghostscript_interface::error, this, &dviRenderer::error);
}

dviRenderer::~dviRenderer()
{
#ifdef DEBUG_DVIRENDERER
    qCDebug(OkularDviDebug) << "~dviRenderer";
#endif

    QMutexLocker locker(&mutex);

    delete PS_interface;
    delete dviFile;
}

//------ this function calls the dvi interpreter ----------

void dviRenderer::drawPage(RenderedDocumentPagePixmap *page)
{
#ifdef DEBUG_DVIRENDERER
    // qCDebug(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called, page number " << page->pageNumber;
#endif

    // Paranoid safety checks
    if (page == nullptr) {
        qCCritical(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called with argument == 0";
        return;
    }
    // Paranoid safety checks
    if (!page->pageNumber.isValid()) {
        qCCritical(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called for a documentPage with page number 0";
        return;
    }

    QMutexLocker locker(&mutex);

    if (dviFile == nullptr) {
        qCCritical(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called, but no dviFile class allocated.";
        page->clear();
        return;
    }
    if (static_cast<quint16>(page->pageNumber) > dviFile->total_pages) {
        qCCritical(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called for a documentPage with page number " << static_cast<quint16>(page->pageNumber) << " but the current dviFile has only " << dviFile->total_pages
                                   << " pages.";
        return;
    }
    if (dviFile->dvi_Data() == nullptr) {
        qCCritical(OkularDviDebug) << "dviRenderer::drawPage(documentPage *) called, but no dviFile is loaded yet.";
        page->clear();
        return;
    }

    /* locateFonts() is here just once (if it has not been executed
       not been executed yet), so that it is possible to easily intercept
       the cancel signal (because for example the user tries to open
       another document); it would not have been possible (or more
       complicated) in case it was still in the document loading section.
     */
    if (!fontpoolLocateFontsDone) {
        font_pool.locateFonts();
        fontpoolLocateFontsDone = true;
    }

    double resolution = page->resolution;

    if (resolution != resolutionInDPI) {
        setResolution(resolution);
    }

    currentlyDrawnPage = page;
    shrinkfactor = 1200 / resolutionInDPI;
    current_page = static_cast<quint16>(page->pageNumber) - 1;

    // Reset colors
    colorStack.clear();
    globalColor = Qt::black;

    int pageWidth = page->width;
    int pageHeight = page->height;

    QImage img(pageWidth, pageHeight, QImage::Format_RGB32);
    foreGroundPainter = new QPainter(&img);
    if (foreGroundPainter != nullptr) {
        errorMsg.clear();
        draw_page();
        delete foreGroundPainter;
        foreGroundPainter = nullptr;
    } else {
        qCDebug(OkularDviDebug) << "painter creation failed.";
    }
    page->img = img;
    // page->setImage(img);

    // Postprocess hyperlinks
    // Without that, based on the way TeX draws certain characters like german "Umlaute",
    // some hyperlinks would be broken into two overlapping parts, in the middle of a word.
    QVector<Hyperlink>::iterator i = page->hyperLinkList.begin();
    QVector<Hyperlink>::iterator j;
    while (i != page->hyperLinkList.end()) {
        // Iterator j always points to the element after i.
        j = i;
        j++;

        if (j == page->hyperLinkList.end()) {
            break;
        }

        Hyperlink &hi = *i;
        Hyperlink &hj = *j;

        bool merged = false;

        // Merge all hyperlinks that point to the same target, and have the same baseline.
        while (hi.linkText == hj.linkText && hi.baseline == hj.baseline) {
            merged = true;
            hi.box = hi.box.united(hj.box);

            j++;
            if (j == page->hyperLinkList.end()) {
                break;
            }

            hj = *j;
        }

        if (merged) {
            i = page->hyperLinkList.erase(++i, j);
        } else {
            i++;
        }
    }

    if (errorMsg.isEmpty() != true) {
        Q_EMIT error(i18n("File corruption. %1", errorMsg), -1);
        errorMsg.clear();
        currentlyDrawnPage = nullptr;
        return;
    }
    currentlyDrawnPage = nullptr;
}

void dviRenderer::getText(RenderedDocumentPagePixmap *page)
{
    bool postscriptBackup = _postscript;
    // Disable postscript-specials temporarily to speed up text extraction.
    _postscript = false;

    drawPage(page);

    _postscript = postscriptBackup;
}

/*
void dviRenderer::showThatSourceInformationIsPresent()
{
  // In principle, we should use a KMessagebox here, but we want to
  // add a button "Explain in more detail..." which opens the
  // Helpcenter. Thus, we practically re-implement the KMessagebox
  // here. Most of the code is stolen from there.

  // Check if the 'Don't show again' feature was used
  KConfig *config = KSharedConfig::openConfig();
  KConfigGroup saver(config, "Notification Messages");
  bool showMsg = config->readEntry( "KDVI-info_on_source_specials", true);

  if (showMsg) {
    KDialogBase dialog(i18n("KDVI: Information"), KDialogBase::Yes, KDialogBase::Yes, KDialogBase::Yes,
                       parentWidget, "information", true, true, KStandardGuiItem::ok());

    QWidget *topcontents = new QWidget (&dialog);
    QVBoxLayout *topcontentsVBoxLayout = new QVBoxLayout(topcontents);
    topcontentsVBoxLayout->setContentsMargins(0, 0, 0, 0);
    topcontentsVBoxLayout->setSpacing(KDialog::spacingHint()*2);
    topcontentsVBoxLayout->setContentsMargins(KDialog::marginHint()*2, KDialog::marginHint()*2, KDialog::marginHint()*2, KDialog::marginHint()*2);

    QWidget *contents = new QWidget(topcontents);
    topcontentsVBoxLayout->addWidget(contents);
    QHBoxLayout * lay = new QHBoxLayout(contents);
    lay->setSpacing(KDialog::spacingHint()*2);

    lay->addStretch(1);
    QLabel *label1 = new QLabel( contents);
    label1->setPixmap(QMessageBox::standardIcon(QMessageBox::Information));
    lay->addWidget(label1);
    QLabel *label2 = new QLabel( i18n("<qt>This DVI file contains source file information. You may click into the text with the "
                                      "middle mouse button, and an editor will open the TeX-source file immediately.</qt>"),
                                 contents);
    label2->setMinimumSize(label2->sizeHint());
    lay->addWidget(label2);
    lay->addStretch(1);
    QSize extraSize = QSize(50,30);
    QCheckBox *checkbox = new QCheckBox(i18n("Do not show this message again"), topcontents);
    topcontentsVBoxLayout->addWidget(checkbox);
    extraSize = QSize(50,0);
    dialog.setHelpLinkText(i18n("Explain in more detail..."));
    dialog.setHelp("inverse-search", "kdvi");
    dialog.enableLinkedHelp(true);
    dialog.setMainWidget(topcontents);
    dialog.enableButtonSeparator(false);
    dialog.incInitialSize( extraSize );
    dialog.exec();

    showMsg = !checkbox->isChecked();
    if (!showMsg) {
      KConfigGroup saver(config, "Notification Messages");
      config->writeEntry( "KDVI-info_on_source_specials", showMsg);
    }
    config->sync();
  }
}
*/

void dviRenderer::embedPostScript()
{
#ifdef DEBUG_DVIRENDERER
    qCDebug(OkularDviDebug) << "dviRenderer::embedPostScript()";
#endif

    if (!dviFile) {
        return;
    }

    /*  embedPS_progress = new QProgressDialog(parentWidget);
    embedPS_progress->setWindowTitle(i18n("Embedding PostScript Files"));
    embedPS_progress->setLabelText(QString());
    */
    if (!embedPS_progress) {
        return;
    }
    embedPS_progress->setCancelButton(nullptr);
    embedPS_progress->setCancelButton(nullptr);
    embedPS_progress->setMinimumDuration(400);
    embedPS_progress->setMaximum(dviFile->numberOfExternalPSFiles);
    embedPS_progress->setValue(0);
    embedPS_numOfProgressedFiles = 0;

    quint16 currPageSav = current_page;
    errorMsg.clear();
    for (current_page = 0; current_page < dviFile->total_pages; current_page++) {
        if (current_page < dviFile->total_pages) {
            command_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page)];
            end_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page + 1)];
        } else {
            command_pointer = end_pointer = nullptr;
        }

        memset((char *)&currinf.data, 0, sizeof(currinf.data));
        currinf.fonttable = &(dviFile->tn_table);
        currinf._virtual = nullptr;
        prescan(&dviRenderer::prescan_embedPS);
    }

    delete embedPS_progress;
    embedPS_progress = nullptr;

    if (!errorMsg.isEmpty()) {
        Q_EMIT warning(i18n("Not all PostScript files could be embedded into your document. %1", errorMsg), -1);
        errorMsg.clear();
    } else {
        Q_EMIT notice(i18n("All external PostScript files were embedded into your document."), -1);
    }

    // Prescan phase starts here
#ifdef PERFORMANCE_MEASUREMENT
    // qCDebug(OkularDviDebug) << "Time elapsed till prescan phase starts " << performanceTimer.elapsed() << "ms";
    // QTime preScanTimer;
    // preScanTimer.start();
#endif
    dviFile->numberOfExternalPSFiles = 0;
    prebookmarks.clear();
    for (current_page = 0; current_page < dviFile->total_pages; current_page++) {
        PostScriptOutPutString = new QString();

        if (current_page < dviFile->total_pages) {
            command_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page)];
            end_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page + 1)];
        } else {
            command_pointer = end_pointer = nullptr;
        }

        memset((char *)&currinf.data, 0, sizeof(currinf.data));
        currinf.fonttable = &(dviFile->tn_table);
        currinf._virtual = nullptr;

        prescan(&dviRenderer::prescan_parseSpecials);

        if (!PostScriptOutPutString->isEmpty()) {
            PS_interface->setPostScript(current_page, *PostScriptOutPutString);
        }
        delete PostScriptOutPutString;
    }
    PostScriptOutPutString = nullptr;

#ifdef PERFORMANCE_MEASUREMENT
    // qCDebug(OkularDviDebug) << "Time required for prescan phase: " << preScanTimer.restart() << "ms";
#endif
    current_page = currPageSav;
    _isModified = true;
}

bool dviRenderer::isValidFile(const QString &filename) const
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        return false;
    }

    unsigned char test[4];
    if (f.read((char *)test, 2) < 2 || test[0] != 247 || test[1] != 2) {
        return false;
    }

    int n = f.size();
    if (n < 134) { // Too short for a dvi file
        return false;
    }
    f.seek(n - 4);

    unsigned char trailer[4] = {0xdf, 0xdf, 0xdf, 0xdf};

    if (f.read((char *)test, 4) < 4 || strncmp((char *)test, (char *)trailer, 4) != 0) {
        return false;
    }
    // We suppose now that the dvi file is complete and OK
    return true;
}

bool dviRenderer::setFile(const QString &fname, const QUrl &base)
{
#ifdef DEBUG_DVIRENDERER
    qCDebug(OkularDviDebug) << "dviRenderer::setFile( fname='" << fname << "' )"; //, ref='" << ref << "', sourceMarker=" << sourceMarker << " )";
#endif

    // QMutexLocker lock(&mutex);

    QFileInfo fi(fname);
    QString filename = fi.absoluteFilePath();

    // If fname is the empty string, then this means: "close". Delete
    // the dvifile and the pixmap.
    if (fname.isEmpty()) {
        // Delete DVI file
        delete dviFile;
        dviFile = nullptr;
        return true;
    }

    // Make sure the file actually exists.
    if (!fi.exists() || fi.isDir()) {
        Q_EMIT error(i18n("The specified file '%1' does not exist.", filename), -1);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    dvifile *dviFile_new = new dvifile(filename, &font_pool);

    if ((dviFile == nullptr) || (dviFile->filename != filename)) {
        dviFile_new->sourceSpecialMarker = true;
    } else {
        dviFile_new->sourceSpecialMarker = false;
    }

    if ((dviFile_new->dvi_Data() == nullptr) || (dviFile_new->errorMsg.isEmpty() != true)) {
        QApplication::restoreOverrideCursor();
        if (dviFile_new->errorMsg.isEmpty() != true) {
            Q_EMIT error(i18n("File corruption. %1", dviFile_new->errorMsg), -1);
        }
        delete dviFile_new;
        return false;
    }

    delete dviFile;
    dviFile = dviFile_new;
    numPages = dviFile->total_pages;
    _isModified = false;
    baseURL = base;

    font_pool.setExtraSearchPath(fi.absolutePath());
    font_pool.setCMperDVIunit(dviFile->getCmPerDVIunit());

    // Extract PostScript from the DVI file, and store the PostScript
    // specials in PostScriptDirectory, and the headers in the
    // PostScriptHeaderString.
    PS_interface->clear();

    // If the DVI file comes from a remote URL (e.g. downloaded from a
    // web server), we limit the PostScript files that can be accessed
    // by this file to the download directory, in order to limit the
    // possibilities of a denial of service attack.
    QString includePath;
    if (!baseURL.isLocalFile()) {
        includePath = filename;
        includePath.truncate(includePath.lastIndexOf(QLatin1Char('/')));
    }
    PS_interface->setIncludePath(includePath);

    // We will also generate a list of hyperlink-anchors and source-file
    // anchors in the document. So declare the existing lists empty.
    // anchorList.clear();
    sourceHyperLinkAnchors.clear();
    // bookmarks.clear();
    prebookmarks.clear();

    if (dviFile->page_offset.isEmpty() == true) {
        return false;
    }

    // We should pre-scan the document now (to extract embedded,
    // PostScript, Hyperlinks, ets).

    // PRESCAN STARTS HERE
#ifdef PERFORMANCE_MEASUREMENT
    // qCDebug(OkularDviDebug) << "Time elapsed till prescan phase starts " << performanceTimer.elapsed() << "ms";
    // QTime preScanTimer;
    // preScanTimer.start();
#endif
    dviFile->numberOfExternalPSFiles = 0;
    quint16 currPageSav = current_page;
    prebookmarks.clear();

    for (current_page = 0; current_page < dviFile->total_pages; current_page++) {
        PostScriptOutPutString = new QString();

        if (current_page < dviFile->total_pages) {
            command_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page)];
            end_pointer = dviFile->dvi_Data() + dviFile->page_offset[int(current_page + 1)];
        } else {
            command_pointer = end_pointer = nullptr;
        }

        memset((char *)&currinf.data, 0, sizeof(currinf.data));
        currinf.fonttable = &(dviFile->tn_table);
        currinf._virtual = nullptr;
        prescan(&dviRenderer::prescan_parseSpecials);

        if (!PostScriptOutPutString->isEmpty()) {
            PS_interface->setPostScript(current_page, *PostScriptOutPutString);
        }
        delete PostScriptOutPutString;
    }
    PostScriptOutPutString = nullptr;

#ifdef PERFORMANCE_MEASUREMENT
    // qCDebug(OkularDviDebug) << "Time required for prescan phase: " << preScanTimer.restart() << "ms";
#endif
    current_page = currPageSav;
    // PRESCAN ENDS HERE

    pageSizes.resize(0);
    if (dviFile->suggestedPageSize != nullptr) {
        // Fill the vector pageSizes with total_pages identical entries
        pageSizes.fill(*(dviFile->suggestedPageSize), dviFile->total_pages);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

Anchor dviRenderer::parseReference(const QString &reference)
{
    QMutexLocker locker(&mutex);

#ifdef DEBUG_DVIRENDERER
    qCCritical(OkularDviDebug) << "dviRenderer::parseReference( " << reference << " ) called";
#endif

    if (dviFile == nullptr) {
        return Anchor();
    }

    // case 1: The reference is a number, which we'll interpret as a
    // page number.
    bool ok;
    int page = reference.toInt(&ok);
    if (ok == true) {
        if (page < 0) {
            page = 0;
        }
        if (page > dviFile->total_pages) {
            page = dviFile->total_pages;
        }

        return Anchor(page, Length());
    }

    // case 2: The reference is of form "src:1111Filename", where "1111"
    // points to line number 1111 in the file "Filename". KDVI then
    // looks for source specials of the form "src:xxxxFilename", and
    // tries to find the special with the biggest xxxx
    if (reference.indexOf(QStringLiteral("src:"), 0, Qt::CaseInsensitive) == 0) {
        // Extract the file name and the numeral part from the reference string
        DVI_SourceFileSplitter splitter(reference, dviFile->filename);
        quint32 refLineNumber = splitter.line();
        QString refFileName = splitter.filePath();

        if (sourceHyperLinkAnchors.isEmpty()) {
            Q_EMIT warning(i18n("You have asked Okular to locate the place in the DVI file which corresponds to "
                                "line %1 in the TeX-file %2. It seems, however, that the DVI file "
                                "does not contain the necessary source file information. ",
                                refLineNumber,
                                refFileName),
                           -1);
            return Anchor();
        }

        // Go through the list of source file anchors, and find the anchor
        // whose line number is the biggest among those that are smaller
        // than the refLineNumber. That way, the position in the DVI file
        // which is highlighted is always a little further up than the
        // position in the editor, e.g. if the DVI file contains
        // positional information at the beginning of every paragraph,
        // KDVI jumps to the beginning of the paragraph that the cursor is
        // in, and never to the next paragraph. If source file anchors for
        // the refFileName can be found, but none of their line numbers is
        // smaller than the refLineNumber, the reason is most likely, that
        // the cursor in the editor stands somewhere in the preamble of
        // the LaTeX file. In that case, we jump to the beginning of the
        // document.
        bool anchorForRefFileFound = false; // Flag that is set if source file anchors for the refFileName could be found at all

        QVector<DVI_SourceFileAnchor>::iterator bestMatch = sourceHyperLinkAnchors.end();
        QVector<DVI_SourceFileAnchor>::iterator it;
        for (it = sourceHyperLinkAnchors.begin(); it != sourceHyperLinkAnchors.end(); ++it) {
            if (refFileName.trimmed() == it->fileName.trimmed() || refFileName.trimmed() == it->fileName.trimmed() + QStringLiteral(".tex")) {
                anchorForRefFileFound = true;

                if ((it->line <= refLineNumber) && ((bestMatch == sourceHyperLinkAnchors.end()) || (it->line > bestMatch->line))) {
                    bestMatch = it;
                }
            }
        }

        if (bestMatch != sourceHyperLinkAnchors.end()) {
            return Anchor(bestMatch->page, bestMatch->distance_from_top);
        } else if (anchorForRefFileFound == false) {
            Q_EMIT warning(i18n("Okular was not able to locate the place in the DVI file which corresponds to "
                                "line %1 in the TeX-file %2.",
                                refLineNumber,
                                refFileName),
                           -1);
        } else {
            return Anchor();
        }
        return Anchor();
    }
    return Anchor();
}

void dviRenderer::setResolution(double resolution_in_DPI)
{
    // Ignore minute changes. The difference to the current value would
    // hardly be visible anyway. That saves a lot of re-painting,
    // e.g. when the user resizes the window, and a flickery mouse
    // changes the window size by 1 pixel all the time.
    if (fabs(resolutionInDPI - resolution_in_DPI) < 1) {
        return;
    }

    resolutionInDPI = resolution_in_DPI;

    // Pass the information on to the font pool.
    font_pool.setDisplayResolution(resolutionInDPI);
    shrinkfactor = 1200 / resolutionInDPI;
    return;
}

void dviRenderer::handleSRCLink(const QString &linkText, const QPoint point, DocumentWidget *widget)
{
    Q_UNUSED(linkText);
    Q_UNUSED(point);
    Q_UNUSED(widget);
}

QString dviRenderer::PDFencodingToQString(const QString &_pdfstring)
{
    // This method locates special PDF characters in a string and
    // replaces them by UTF8. See Section 3.2.3 of the PDF reference
    // guide for information.
    QString pdfstring = _pdfstring;
    pdfstring = pdfstring.replace(QLatin1String("\\n"), QLatin1String("\n"));
    pdfstring = pdfstring.replace(QLatin1String("\\r"), QLatin1String("\n"));
    pdfstring = pdfstring.replace(QLatin1String("\\t"), QLatin1String("\t"));
    pdfstring = pdfstring.replace(QLatin1String("\\f"), QLatin1String("\f"));
    pdfstring = pdfstring.replace(QLatin1String("\\("), QLatin1String("("));
    pdfstring = pdfstring.replace(QLatin1String("\\)"), QLatin1String(")"));
    pdfstring = pdfstring.replace(QLatin1String("\\\\"), QLatin1String("\\"));

    // Now replace octal character codes with the characters they encode
    QRegularExpression regex(QStringLiteral("(\\\\)(\\d\\d\\d)")); // matches "\xyz" where x,y,z are numbers
    QRegularExpressionMatch match;
    while ((match = regex.match(pdfstring)).hasMatch()) {
        pdfstring = pdfstring.replace(match.capturedStart(0), 4, QChar(match.captured(2).toInt(nullptr, 8)));
    }
    regex.setPattern(QStringLiteral("(\\\\)(\\d\\d)")); // matches "\xy" where x,y are numbers
    while ((match = regex.match(pdfstring)).hasMatch()) {
        pdfstring = pdfstring.replace(match.capturedStart(0), 3, QChar(match.captured(2).toInt(nullptr, 8)));
    }
    regex.setPattern(QStringLiteral("(\\\\)(\\d)")); // matches "\x" where x is a number
    while ((match = regex.match(pdfstring)).hasMatch()) {
        pdfstring = pdfstring.replace(match.capturedStart(0), 2, QChar(match.captured(2).toInt(nullptr, 8)));
    }

    return pdfstring;
}

void dviRenderer::exportPDF()
{
    /*
      QExplicitlySharedDataPointer<DVIExport> exporter(new DVIExportToPDF(*this, parentWidget));
      if (exporter->started())
        all_exports_[exporter.data()] = exporter;
    */
}

void dviRenderer::exportPS(const QString &fname, const QStringList &options, QPrinter *printer, QPrinter::Orientation orientation)
{
    QExplicitlySharedDataPointer<DVIExport> exporter(new DVIExportToPS(*this, fname, options, printer, font_pool.getUseFontHints(), orientation));
    if (exporter->started()) {
        all_exports_[exporter.data()] = exporter;
    }
}

/*
void dviRenderer::editor_finished(const DVISourceEditor*)
{
  editor_.attach(0);
}
*/

void dviRenderer::export_finished(const DVIExport *key)
{
    typedef QMap<const DVIExport *, QExplicitlySharedDataPointer<DVIExport>> ExportMap;
    ExportMap::iterator it = all_exports_.find(key);
    if (it != all_exports_.end()) {
        all_exports_.remove(key);
    }
}

void dviRenderer::setEventLoop(QEventLoop *el)
{
    if (el == nullptr) {
        delete m_eventLoop;
        m_eventLoop = nullptr;
    } else {
        m_eventLoop = el;
    }
}
