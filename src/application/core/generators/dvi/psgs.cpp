// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
//
// ghostscript_interface
//
// Part of KDVI - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2004 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include "psgs.h"
#include <config.h>
#include <KLocalizedString>
#include <KProcess>
#include <QDir>
#include <QLoggingCategory>
#include <QPainter>
#include <QPixmap>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include "debug_dvi.h"
#include "dviFile.h"
#include "pageNumber.h"
#include "psheader.cpp"

//#define DEBUG_PSGS

// extern char psheader[];

pageInfo::pageInfo(const QString& _PostScriptString)
{
    PostScriptString = new QString(_PostScriptString);
    background = Qt::white;
    permanentBackground = Qt::white;
}

pageInfo::~pageInfo()
{
    if(PostScriptString != nullptr)
    {
        delete PostScriptString;
    }
}

// ======================================================

ghostscript_interface::ghostscript_interface()
{
    PostScriptHeaderString = new QString();

    knownDevices.append(QStringLiteral("png16m"));
    knownDevices.append(QStringLiteral("jpeg"));
    knownDevices.append(QStringLiteral("pnn"));
    knownDevices.append(QStringLiteral("pnnraw"));
    gsDevice = knownDevices.begin();
}

ghostscript_interface::~ghostscript_interface()
{
    if(PostScriptHeaderString != nullptr)
    {
        delete PostScriptHeaderString;
    }
    qDeleteAll(pageList);
}

void ghostscript_interface::setPostScript(const quint16 page,
                                          const QString& PostScript)
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug)
        << "ghostscript_interface::setPostScript( " << page << ", ... )";
#endif

    if(pageList.value(page) == nullptr)
    {
        pageInfo* info = new pageInfo(PostScript);
        // Check if dict is big enough
        if(pageList.count() > pageList.capacity() - 2)
        {
            pageList.reserve(pageList.capacity() * 2);
        }
        pageList.insert(page, info);
    }
    else
    {
        *(pageList.value(page)->PostScriptString) = PostScript;
    }
}

void ghostscript_interface::setIncludePath(const QString& _includePath)
{
    if(_includePath.isEmpty())
    {
        includePath = QLatin1Char('*');  // Allow all files
    }
    else
    {
        includePath = _includePath + QStringLiteral("/*");
    }
}

void ghostscript_interface::setBackgroundColor(const quint16 page,
                                               const QColor& background_color,
                                               bool permanent)
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug) << "ghostscript_interface::setBackgroundColor( "
                            << page << ", " << background_color << " )";
#endif

    if(pageList.value(page) == nullptr)
    {
        pageInfo* info = new pageInfo(QString());
        info->background = background_color;
        if(permanent)
        {
            info->permanentBackground = background_color;
        }
        // Check if dict is big enough
        if(pageList.count() > pageList.capacity() - 2)
        {
            pageList.reserve(pageList.capacity() * 2);
        }
        pageList.insert(page, info);
    }
    else
    {
        pageList.value(page)->background = background_color;
        if(permanent)
        {
            pageList.value(page)->permanentBackground = background_color;
        }
    }
}

void ghostscript_interface::restoreBackgroundColor(const quint16 page)
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug)
        << "ghostscript_interface::restoreBackgroundColor( " << page << " )";
#endif
    if(pageList.value(page) == nullptr)
    {
        return;
    }

    pageInfo* info = pageList.value(page);
    info->background = info->permanentBackground;
}

// Returns the background color for a certain page. This color is
// always guaranteed to be valid

QColor ghostscript_interface::getBackgroundColor(const quint16 page) const
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug)
        << "ghostscript_interface::getBackgroundColor( " << page << " )";
#endif

    if(pageList.value(page) == nullptr)
    {
        return Qt::white;
    }
    else
    {
        return pageList.value(page)->background;
    }
}

void ghostscript_interface::clear()
{
    PostScriptHeaderString->truncate(0);

    // Deletes all items, removes temporary files, etc.
    qDeleteAll(pageList);
    pageList.clear();
}

void ghostscript_interface::gs_generate_graphics_file(const quint16 page,
                                                      const QString& filename,
                                                      long magnification)
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug)
        << "ghostscript_interface::gs_generate_graphics_file( " << page << ", "
        << filename << " )";
#endif

    if(knownDevices.isEmpty())
    {
        qCCritical(OkularDviDebug) << "No known devices found";
        return;
    }

    // Make sure gs is in PATH and not just in the CWD
    static const QString gsFullPath =
        QStandardPaths::findExecutable(QStringLiteral("gs"));
    if(gsFullPath.isEmpty())
    {
        qCCritical(OkularDviDebug) << "gs is not in path";
        return;
    }

    pageInfo* info = pageList.value(page);

    // Generate a PNG-file
    // Step 1: Write the PostScriptString to a File
    QTemporaryFile PSfile(QDir::tempPath() +
                          QLatin1String("/okular_XXXXXX.ps"));
    PSfile.setAutoRemove(false);
    PSfile.open();
    const QString PSfileName = PSfile.fileName();

    QTextStream os(&PSfile);
    os << "%!PS-Adobe-2.0\n"
       << "%%Creator: kdvi\n"
       << "%%Title: KDVI temporary PostScript\n"
       << "%%Pages: 1\n"
       << "%%PageOrder: Ascend\n"
       // HSize and VSize in 1/72 inch
       << "%%BoundingBox: 0 0 " << (qint32)(72 * (pixel_page_w / resolution))
       << ' ' << (qint32)(72 * (pixel_page_h / resolution)) << '\n'
       << "%%EndComments\n"
       << "%!\n"
       << psheader
       << "TeXDict begin "
       // HSize in (1/(65781.76*72))inch
       << (qint32)(72 * 65781 * (pixel_page_w / resolution))
       << ' '
       // VSize in (1/(65781.76*72))inch
       << (qint32)(72 * 65781 * (pixel_page_h / resolution))
       << ' '
       // Magnification
       << (qint32)(magnification)
       // dpi and vdpi
       << " 300 300"
       // Name
       << " (test.dvi)"
       << " @start end\n"
       << "TeXDict begin\n"
       // Start page
       << "1 0 bop 0 0 a \n";

    if(!PostScriptHeaderString->toLatin1().isNull())
    {
        os << PostScriptHeaderString->toLatin1();
    }

    if(info->background != Qt::white)
    {
        QString colorCommand =
            QStringLiteral(
                "gsave %1 %2 %3 setrgbcolor clippath fill grestore\n")
                .arg(info->background.red() / 255.0)
                .arg(info->background.green() / 255.0)
                .arg(info->background.blue() / 255.0);
        os << colorCommand.toLatin1();
    }

    if(!info->PostScriptString->isNull())
    {
        os << *(info->PostScriptString);
    }

    os << "end\n"
       << "showpage \n";

    PSfile.close();

    // Step 2: Call GS with the File
    QFile::remove(filename);
    KProcess proc;
    proc.setOutputChannelMode(KProcess::SeparateChannels);
    QStringList argus;
    argus << gsFullPath;
    argus << QStringLiteral("-dSAFER") << QStringLiteral("-dPARANOIDSAFER")
          << QStringLiteral("-dDELAYSAFER") << QStringLiteral("-dNOPAUSE")
          << QStringLiteral("-dBATCH");
    argus << QStringLiteral("-sDEVICE=%1").arg(*gsDevice);
    argus << QStringLiteral("-sOutputFile=%1").arg(filename);
    argus << QStringLiteral("-sExtraIncludePath=%1").arg(includePath);
    argus << QStringLiteral("-g%1x%2")
                 .arg(pixel_page_w)
                 .arg(pixel_page_h);  // page size in pixels
    argus << QStringLiteral("-r%1").arg(resolution);  // resolution in dpi
    argus << QStringLiteral(
        "-dTextAlphaBits=4 -dGraphicsAlphaBits=2");  // Antialiasing
    argus
        << QStringLiteral("-c")
        << QStringLiteral(
               "<< /PermitFileReading [ ExtraIncludePath ] /PermitFileWriting "
               "[] /PermitFileControl [] >> setuserparams .locksafe");
    argus << QStringLiteral("-f") << PSfileName;

#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug) << argus.join(" ");
#endif

    proc << argus;
    int res = proc.execute();

    if(res)
    {
        // Starting ghostscript did not work.
        // TODO: Issue error message, switch PS support off.
        qCCritical(OkularDviDebug) << "ghostview could not be started";
    }

    PSfile.remove();

    // Check if gs has indeed produced a file.
    if(QFile::exists(filename) == false)
    {
        qCCritical(OkularDviDebug) << "GS did not produce output.";

        // No. Check is the reason is that the device is not compiled into
        // ghostscript. If so, try again with another device.
        QString GSoutput;
        proc.setReadChannel(QProcess::StandardOutput);
        while(proc.canReadLine())
        {
            GSoutput = QString::fromLocal8Bit(proc.readLine());
            if(GSoutput.contains(QStringLiteral("Unknown device")))
            {
                qCDebug(OkularDviDebug)
                    << QString::fromLatin1("The version of ghostview installed "
                                           "on this computer does not support "
                                           "the '%1' ghostview device driver.")
                           .arg(*gsDevice);
                knownDevices.erase(gsDevice);
                gsDevice = knownDevices.begin();
                if(knownDevices.isEmpty())
                {
                    // TODO: show a requestor of some sort.
                    Q_EMIT error(
                        i18n(
                            "The version of Ghostview that is installed on "
                            "this computer does not contain "
                            "any of the Ghostview device drivers that are "
                            "known to Okular. PostScript "
                            "support has therefore been turned off in Okular."),
                        -1);
                }
                else
                {
                    qCDebug(OkularDviDebug)
                        << QStringLiteral("Okular will now try to use the '%1' "
                                          "device driver.")
                               .arg(*gsDevice);
                    gs_generate_graphics_file(page, filename, magnification);
                }
                return;
            }
        }
    }
}

void ghostscript_interface::graphics(const quint16 page, double dpi,
                                     long magnification, QPainter* paint)
{
#ifdef DEBUG_PSGS
    qCDebug(OkularDviDebug) << "ghostscript_interface::graphics( " << page
                            << ", " << dpi << ", ... ) called.";
#endif

    if(paint == nullptr)
    {
        qCCritical(OkularDviDebug)
            << "ghostscript_interface::graphics(PageNumber page, double dpi, "
               "long magnification, QPainter *paint) called with paint == 0";
        return;
    }

    resolution = dpi;

    pixel_page_w = paint->viewport().width();
    pixel_page_h = paint->viewport().height();

    pageInfo* info = pageList.value(page);

    // No PostScript? Then return immediately.
    if((info == nullptr) || (info->PostScriptString->isEmpty()))
    {
#ifdef DEBUG_PSGS
        qCDebug(OkularDviDebug) << "No PostScript found. Not drawing anything.";
#endif
        return;
    }

    QTemporaryFile gfxFile;
    gfxFile.open();
    const QString gfxFileName = gfxFile.fileName();
    // We are want the filename, not the file.
    gfxFile.close();

    gs_generate_graphics_file(page, gfxFileName, magnification);

    QImage MemoryCopy(gfxFileName);
    paint->drawImage(0, 0, MemoryCopy);
    return;
}

QString ghostscript_interface::locateEPSfile(const QString& filename,
                                             const QUrl& base)
{
    // If the base URL indicates that the DVI file is local, try to find
    // the graphics file in the directory where the DVI file resides
    if(base.isLocalFile())
    {
        QString path = base.path();  // -> "/bar/foo.dvi"
        QFileInfo fi1(path);
        QFileInfo fi2(fi1.dir(), filename);
        if(fi2.exists())
        {
            return fi2.absoluteFilePath();
        }
    }

    // Otherwise, use kpsewhich to find the eps file.
    // Make sure kpsewhich is in PATH and not just in the CWD
    static const QString fullPath =
        QStandardPaths::findExecutable(QStringLiteral("kpsewhich"));
    if(fullPath.isEmpty())
    {
        return {};
    }

    KProcess proc;
    proc << fullPath << filename;
    proc.execute();
    return QString::fromLocal8Bit(proc.readLine().trimmed());
}
