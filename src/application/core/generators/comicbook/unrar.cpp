/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "unrar.h"
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QGlobalStatic>
#include <QLoggingCategory>
#include <QTemporaryDir>
#if defined(WITH_KPTY)
    #include <KPty/kptydevice.h>
    #include <KPty/kptyprocess.h>
#endif

#include <QRegularExpression>
#include <QStandardPaths>
#include <memory>
#include "debug_comicbook.h"

struct UnrarHelper
{
    UnrarHelper();
    ~UnrarHelper();

    UnrarHelper(const UnrarHelper&) = delete;
    UnrarHelper& operator=(const UnrarHelper&) = delete;

    UnrarFlavour* kind;
    QString unrarPath;
    QString lsarPath;
};

Q_GLOBAL_STATIC(UnrarHelper, helper)

static UnrarFlavour* detectUnrar(const QString& unrarPath,
                                 const QString& versionCommand)
{
    UnrarFlavour* kind = nullptr;
    QProcess proc;
    proc.start(unrarPath, QStringList() << versionCommand);
    bool ok = proc.waitForFinished(-1);
    Q_UNUSED(ok)
    const QRegularExpression regex(QStringLiteral("[\r\n]"));
    const QStringList lines =
        QString::fromLocal8Bit(proc.readAllStandardOutput())
            .split(regex, QString::SkipEmptyParts);
    if(!lines.isEmpty())
    {
        if(lines.first().startsWith(QLatin1String("UNRAR ")))
        {
            kind = new NonFreeUnrarFlavour();
        }
        else if(lines.first().startsWith(QLatin1String("RAR ")))
        {
            kind = new NonFreeUnrarFlavour();
        }
        else if(lines.first().startsWith(QLatin1String("unrar ")))
        {
            kind = new FreeUnrarFlavour();
        }
        else if(lines.first().startsWith(QLatin1String("v")))
        {
            kind = new UnarFlavour();
        }
    }
    return kind;
}

UnrarHelper::UnrarHelper() :
    kind(nullptr)
{
    QString path = QStandardPaths::findExecutable(QStringLiteral("lsar"));

    if(!path.isEmpty())
    {
        lsarPath = path;
    }

    path = QStandardPaths::findExecutable(QStringLiteral("unrar-nonfree"));

    if(path.isEmpty())
    {
        path = QStandardPaths::findExecutable(QStringLiteral("unrar"));
    }
    if(path.isEmpty())
    {
        path = QStandardPaths::findExecutable(QStringLiteral("rar"));
    }
    if(path.isEmpty())
    {
        path = QStandardPaths::findExecutable(QStringLiteral("unar"));
    }

    if(!path.isEmpty())
    {
        kind = detectUnrar(path, QStringLiteral("--version"));
    }

    if(!kind)
    {
        kind = detectUnrar(path, QStringLiteral("-v"));
    }

    if(!kind)
    {
        // no luck, print that
        qWarning() << "Neither unrar nor unarchiver were found.";
    }
    else
    {
        unrarPath = path;
        qCDebug(OkularComicbookDebug)
            << "detected:" << path << "(" << kind->name() << ")";
    }
}

UnrarHelper::~UnrarHelper()
{
    delete kind;
}

Unrar::Unrar() :
    QObject(nullptr),
    mLoop(nullptr),
    mTempDir(nullptr)
{
}

Unrar::~Unrar()
{
    delete mTempDir;
}

bool Unrar::open(const QString& fileName)
{
    if(!isSuitableVersionAvailable())
    {
        return false;
    }

    delete mTempDir;
    mTempDir = new QTemporaryDir();

    mFileName = fileName;

    /**
     * Extract the archive to a temporary directory
     */
    mStdOutData.clear();
    mStdErrData.clear();

    const int ret = startSyncProcess(
        helper->kind->processOpenArchiveArgs(mFileName, mTempDir->path()));
    bool ok = ret == 0;

    return ok;
}

QStringList Unrar::list()
{
    mStdOutData.clear();
    mStdErrData.clear();

    if(!isSuitableVersionAvailable())
    {
        return QStringList();
    }

    startSyncProcess(helper->kind->processListArgs(mFileName));

    const QRegularExpression regex(QStringLiteral("[\r\n]"));
    QStringList listFiles = helper->kind->processListing(
        QString::fromLocal8Bit(mStdOutData)
            .split(regex, QString::SkipEmptyParts));

    QString subDir;

    if(listFiles.last().endsWith(QLatin1Char('/')) &&
       helper->kind->name() == QLatin1String("unar"))
    {
        // Subfolder detected. The unarchiver is unable to extract all files
        // into a single folder
        subDir = listFiles.last();
        listFiles.removeLast();
    }

    QStringList newList;
    for(const QString& f : qAsConst(listFiles))
    {
        // Extract all the files to mTempDir regardless of their path inside the
        // archive This will break if ever an arvhice with two files with the
        // same name in different subfolders
        QFileInfo fi(f);
        if(QFile::exists(mTempDir->path() + QLatin1Char('/') + subDir +
                         fi.fileName()))
        {
            newList.append(subDir + fi.fileName());
        }
    }
    return newList;
}

QByteArray Unrar::contentOf(const QString& fileName) const
{
    if(!isSuitableVersionAvailable())
    {
        return QByteArray();
    }

    QFile file(mTempDir->path() + QLatin1Char('/') + fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return QByteArray();
    }

    return file.readAll();
}

QIODevice* Unrar::createDevice(const QString& fileName) const
{
    if(!isSuitableVersionAvailable())
    {
        return nullptr;
    }

    std::unique_ptr<QFile> file(
        new QFile(mTempDir->path() + QLatin1Char('/') + fileName));
    if(!file->open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    return file.release();
}

bool Unrar::isAvailable()
{
    return helper->kind;
}

bool Unrar::isSuitableVersionAvailable()
{
    if(!isAvailable())
    {
        return false;
    }

    if(dynamic_cast<NonFreeUnrarFlavour*>(helper->kind) ||
       dynamic_cast<UnarFlavour*>(helper->kind))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Unrar::readFromStdout()
{
    if(!mProcess)
    {
        return;
    }

    mStdOutData += mProcess->readAllStandardOutput();
}

void Unrar::readFromStderr()
{
    if(!mProcess)
    {
        return;
    }

    mStdErrData += mProcess->readAllStandardError();
    if(!mStdErrData.isEmpty())
    {
        mProcess->kill();
        return;
    }
}

void Unrar::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    if(mLoop)
    {
        mLoop->exit(exitStatus == QProcess::CrashExit ? 1 : 0);
    }
}

int Unrar::startSyncProcess(const ProcessArgs& args)
{
    int ret = 0;

#if !defined(WITH_KPTY)
    mProcess = new QProcess(this);
    connect(mProcess, &QProcess::readyReadStandardOutput, this,
            &Unrar::readFromStdout);
    connect(mProcess, &QProcess::readyReadStandardError, this,
            &Unrar::readFromStderr);
    connect(mProcess,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(
                &QProcess::finished),
            this, &Unrar::finished);

#else
    mProcess = new KPtyProcess(this);
    mProcess->setOutputChannelMode(KProcess::SeparateChannels);
    connect(mProcess, &KPtyProcess::readyReadStandardOutput, this,
            &Unrar::readFromStdout);
    connect(mProcess, &KPtyProcess::readyReadStandardError, this,
            &Unrar::readFromStderr);
    connect(mProcess,
            static_cast<void (KPtyProcess::*)(int, QProcess::ExitStatus)>(
                &KPtyProcess::finished),
            this, &Unrar::finished);

#endif

#if !defined(WITH_KPTY)
    if(helper->kind->name() == QLatin1String("unar") && args.useLsar)
    {
        mProcess->start(helper->lsarPath, args.appArgs,
                        QIODevice::ReadWrite | QIODevice::Unbuffered);
    }
    else
    {
        mProcess->start(helper->unrarPath, args.appArgs,
                        QIODevice::ReadWrite | QIODevice::Unbuffered);
    }

    ret = mProcess->waitForFinished(-1) ? 0 : 1;
#else
    if(helper->kind->name() == QLatin1String("unar") && args.useLsar)
    {
        mProcess->setProgram(helper->lsarPath, args.appArgs);
    }
    else
    {
        mProcess->setProgram(helper->unrarPath, args.appArgs);
    }

    mProcess->setNextOpenMode(QIODevice::ReadWrite | QIODevice::Unbuffered);
    mProcess->start();
    QEventLoop loop;
    mLoop = &loop;
    ret = loop.exec(QEventLoop::WaitForMoreEvents |
                    QEventLoop::ExcludeUserInputEvents);
    mLoop = nullptr;
#endif

    delete mProcess;
    mProcess = nullptr;

    return ret;
}

void Unrar::writeToProcess(const QByteArray& data)
{
    if(!mProcess || data.isNull())
    {
        return;
    }

#if !defined(WITH_KPTY)
    mProcess->write(data);
#else
    mProcess->pty()->write(data);
#endif
}
