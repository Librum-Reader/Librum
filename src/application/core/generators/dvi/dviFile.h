// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// Class: dviFile
//
// Class that represents a DVI file. Part of KDVI - A DVI previewing
// plugin for kviewshell.
//
// SPDX-FileCopyrightText: 2004-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _DVIFILE_H
#define _DVIFILE_H

#include "bigEndianByteReader.h"

#include <QHash>
#include <QMap>
#include <QString>
#include <QVector>

class fontPool;
class pageSize;
class TeXFontDefinition;

class dvifile : public bigEndianByteReader
{
public:
    /** Makes a deep copy of the old DVI file. */
    dvifile(const dvifile *old, fontPool *fp);
    dvifile(const QString &fname, class fontPool *pool);

    ~dvifile();

    dvifile(const dvifile &) = delete;
    dvifile &operator=(const dvifile &) = delete;

    fontPool *font_pool;
    QString filename;
    QString generatorString;
    quint16 total_pages;
    QVector<quint32> page_offset;

    /** Saves the DVI file. Returns true on success. */
    bool saveAs(const QString &filename);

    // Returns a pointer to the DVI file's data, or 0 if no data has yet
    // been allocated.
    quint8 *dvi_Data()
    {
        return dviData.data();
    }

    qint64 size_of_file;
    QString errorMsg;

    /** This field is set to zero when the DVI file is constructed, and
        will be modified during the prescan phase (at this time the
        prescan code still resides in the dviRenderer class) */
    quint16 numberOfExternalPSFiles;

    /** This field is set to zero when the DVI file is constructed, and
        will be modified during the prescan phase (at this time the
        prescan code still resides in the dviRenderer class) */
    quint16 numberOfExternalNONPSFiles;

    quint32 beginning_of_postamble;

    /** This flag is set to "true" during the construction of the
        dvifile, and is never changed afterwards by the dvifile
        class. It is used in kdvi in conjunction with source-specials:
        the first time a page with source specials is rendered, KDVI
        shows an info dialog, and the flag is set to false. That way
        KDVI ensures that the user is only informed once. */
    bool sourceSpecialMarker;

    QHash<int, TeXFontDefinition *> tn_table;

    /** Returns the number of centimeters per DVI unit in this DVI
        file. */
    double getCmPerDVIunit() const
    {
        return cmPerDVIunit;
    }

    /** Returns the magnification of the DVI file, as described in the
        DVI Standard. */
    quint32 getMagnification() const
    {
        return _magnification;
    }

    /** This member is set to zero on construction and can be used by
        other software to count error messages that were printed when
        the DVI-file was processed. Suggested application: limit the
        number of error messages to, say, 25. */
    quint8 errorCounter;

    /** Papersize information read from the dvi-File */
    pageSize *suggestedPageSize;

    /** Sets new DVI data; all old data is erased. EXPERIMENTAL, use
        with care. */
    void setNewData(const QVector<quint8> &newData)
    {
        dviData = newData;
    }

    /** Page numbers that appear in a DVI document need not be
        ordered. Worse, page numbers need not be unique. This method
        renumbers the pages. */
    void renumber();

    /** PDF to PS file conversion

    This utility method takes the name of a PDF-file, and attempts to
    convert it to a PS file. The dvifile internally keeps a list of
    converted files, to do two things:

    - convert files only once.

    - delete all converted files on destruction

    @warning The internal buffer can lead to difficulties if filenames
      of PDF-files are not unique: if the content of a PDF file is
      changed and this method is called a second time with the same file
      name, the method will then NOT convert the file, but simply return
      the name from the buffer

    @returns The name of the PS file, or QString() on failure.
    */
    QString convertPDFtoPS(const QString &PDFFilename, QString *converrorms = nullptr);

private:
    void pdf2psNotFound(const QString &PDFFilename, QString *converrorms);

    /** process_preamble reads the information in the preamble and
        stores it into global variables for later use. */
    void process_preamble();
    void find_postamble();
    /** read_postamble reads the information in the postamble, storing
        it into global variables. It also takes care of reading in all
        of the pixel files for the fonts used in the job. */
    void read_postamble();
    void prepare_pages();

    /** Offset in DVI file of last page, set in read_postamble(). */
    quint32 last_page_offset;
    quint32 _magnification;

    double cmPerDVIunit;

    QVector<quint8> dviData;

    /** Map of filenames for converted PDF files

    This map contains names of PDF files that were converted to
    PostScript. The key is the name of the PDF file, the data the name
    of the associated PS file, or QString(), if the file could not
    be converted. The PS files are deleted when the DVI-file is
    destructed. */
    QMap<QString, QString> convertedFiles;

    /** Flag, used so that KDVI complains only once about a missing
        "PDF2PS" utility. Set to "false" in the constructor. */
    bool have_complainedAboutMissingPDF2PS;
};

#endif // ifndef _DVIFILE_H
