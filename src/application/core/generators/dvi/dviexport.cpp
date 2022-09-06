// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
/**
 * \file dviexport.h
 * Distributed under the GNU GPL version 2 or (at your option)
 * any later version. See accompanying file COPYING or copy at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * \author Angus Leeming
 * \author Stefan Kebekus
 *
 * Classes DVIExportToPDF and DVIExportToPS control the export
 * of a DVI file to PDF or PostScript format, respectively.
 * Common functionality is factored out into a common base class,
 * DVIExport which itself derives from QSharedData allowing easy,
 * polymorphic storage of multiple QExplicitlySharedDataPointer<DVIExport> variables
 * in a container of all exported processes.
 */

#include <config.h>
#include <core/fileprinter.h>

#include "dviexport.h"

#include "debug_dvi.h"
#include "dviFile.h"
#include "dviRenderer.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KProcess>

#include <QEventLoop>
#include <QFileInfo>
#include <QLabel>
#include <QTemporaryFile>

#include <QStandardPaths>
#include <cassert>

DVIExport::DVIExport(dviRenderer &parent)
    : started_(false)
    , process_(nullptr)
    , parent_(&parent)
{
    connect(this, &DVIExport::error, &parent, &dviRenderer::error);
}

DVIExport::~DVIExport()
{
    delete process_;
}

void DVIExport::start(const QString &command, const QStringList &args, const QString &working_directory, const QString &error_message)
{
    assert(!process_);

    process_ = new KProcess;
    process_->setOutputChannelMode(KProcess::MergedChannels);
    process_->setNextOpenMode(QIODevice::Text);
    connect(process_, &KProcess::readyReadStandardOutput, this, &DVIExport::output_receiver);
    connect(process_, QOverload<int, QProcess::ExitStatus>::of(&KProcess::finished), this, &DVIExport::finished);

    *process_ << command << args;

    if (!working_directory.isEmpty()) {
        process_->setWorkingDirectory(working_directory);
    }

    error_message_ = error_message;

    process_->start();
    if (!process_->waitForStarted(-1)) {
        qCCritical(OkularDviDebug) << command << " failed to start";
    } else {
        started_ = true;
    }

    if (parent_->m_eventLoop) {
        parent_->m_eventLoop->exec();
    }
}

void DVIExport::abort_process_impl()
{
    // deleting process_ kills the external process itself
    // if it's still running.
    delete process_;
    process_ = nullptr;
}

void DVIExport::finished_impl(int exit_code)
{
    if (process_ && exit_code != 0) {
        Q_EMIT error(error_message_, -1);
    }
    // Remove this from the store of all export processes.
    parent_->m_eventLoop->exit(exit_code);
    parent_->export_finished(this);
}

void DVIExport::output_receiver()
{
    if (process_) {
        process_->readAllStandardOutput();
    }
}

DVIExportToPDF::DVIExportToPDF(dviRenderer &parent, const QString &output_name)
    : DVIExport(parent)
{
    // Neither of these should happen. Paranoia checks.
    if (!parent.dviFile) {
        return;
    }
    const dvifile &dvi = *(parent.dviFile);

    const QFileInfo input(dvi.filename);
    if (!input.exists() || !input.isReadable()) {
        return;
    }

    if (QStandardPaths::findExecutable(QStringLiteral("dvipdfm")).isEmpty()) {
        Q_EMIT error(i18n("<qt><p>Okular could not locate the program <em>dvipdfm</em> on your computer. This program is "
                          "essential for the export function to work. You can, however, convert "
                          "the DVI-file to PDF using the print function of Okular, but that will often "
                          "produce documents which print okay, but are of inferior quality if viewed in "
                          "Acrobat Reader. It may be wise to upgrade to a more recent version of your "
                          "TeX distribution which includes the <em>dvipdfm</em> program.</p>"
                          "<p>Hint to the perplexed system administrator: Okular uses the PATH environment variable "
                          "when looking for programs.</p></qt>"),
                     -1);
        return;
    }

    if (output_name.isEmpty()) {
        return;
    }

    start(QStringLiteral("dvipdfm"),
          QStringList() << QStringLiteral("-o") << output_name << dvi.filename,
          QFileInfo(dvi.filename).absolutePath(),
          i18n("<qt>The external program 'dvipdfm', which was used to export the file, reported an error. "
               "You might wish to look at the <strong>document info dialog</strong> which you will "
               "find in the File-Menu for a precise error report.</qt>"));
}

DVIExportToPS::DVIExportToPS(dviRenderer &parent, const QString &output_name, const QStringList &options, QPrinter *printer, bool useFontHinting, QPrinter::Orientation orientation)
    : DVIExport(parent)
    , printer_(printer)
    , orientation_(orientation)
{
    // None of these should happen. Paranoia checks.
    if (!parent.dviFile) {
        return;
    }
    const dvifile &dvi = *(parent.dviFile);

    const QFileInfo input(dvi.filename);
    if (!input.exists() || !input.isReadable()) {
        return;
    }

    if (dvi.page_offset.isEmpty()) {
        return;
    }

    if (dvi.numberOfExternalNONPSFiles != 0) {
        Q_EMIT error(i18n("<qt>This DVI file refers to external graphic files which are not in PostScript format, and cannot be handled by the "
                          "<em>dvips</em> program that Okular uses internally to print or to export to PostScript. The functionality that "
                          "you require is therefore unavailable in this version of Okular.</qt>"),
                     -1);
        return;
    }

    if (QStandardPaths::findExecutable(QStringLiteral("dvips")).isEmpty()) {
        Q_EMIT error(i18n("<qt><p>Okular could not locate the program <em>dvips</em> on your computer. "
                          "That program is essential for the export function to work.</p>"
                          "<p>Hint to the perplexed system administrator: Okular uses the PATH environment "
                          "variable when looking for programs.</p></qt>"),
                     -1);
        return;
    }

    if (output_name.isEmpty()) {
        return;
    }

    output_name_ = output_name;

    // There is a major problem with dvips, at least 5.86 and lower: the
    // arguments of the option "-pp" refer to TeX-pages, not to
    // sequentially numbered pages. For instance "-pp 7" may refer to 3
    // or more pages: one page "VII" in the table of contents, a page
    // "7" in the text body, and any number of pages "7" in various
    // appendices, indices, bibliographies, and so forth. KDVI currently
    // uses the following disgusting workaround: if the "options"
    // variable is used, the DVI-file is copied to a temporary file, and
    // all the page numbers are changed into a sequential ordering
    // (using UNIX files, and taking manually care of CPU byte
    // ordering). Finally, dvips is then called with the new file, and
    // the file is afterwards deleted. Isn't that great?

    // A similar problem occurs with DVI files that contain page size
    // information. On these files, dvips pointblank refuses to change
    // the page orientation or set another page size. Thus, if the
    // DVI-file does contain page size information, we remove that
    // information first.

    // input_name is the name of the DVI which is used by dvips, either
    // the original file, or a temporary file with a new numbering.
    QString input_name = dvi.filename;
    if (!options.isEmpty() || dvi.suggestedPageSize != nullptr) {
        // Get a name for a temporary file.
        // Must open the QTemporaryFile to access the name.
        QTemporaryFile tmpfile;
        tmpfile.setAutoRemove(false);
        tmpfile.open();
        tmpfile_name_ = tmpfile.fileName();
        tmpfile.close();

        input_name = tmpfile_name_;

        fontPool fp(useFontHinting);
        dvifile newFile(&dvi, &fp);

        // Renumber pages
        newFile.renumber();

        const quint16 saved_current_page = parent.current_page;
        dvifile *saved_dvi = parent.dviFile;
        parent.dviFile = &newFile;
        parent.errorMsg = QString();

        // Remove any page size information from the file
        for (parent.current_page = 0; parent.current_page < newFile.total_pages; parent.current_page++) {
            if (parent.current_page < newFile.total_pages) {
                parent.command_pointer = newFile.dvi_Data() + parent.dviFile->page_offset[int(parent.current_page)];
                parent.end_pointer = newFile.dvi_Data() + parent.dviFile->page_offset[int(parent.current_page + 1)];
            } else {
                parent.command_pointer = nullptr;
                parent.end_pointer = nullptr;
            }

            memset((char *)&parent.currinf.data, 0, sizeof(parent.currinf.data));
            parent.currinf.fonttable = &(parent.dviFile->tn_table);
            parent.currinf._virtual = nullptr;
            parent.prescan(&dviRenderer::prescan_removePageSizeInfo);
        }

        parent.current_page = saved_current_page;
        parent.dviFile = saved_dvi;
        newFile.saveAs(input_name);
    }

    QStringList args;
    if (!printer) {
        // Export hyperlinks
        args << QStringLiteral("-z");
    }

    if (!options.isEmpty()) {
        args += options;
    }

    args << input_name << QStringLiteral("-o") << output_name_;

    start(QStringLiteral("dvips"),
          args,
          QFileInfo(dvi.filename).absolutePath(),
          i18n("<qt>The external program 'dvips', which was used to export the file, reported an error. "
               "You might wish to look at the <strong>document info dialog</strong> which you will "
               "find in the File-Menu for a precise error report.</qt>"));
}

void DVIExportToPS::finished_impl(int exit_code)
{
    if (printer_ && !output_name_.isEmpty()) {
        const QFileInfo output(output_name_);
        if (output.exists() && output.isReadable()) {
            // I'm not 100% sure on this, think we still need to select pages in export to ps above
            Okular::FilePrinter::printFile((*printer_), output_name_, orientation_, Okular::FilePrinter::ApplicationDeletesFiles, Okular::FilePrinter::ApplicationSelectsPages, QString());
        }
    }

    if (!tmpfile_name_.isEmpty()) {
        // Delete the file.
        QFile(tmpfile_name_).remove();
        tmpfile_name_.clear();
    }

    DVIExport::finished_impl(exit_code);
}

void DVIExportToPS::abort_process_impl()
{
    if (!tmpfile_name_.isEmpty()) {
        // Delete the file.
        QFile(tmpfile_name_).remove();
        tmpfile_name_.clear();
    }

    printer_ = nullptr;

    DVIExport::abort_process_impl();
}
