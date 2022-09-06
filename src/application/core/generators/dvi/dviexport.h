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

#ifndef DVIEXPORT_H
#define DVIEXPORT_H

#include <QExplicitlySharedDataPointer>

#include <QObject>
#include <QPrinter>

class dviRenderer;
class KProcess;
class QStringList;

class DVIExport : public QObject, public QSharedData
{
    Q_OBJECT
public:
    ~DVIExport() override;

    /** @c started() Flags whether or not the external process was
     *  spawned successfully.
     *  Can be used to decide whether to discard the DVIExport variable,
     *  or to store it and await notification that the external process
     *  has finished.
     */
    bool started() const
    {
        return started_;
    }

Q_SIGNALS:
    void error(const QString &message, int duration);

protected:
    /** @param parent is stored internally in order to inform the parent
     *  that the external process has finished and that this variable
     *  can be removed from any stores.
     */
    explicit DVIExport(dviRenderer &parent);

    /** Spawns the external process having connected slots to the child
     *  process's stdin and stdout streams.
     */
    void start(const QString &command, const QStringList &args, const QString &working_directory, const QString &error_message);

    /** The real implementation of the abort_process() slot that is
     *  called when the fontProcessDialog is closed by the user,
     *  indicating that the export should be halted.
     */
    virtual void abort_process_impl();

    /** The real implementation of the finished() slot that is called
     *  when the external process finishes.
     *  @param exit_code the exit code returned by the external process.
     */
    virtual void finished_impl(int exit_code);

private Q_SLOTS:
    /// Calls an impl() inline so that derived classes don't need slots.
    void abort_process()
    {
        abort_process_impl();
    }
    void finished(int exit_code)
    {
        finished_impl(exit_code);
    }

    /** This slot receives all output from the child process's stdin
     *  and stdout streams.
     */
    void output_receiver();

private:
    QString error_message_;
    bool started_;
    KProcess *process_;
    dviRenderer *parent_;
};

class DVIExportToPDF : public DVIExport
{
    Q_OBJECT

public:
    /** @param parent is stored internally in order to inform the parent
     *  that the external process has finished.
     *  @param output_name is the name of the PDF file that is
     *  to contain the exported data.   */
    DVIExportToPDF(dviRenderer &parent, const QString &output_name);
};

class DVIExportToPS : public DVIExport
{
    Q_OBJECT

public:
    /** @param parent is stored internally in order to inform the parent
     *  that the external process has finished.
     *  @param output_name is the name of the PostScript file that is
     *  to contain the exported data.
     *  @param options extra command line arguments that are to be
     *  passed to the external process's argv command line.
     *  @param printer having generated the PostScript file, it is passed
     *  to @c printer (if not null).
     *  @param useFontHinting boolean that defines whether to use font hinting.
     *  @param orientation the original orientation of the document
     */
    DVIExportToPS(dviRenderer &parent, const QString &output_name, const QStringList &options, QPrinter *printer, bool useFontHinting, QPrinter::Orientation orientation = QPrinter::Portrait);

private:
    void abort_process_impl() override;
    void finished_impl(int exit_code) override;

    QPrinter *printer_;
    QString output_name_;
    QString tmpfile_name_;
    QPrinter::Orientation orientation_;
};

#endif
