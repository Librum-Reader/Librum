// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// dviRenderer_prescan.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003-2004 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include "TeXFont.h"
#include "debug_dvi.h"
#include "dvi.h"
#include "dviFile.h"
#include "dviRenderer.h"
#include "prebookmark.h"
#include "psgs.h"

#include <KLocalizedString>
#include <KProcess>
#include <QMimeDatabase>
#include <QMimeType>

#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QPaintDevice>
#include <QProgressBar>
#include <QStandardPaths>
#include <QTextStream>

extern QPainter foreGroundPaint;
extern void parse_special_argument(const QString &strg, const char *argument_name, int *variable);

//#define DEBUG_PRESCAN

void dviRenderer::prescan_embedPS(char *cp, quint8 *beginningOfSpecialCommand)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "dviRenderer::prescan_embedPS( cp = " << cp << " ) ";
#endif

    // Encapsulated Postscript File
    if (qstrnicmp(cp, "PSfile=", 7) != 0) {
        return;
    }

    QString command = QString::fromLocal8Bit(cp + 7);

    QString include_command = command.simplified();

    // The line is supposed to start with "..ile=", and then comes the
    // filename. Figure out what the filename is and stow it away. Of
    // course, this does not work if the filename contains spaces
    // (already the simplified() above is wrong). If you have
    // files like this, go away.
    QString EPSfilename = include_command;
    EPSfilename.truncate(EPSfilename.indexOf(QLatin1Char(' ')));

    // Strip enclosing quotation marks which are included by some LaTeX
    // macro packages (but not by others). This probably means that
    // graphic files are no longer found if the filename really does
    // contain quotes, but we don't really care that much.
    if ((EPSfilename.at(0) == QLatin1Char('"')) && (EPSfilename.at(EPSfilename.length() - 1) == QLatin1Char('"'))) {
        EPSfilename = EPSfilename.mid(1, EPSfilename.length() - 2);
    }

    // Now locate the Gfx file on the hard disk...
    EPSfilename = ghostscript_interface::locateEPSfile(EPSfilename, baseURL);

    // If the file is neither a PostScript not a PDF file, we exit here.
    // The graphic file is later read when the page is rendered.
    QMimeDatabase db;
    QMimeType const mime_type = db.mimeTypeForFile(EPSfilename, QMimeDatabase::MatchContent);
    QString const &mime_type_name = mime_type.isValid() ? mime_type.name() : QString();

    bool const is_ps_file = (mime_type_name == QLatin1String("application/postscript") || mime_type_name == QLatin1String("image/x-eps"));
    bool const is_pdf_file = (!is_ps_file && mime_type_name == QLatin1String("application/pdf"));
    if (!(is_ps_file || is_pdf_file)) {
        return;
    }

    QString originalFName = EPSfilename;

    embedPS_progress->setLabelText(i18n("Embedding %1", EPSfilename));
    qApp->processEvents();

    // If the EPSfilename really points to a PDF file, convert that file now.
    if (is_pdf_file) {
        EPSfilename = dviFile->convertPDFtoPS(EPSfilename);
    }

    if (!QFile::exists(EPSfilename)) {
        // Find the number of the page
        quint32 currentOffset = beginningOfSpecialCommand - dviFile->dvi_Data();
        int page = 0;
        for (; page < dviFile->total_pages; page++) {
            if ((dviFile->page_offset[page] <= currentOffset) && (currentOffset <= dviFile->page_offset[page + 1])) {
                break;
            }
        }
        if (is_pdf_file) {
            errorMsg += i18n("Page %1: The PDF file <strong>%2</strong> could not be converted to PostScript.<br>", page + 1, originalFName);
        } else {
            errorMsg += i18n("Page %1: The PostScript file <strong>%2</strong> could not be found.<br>", page + 1, originalFName);
        }
        embedPS_progress->setValue(embedPS_progress->value() + 1);
        qApp->processEvents();
        return;
    }

    // Now parse the arguments.
    int llx = 0;
    int lly = 0;
    int urx = 0;
    int ury = 0;
    int rwi = 0;
    int rhi = 0;
    int angle = 0;

    // just to avoid ambiguities; the filename could contain keywords
    include_command = include_command.mid(include_command.indexOf(QLatin1Char(' ')));

    parse_special_argument(include_command, "llx=", &llx);
    parse_special_argument(include_command, "lly=", &lly);
    parse_special_argument(include_command, "urx=", &urx);
    parse_special_argument(include_command, "ury=", &ury);
    parse_special_argument(include_command, "rwi=", &rwi);
    parse_special_argument(include_command, "rhi=", &rhi);
    parse_special_argument(include_command, "angle=", &angle);

    int clip = include_command.indexOf(QStringLiteral(" clip")); // -1 if clip keyword is not present, >= 0 otherwise

    // Generate the PostScript commands to be included
    QString PS = QStringLiteral("ps: @beginspecial %1 @llx %2 @lly %3 @urx %4 @ury").arg(llx).arg(lly).arg(urx).arg(ury);
    if (rwi != 0) {
        PS.append(QStringLiteral(" %1 @rwi").arg(rwi));
    }
    if (rhi != 0) {
        PS.append(QStringLiteral(" %1 @rhi").arg(rhi));
    }
    if (angle != 0) {
        PS.append(QStringLiteral(" %1 @angle").arg(angle));
    }
    if (clip != -1) {
        PS.append(QStringLiteral(" @clip"));
    }
    PS.append(QStringLiteral(" @setspecial\n"));

    QFile file(EPSfilename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            PS += stream.readLine().section(QLatin1Char('%'), 0, 0);
            PS += QLatin1Char('\n');
        }
        file.close();
    }
    PS.append(QStringLiteral("@endspecial"));
    PS = PS.simplified();

    _isModified = true;
    quint32 lengthOfOldSpecial = command_pointer - beginningOfSpecialCommand;
    quint32 lengthOfNewSpecial = PS.length() + 5;

    QVector<quint8> newDVI(dviFile->size_of_file + lengthOfNewSpecial - lengthOfOldSpecial);

    quint8 *commandPtrSav = command_pointer;
    quint8 *endPtrSav = end_pointer;
    end_pointer = newDVI.data() + dviFile->size_of_file + lengthOfNewSpecial - lengthOfOldSpecial;
    memcpy(newDVI.data(), dviFile->dvi_Data(), beginningOfSpecialCommand - dviFile->dvi_Data());
    command_pointer = newDVI.data() + (beginningOfSpecialCommand - dviFile->dvi_Data());
    command_pointer[0] = XXX4;
    command_pointer++;
    writeUINT32(PS.length());
    memcpy(newDVI.data() + (beginningOfSpecialCommand - dviFile->dvi_Data()) + 5, PS.toLatin1().constData(), PS.length());
    memcpy(
        newDVI.data() + (beginningOfSpecialCommand - dviFile->dvi_Data()) + lengthOfNewSpecial, beginningOfSpecialCommand + lengthOfOldSpecial, dviFile->size_of_file - (beginningOfSpecialCommand - dviFile->dvi_Data()) - lengthOfOldSpecial);

    // Adjust page pointers in the DVI file
    dviFile->size_of_file = dviFile->size_of_file + lengthOfNewSpecial - lengthOfOldSpecial;
    end_pointer = newDVI.data() + dviFile->size_of_file;
    quint32 currentOffset = beginningOfSpecialCommand - dviFile->dvi_Data();
    for (int i = 0; i < dviFile->total_pages; i++) {
        if (dviFile->page_offset[i] > currentOffset) {
            dviFile->page_offset[i] = dviFile->page_offset[i] + lengthOfNewSpecial - lengthOfOldSpecial;
            command_pointer = dviFile->page_offset[i] + newDVI.data() + 4 * 10 + 1;
            quint32 a = readUINT32();
            if (a > currentOffset) {
                a = a + lengthOfNewSpecial - lengthOfOldSpecial;
                command_pointer = dviFile->page_offset[i] + newDVI.data() + 4 * 10 + 1;
                writeUINT32(a);
            }
        }
    }

    dviFile->beginning_of_postamble = dviFile->beginning_of_postamble + lengthOfNewSpecial - lengthOfOldSpecial;
    dviFile->page_offset[int(dviFile->total_pages)] = dviFile->beginning_of_postamble;

    command_pointer = newDVI.data() + dviFile->beginning_of_postamble + 1;
    quint32 a = readUINT32();
    if (a > currentOffset) {
        a = a + lengthOfNewSpecial - lengthOfOldSpecial;
        command_pointer = newDVI.data() + dviFile->beginning_of_postamble + 1;
        writeUINT32(a);
    }

    command_pointer = newDVI.data() + dviFile->size_of_file - 1;
    while ((*command_pointer == TRAILER) && (command_pointer > newDVI.data())) {
        command_pointer--;
    }
    command_pointer -= 4;
    writeUINT32(dviFile->beginning_of_postamble);
    command_pointer -= 4;

    command_pointer = commandPtrSav;
    end_pointer = endPtrSav;

    // Modify all pointers to point to the newly allocated memory
    command_pointer = newDVI.data() + (command_pointer - dviFile->dvi_Data()) + lengthOfNewSpecial - lengthOfOldSpecial;
    end_pointer = newDVI.data() + (end_pointer - dviFile->dvi_Data()) + lengthOfNewSpecial - lengthOfOldSpecial;

    dviFile->setNewData(newDVI);

    embedPS_progress->setValue(embedPS_progress->value() + 1);
    qApp->processEvents();
    return;
}

void dviRenderer::prescan_removePageSizeInfo(char *cp, quint8 *beginningOfSpecialCommand)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "dviRenderer::prescan_embedPS( cp = " << cp << " ) ";
#endif

    // Encapsulated Postscript File
    if (qstrnicmp(cp, "papersize=", 10) != 0) {
        return;
    }

    for (quint8 *ptr = beginningOfSpecialCommand; ptr < command_pointer; ptr++) {
        *ptr = NOP;
    }
}

void dviRenderer::prescan_ParsePapersizeSpecial(const QString &_cp)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "Papersize-Special : papersize" << _cp;
#endif

    QString cp = _cp.simplified();

    if (cp[0] == QLatin1Char('=')) {
        cp = cp.mid(1);
        dviFile->suggestedPageSize = new pageSize;
        dviFile->suggestedPageSize->setPageSize(cp);
    } else {
        printErrorMsgForSpecials(i18n("The papersize data '%1' could not be parsed.", cp));
    }

    return;
}

void dviRenderer::prescan_ParseBackgroundSpecial(const QString &cp)
{
    QColor col = parseColorSpecification(cp.trimmed());
    if (col.isValid()) {
        for (quint16 page = current_page; page < dviFile->total_pages; page++) {
            PS_interface->setBackgroundColor(page, col);
        }
    }
    return;
}

void dviRenderer::prescan_ParseHTMLAnchorSpecial(const QString &_cp)
{
    QString cp = _cp;
    cp.truncate(cp.indexOf(QLatin1Char('"')));
    Length l;
    l.setLength_in_inch(currinf.data.dvi_v / (resolutionInDPI * shrinkfactor));
    anchorList[cp] = Anchor(current_page + 1, l);
}

void dviRenderer::prescan_ParsePSHeaderSpecial(const QString &cp)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "PostScript-special, header " << cp;
#endif

    QString _file = cp;

    // If the file is not found in the current directory, use kpsewhich
    // to find it.
    if (!QFile::exists(_file)) {
        // Otherwise, use kpsewhich to find the eps file.
        // Make sure kpsewhich is in PATH and not just in the CWD
        static const QString fullPath = QStandardPaths::findExecutable(QStringLiteral("kpsewhich"));
        if (!fullPath.isEmpty()) {
            KProcess proc;
            proc << fullPath << cp;
            proc.setOutputChannelMode(KProcess::SeparateChannels);
            proc.execute();
            _file = QString::fromLocal8Bit(proc.readLine().trimmed());
        }
    }

    if (QFile::exists(_file)) {
        PS_interface->PostScriptHeaderString->append(QStringLiteral(" (%1) run\n").arg(_file));
    }
}

void dviRenderer::prescan_ParsePSBangSpecial(const QString &cp)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "PostScript-special, literal header " << cp;
#endif

    PS_interface->PostScriptHeaderString->append(QStringLiteral(" @defspecial \n"));
    PS_interface->PostScriptHeaderString->append(cp);
    PS_interface->PostScriptHeaderString->append(QStringLiteral(" @fedspecial \n"));
}

void dviRenderer::prescan_ParsePSQuoteSpecial(const QString &cp)
{
#ifdef DEBUG_PRESCAN
    qCCritical(OkularDviDebug) << "PostScript-special, literal PostScript " << cp;
#endif

    double PS_H = (currinf.data.dvi_h * 300.0) / (65536 * 1200) - 300;
    double PS_V = (currinf.data.dvi_v * 300.0) / 1200 - 300;
    PostScriptOutPutString->append(QStringLiteral(" %1 %2 moveto\n").arg(PS_H).arg(PS_V));
    PostScriptOutPutString->append(QStringLiteral(" @beginspecial @setspecial \n"));
    PostScriptOutPutString->append(cp);
    PostScriptOutPutString->append(QStringLiteral(" @endspecial \n"));
}

void dviRenderer::prescan_ParsePSSpecial(const QString &cp)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "PostScript-special, direct PostScript " << cp;
#endif

    // Unfortunately, in some TeX distribution the hyperref package uses
    // the dvips driver by default, rather than the hypertex driver. As
    // a result, the DVI files produced are full of PostScript that
    // specifies links and anchors, and KDVI would call the ghostscript
    // interpreter for every page which makes it really slow. This is a
    // major nuisance, so that we try to filter and interpret the
    // hypertex generated PostScript here.
    if (cp.startsWith(QLatin1String("ps:SDict begin"))) {
        // We suspect this may be hyperref generated nonsense. Let's check
        // for some known code that hyperref generates.
        if (cp == QLatin1String("ps:SDict begin H.S end")) {
            return; // start of hyperref rectangle
        }
        if (cp == QLatin1String("ps:SDict begin H.R end")) {
            return; // end of hyperref rectangle
        }
        if (cp.endsWith(QLatin1String("H.A end"))) {
            return; // end of hyperref anchor
        }
        if (cp.endsWith(QLatin1String("H.L end"))) {
            return; // end of hyperref link
        }
        if (cp.startsWith(QLatin1String("ps:SDict begin /product where{pop product(Distiller)"))) {
            return; // hyperref tries to work around Distiller bug
        }
        if (cp.startsWith(QLatin1String("ps:SDict begin [")) && cp.endsWith(QLatin1String(" pdfmark end"))) { // hyperref definition of link/anchor/bookmark/etc
            if (cp.contains(QStringLiteral("/DEST"))) {                                                       // The PostScript code defines an anchor
                QString anchorName = cp.section(QLatin1Char('('), 1, 1).section(QLatin1Char(')'), 0, 0);
                Length l;
                l.setLength_in_inch(currinf.data.dvi_v / (resolutionInDPI * shrinkfactor));
                anchorList[anchorName] = Anchor(current_page + 1, l);
            }
            // The PostScript code defines a bookmark
            if (cp.contains(QStringLiteral("/Dest")) && cp.contains(QStringLiteral("/Title"))) {
                const QString childrenNumberAndMoreStuff = cp.section(QLatin1Char('-'), 1, 1); // Contains from the - symbol to the end of cp, effectively containing the number of children and some stuff after it
                int indexOfFirstNonDigit = 0;
                for (const QChar &c : childrenNumberAndMoreStuff) {
                    if (c.isDigit()) {
                        ++indexOfFirstNonDigit;
                    } else {
                        break;
                    }
                }
                prebookmarks.append(PreBookmark(PDFencodingToQString(cp.section(QLatin1Char('('), 2, 2).section(QLatin1Char(')'), 0, 0)),
                                                cp.section(QLatin1Char('('), 1, 1).section(QLatin1Char(')'), 0, 0),
                                                childrenNumberAndMoreStuff.leftRef(indexOfFirstNonDigit).toUInt()));
            }
            return;
        }
    }

    double PS_H = (currinf.data.dvi_h * 300.0) / (65536 * 1200) - 300;
    double PS_V = (currinf.data.dvi_v * 300.0) / 1200 - 300;

    if (cp.indexOf(QStringLiteral("ps::[begin]"), 0, Qt::CaseInsensitive) == 0) {
        PostScriptOutPutString->append(QStringLiteral(" %1 %2 moveto\n").arg(PS_H).arg(PS_V));
        PostScriptOutPutString->append(QStringLiteral(" %1\n").arg(cp.mid(11)));
    } else {
        if (cp.indexOf(QStringLiteral("ps::[end]"), 0, Qt::CaseInsensitive) == 0) {
            PostScriptOutPutString->append(QStringLiteral(" %1\n").arg(cp.mid(9)));
        } else {
            if (cp.indexOf(QStringLiteral("ps::"), 0, Qt::CaseInsensitive) == 0) {
                PostScriptOutPutString->append(QStringLiteral(" %1\n").arg(cp.mid(4)));
            } else {
                PostScriptOutPutString->append(QStringLiteral(" %1 %2 moveto\n").arg(PS_H).arg(PS_V));
                PostScriptOutPutString->append(QStringLiteral(" %1\n").arg(cp.mid(3)));
            }
        }
    }
}

void dviRenderer::prescan_ParsePSFileSpecial(const QString &cp)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "epsf-special: psfile=" << cp;
#endif

    QString include_command = cp.simplified();

    // The line is supposed to start with "..ile=", and then comes the
    // filename. Figure out what the filename is and stow it away. Of
    // course, this does not work if the filename contains spaces
    // (already the simplified() above is wrong). If you have
    // files like this, go away.
    QString EPSfilename = include_command;
    EPSfilename.truncate(EPSfilename.indexOf(QLatin1Char(' ')));

    // Strip enclosing quotation marks which are included by some LaTeX
    // macro packages (but not by others). This probably means that
    // graphic files are no longer found if the filename really does
    // contain quotes, but we don't really care that much.
    if ((EPSfilename.at(0) == QLatin1Char('\"')) && (EPSfilename.at(EPSfilename.length() - 1) == QLatin1Char('\"'))) {
        EPSfilename = EPSfilename.mid(1, EPSfilename.length() - 2);
    }

    // If the file name ends in 'png', 'gif', 'jpg' or 'jpeg', we assume
    // that this is NOT a PostScript file, and we exit here.
    QString ending = EPSfilename.section(QLatin1Char('.'), -1).toLower();
    if ((ending == QLatin1String("png")) || (ending == QLatin1String("gif")) || (ending == QLatin1String("jpg")) || (ending == QLatin1String("jpeg"))) {
        dviFile->numberOfExternalNONPSFiles++;
        return;
    }

    // Now assume that the graphics file *is* a PostScript file
    dviFile->numberOfExternalPSFiles++;

    // Now locate the Gfx file on the hard disk...
    EPSfilename = ghostscript_interface::locateEPSfile(EPSfilename, baseURL);

    // If the EPSfilename really points to a PDF file, convert that file now.
    if (ending == QLatin1String("pdf")) {
        QString convErrorMsg;
        EPSfilename = dviFile->convertPDFtoPS(EPSfilename, &convErrorMsg);
        if (convErrorMsg.isEmpty() != true) {
            Q_EMIT error(convErrorMsg, -1);
            return;
        }
    }

    // Now parse the arguments.
    int llx = 0;
    int lly = 0;
    int urx = 0;
    int ury = 0;
    int rwi = 0;
    int rhi = 0;
    int angle = 0;

    // just to avoid ambiguities; the filename could contain keywords
    include_command = include_command.mid(include_command.indexOf(QLatin1Char(' ')));

    parse_special_argument(include_command, "llx=", &llx);
    parse_special_argument(include_command, "lly=", &lly);
    parse_special_argument(include_command, "urx=", &urx);
    parse_special_argument(include_command, "ury=", &ury);
    parse_special_argument(include_command, "rwi=", &rwi);
    parse_special_argument(include_command, "rhi=", &rhi);
    parse_special_argument(include_command, "angle=", &angle);

    int clip = include_command.indexOf(QStringLiteral(" clip")); // -1 if clip keyword is not present, >= 0 otherwise

    if (QFile::exists(EPSfilename)) {
        double PS_H = (currinf.data.dvi_h * 300.0) / (65536 * 1200) - 300;
        double PS_V = (currinf.data.dvi_v * 300.0) / 1200 - 300;
        PostScriptOutPutString->append(QStringLiteral(" %1 %2 moveto\n").arg(PS_H).arg(PS_V));
        PostScriptOutPutString->append(QStringLiteral("@beginspecial "));
        PostScriptOutPutString->append(QStringLiteral(" %1 @llx").arg(llx));
        PostScriptOutPutString->append(QStringLiteral(" %1 @lly").arg(lly));
        PostScriptOutPutString->append(QStringLiteral(" %1 @urx").arg(urx));
        PostScriptOutPutString->append(QStringLiteral(" %1 @ury").arg(ury));
        if (rwi != 0) {
            PostScriptOutPutString->append(QStringLiteral(" %1 @rwi").arg(rwi));
        }
        if (rhi != 0) {
            PostScriptOutPutString->append(QStringLiteral(" %1 @rhi").arg(rhi));
        }
        if (angle != 0) {
            PostScriptOutPutString->append(QStringLiteral(" %1 @angle").arg(angle));
        }
        if (clip != -1) {
            PostScriptOutPutString->append(QStringLiteral(" @clip"));
        }
        PostScriptOutPutString->append(QStringLiteral(" @setspecial \n"));
        PostScriptOutPutString->append(QStringLiteral(" (%1) run\n").arg(EPSfilename));
        PostScriptOutPutString->append(QStringLiteral("@endspecial \n"));
    }

    return;
}

void dviRenderer::prescan_ParseSourceSpecial(const QString &cp)
{
    // if no rendering takes place, i.e. when the DVI file is first
    // loaded, generate a DVI_SourceFileAnchor. These anchors are used
    // in forward search, i.e. to relate references line
    // "src:123file.tex" to positions in the DVI file

    // extract the file name and the numeral part from the string
    qint32 j;
    for (j = 0; j < cp.length(); j++) {
        if (!cp.at(j).isNumber()) {
            break;
        }
    }
    quint32 sourceLineNumber = cp.leftRef(j).toUInt();
    QFileInfo fi1(dviFile->filename);
    QString sourceFileName = QFileInfo(fi1.dir(), cp.mid(j).trimmed()).absoluteFilePath();
    Length l;
    l.setLength_in_inch(currinf.data.dvi_v / (resolutionInDPI * shrinkfactor));
    DVI_SourceFileAnchor sfa(sourceFileName, sourceLineNumber, current_page + 1, l);
    sourceHyperLinkAnchors.push_back(sfa);
}

void dviRenderer::prescan_parseSpecials(char *cp, quint8 *)
{
    QString special_command = QString::fromUtf8(cp);

    // Now to those specials which are only interpreted during the
    // prescan phase, and NOT during rendering.

    // PaperSize special
    if (qstrnicmp(cp, "papersize", 9) == 0) {
        prescan_ParsePapersizeSpecial(special_command.mid(9));
        return;
    }

    // color special for background color
    if (qstrnicmp(cp, "background", 10) == 0) {
        prescan_ParseBackgroundSpecial(special_command.mid(10));
        return;
    }

    // HTML anchor special
    if (qstrnicmp(cp, "html:<A name=", 13) == 0) {
        prescan_ParseHTMLAnchorSpecial(special_command.mid(14));
        return;
    }

    // Postscript Header File
    if (qstrnicmp(cp, "header=", 7) == 0) {
        prescan_ParsePSHeaderSpecial(special_command.mid(7));
        return;
    }

    // Literal Postscript Header
    if (cp[0] == '!') {
        prescan_ParsePSBangSpecial(special_command.mid(1));
        return;
    }

    // Literal Postscript inclusion
    if (cp[0] == '"') {
        prescan_ParsePSQuoteSpecial(special_command.mid(1));
        return;
    }

    // PS-Postscript inclusion
    if (qstrnicmp(cp, "ps:", 3) == 0) {
        prescan_ParsePSSpecial(special_command);
        return;
    }

    // Encapsulated Postscript File
    if (qstrnicmp(cp, "PSfile=", 7) == 0) {
        prescan_ParsePSFileSpecial(special_command.mid(7));
        return;
    }

    // source special
    if (qstrnicmp(cp, "src:", 4) == 0) {
        prescan_ParseSourceSpecial(special_command.mid(4));
        return;
    }

    // Finally there are those special commands which must be considered
    // both during rendering and during the pre-scan phase

    // HTML anchor end
    if (qstrnicmp(cp, "html:</A>", 9) == 0) {
        html_anchor_end();
        return;
    }

    return;
}

void dviRenderer::prescan_setChar(unsigned int ch)
{
    TeXFontDefinition *fontp = currinf.fontp;
    if (fontp == nullptr) {
        return;
    }

    if (currinf.set_char_p == &dviRenderer::set_char) {
        glyph *g = ((TeXFont *)(currinf.fontp->font))->getGlyph(ch, true, globalColor);
        if (g == nullptr) {
            return;
        }
        currinf.data.dvi_h += (int)(currinf.fontp->scaled_size_in_DVI_units * dviFile->getCmPerDVIunit() * (1200.0 / 2.54) / 16.0 * g->dvi_advance_in_units_of_design_size_by_2e20 + 0.5);
        return;
    }

    if (currinf.set_char_p == &dviRenderer::set_vf_char) {
        macro *m = &currinf.fontp->macrotable[ch];
        if (m->pos == nullptr) {
            return;
        }
        currinf.data.dvi_h += (int)(currinf.fontp->scaled_size_in_DVI_units * dviFile->getCmPerDVIunit() * (1200.0 / 2.54) / 16.0 * m->dvi_advance_in_units_of_design_size_by_2e20 + 0.5);
        return;
    }
}

void dviRenderer::prescan(parseSpecials specialParser)
{
#ifdef DEBUG_PRESCAN
    qCDebug(OkularDviDebug) << "dviRenderer::prescan( ... )";
#endif

    if (resolutionInDPI == 0.0) {
        setResolution(100);
    }

    qint32 RRtmp = 0, WWtmp = 0, XXtmp = 0, YYtmp = 0, ZZtmp = 0;
    quint8 ch;
    double fontPixelPerDVIunit = dviFile->getCmPerDVIunit() * 1200.0 / 2.54;

    stack.clear();

    currinf.fontp = nullptr;
    currinf.set_char_p = &dviRenderer::set_no_char;

    for (;;) {
        ch = readUINT8();

        if (ch <= (unsigned char)(SETCHAR0 + 127)) {
            prescan_setChar(ch);
            continue;
        }

        if (FNTNUM0 <= ch && ch <= (unsigned char)(FNTNUM0 + 63)) {
            currinf.fontp = currinf.fonttable->value(ch - FNTNUM0);
            if (currinf.fontp == nullptr) {
                errorMsg = i18n("The DVI code referred to font #%1, which was not previously defined.", ch - FNTNUM0);
                return;
            }
            currinf.set_char_p = currinf.fontp->set_char_p;
            continue;
        }

        qint32 a, b;

        switch (ch) {
        case SET1:
            prescan_setChar(readUINT8());
            break;

        case SETRULE:
            /* Be careful, dvicopy outputs rules with height =
               0x80000000. We don't want any SIGFPE here. */
            a = readUINT32();
            b = readUINT32();
            b = ((long)(b * 65536.0 * fontPixelPerDVIunit));
            currinf.data.dvi_h += b;
            break;

        case PUTRULE:
            a = readUINT32();
            b = readUINT32();
            break;

        case PUT1:
        case NOP:
            break;

        case BOP:
            command_pointer += 11 * 4;
            currinf.data.dvi_h = 1200 << 16; // Reminder: DVI-coordinates start at (1",1") from top of page
            currinf.data.dvi_v = 1200;
            currinf.data.pxl_v = int(currinf.data.dvi_v / shrinkfactor);
            currinf.data.w = currinf.data.x = currinf.data.y = currinf.data.z = 0;
            break;

        case PUSH:
            stack.push(currinf.data);
            break;

        case POP:
            if (stack.isEmpty()) {
                return;
            } else {
                currinf.data = stack.pop();
            }
            break;

        case RIGHT1:
        case RIGHT2:
        case RIGHT3:
        case RIGHT4:
            RRtmp = readINT(ch - RIGHT1 + 1);
            currinf.data.dvi_h += ((long)(RRtmp * 65536.0 * fontPixelPerDVIunit));
            break;

        case W1:
        case W2:
        case W3:
        case W4:
            WWtmp = readINT(ch - W0);
            currinf.data.w = ((long)(WWtmp * 65536.0 * fontPixelPerDVIunit));
            // fallthrough
        case W0:
            currinf.data.dvi_h += currinf.data.w;
            break;

        case X1:
        case X2:
        case X3:
        case X4:
            XXtmp = readINT(ch - X0);
            currinf.data.x = ((long)(XXtmp * 65536.0 * fontPixelPerDVIunit));
            // fallthrough
        case X0:
            currinf.data.dvi_h += currinf.data.x;
            break;

        case DOWN1:
        case DOWN2:
        case DOWN3:
        case DOWN4: {
            qint32 DDtmp = readINT(ch - DOWN1 + 1);
            currinf.data.dvi_v += ((long)(DDtmp * 65536.0 * fontPixelPerDVIunit)) / 65536;
            currinf.data.pxl_v = int(currinf.data.dvi_v / shrinkfactor);
        } break;

        case Y1:
        case Y2:
        case Y3:
        case Y4:
            YYtmp = readINT(ch - Y0);
            currinf.data.y = ((long)(YYtmp * 65536.0 * fontPixelPerDVIunit));
            // fallthrough
        case Y0:
            currinf.data.dvi_v += currinf.data.y / 65536;
            currinf.data.pxl_v = int(currinf.data.dvi_v / shrinkfactor);
            break;

        case Z1:
        case Z2:
        case Z3:
        case Z4:
            ZZtmp = readINT(ch - Z0);
            currinf.data.z = ((long)(ZZtmp * 65536.0 * fontPixelPerDVIunit));
            // fallthrough
        case Z0:
            currinf.data.dvi_v += currinf.data.z / 65536;
            currinf.data.pxl_v = int(currinf.data.dvi_v / shrinkfactor);
            break;

        case FNT1:
        case FNT2:
        case FNT3:
        case FNT4:
            currinf.fontp = currinf.fonttable->value(readUINT(ch - FNT1 + 1));
            if (currinf.fontp == nullptr) {
                return;
            }
            currinf.set_char_p = currinf.fontp->set_char_p;
            break;

        case XXX1:
        case XXX2:
        case XXX3:
        case XXX4: {
            quint8 *beginningOfSpecialCommand = command_pointer - 1;
            a = readUINT(ch - XXX1 + 1);
            if (a > 0) {
                char *cmd = new char[a + 1];
                strncpy(cmd, (char *)command_pointer, a);
                command_pointer += a;
                cmd[a] = '\0';
                (this->*specialParser)(cmd, beginningOfSpecialCommand);
                delete[] cmd;
            }
        } break;

        case FNTDEF1:
        case FNTDEF2:
        case FNTDEF3:
        case FNTDEF4:
            command_pointer += 12 + ch - FNTDEF1 + 1;
            command_pointer += readUINT8() + readUINT8();
            break;

        default:
            return;
        } /* end switch */
    }     /* end for */
}
