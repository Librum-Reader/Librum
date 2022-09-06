/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "epubdocument.h"
#include <QDir>
#include <QTemporaryFile>

#include <QRegularExpression>

Q_LOGGING_CATEGORY(OkularEpuDebug, "org.kde.okular.generators.epu", QtWarningMsg)
using namespace Epub;

EpubDocument::EpubDocument(const QString &fileName, const QFont &font)
    : QTextDocument()
    , padding(20)
    , mFont(font)
{
#ifdef Q_OS_WIN
    mEpub = epub_open(qUtf8Printable(fileName), 2);
#else
    mEpub = epub_open(qPrintable(fileName), 2);
#endif

    setPageSize(QSizeF(600, 800));
}

bool EpubDocument::isValid()
{
    return (mEpub ? true : false);
}

EpubDocument::~EpubDocument()
{
    if (mEpub) {
        epub_close(mEpub);
    }

    epub_cleanup();
}

struct epub *EpubDocument::getEpub()
{
    return mEpub;
}

void EpubDocument::setCurrentSubDocument(const QString &doc)
{
    mCurrentSubDocument.clear();
    int index = doc.indexOf(QLatin1Char('/'));
    if (index > 0) {
        mCurrentSubDocument = QUrl::fromLocalFile(doc.left(index + 1));
    }
}

int EpubDocument::maxContentHeight() const
{
    return pageSize().height() - (2 * padding);
}

int EpubDocument::maxContentWidth() const
{
    return pageSize().width() - (2 * padding);
}

QString EpubDocument::checkCSS(const QString &c)
{
    QString css = c;
    // remove paragraph line-heights
    css.remove(QRegularExpression(QStringLiteral("line-height\\s*:\\s*[\\w\\.]*;")));

    // HACK transform em and rem notation to px, because QTextDocument doesn't support
    // em and rem.
    const QStringList cssArray = css.split(QRegularExpression(QStringLiteral("\\s+")));
    QStringList cssArrayReplaced;
    std::size_t cssArrayCount = cssArray.count();
    std::size_t i = 0;
    const QRegularExpression re(QStringLiteral("(([0-9]+)(\\.[0-9]+)?)r?em(.*)"));
    while (i < cssArrayCount) {
        const auto &item = cssArray[i];
        QRegularExpressionMatch match = re.match(item);
        if (match.hasMatch()) {
            double em = match.captured(1).toDouble();
            double px = em * mFont.pointSize();
            cssArrayReplaced.append(QStringLiteral("%1px%2").arg(px).arg(match.captured(4)));
        } else {
            cssArrayReplaced.append(item);
        }
        i++;
    }
    return cssArrayReplaced.join(QStringLiteral(" "));
}

QVariant EpubDocument::loadResource(int type, const QUrl &name)
{
    int size;
    char *data;

    QString fileInPath = mCurrentSubDocument.resolved(name).path();

    // Get the data from the epub file
    size = epub_get_data(mEpub, fileInPath.toUtf8().constData(), &data);

    QVariant resource;

    if (data) {
        switch (type) {
        case QTextDocument::ImageResource: {
            QImage img = QImage::fromData((unsigned char *)data, size);
            const int maxHeight = maxContentHeight();
            const int maxWidth = maxContentWidth();
            if (img.height() > maxHeight) {
                img = img.scaledToHeight(maxHeight, Qt::SmoothTransformation);
            }
            if (img.width() > maxWidth) {
                img = img.scaledToWidth(maxWidth, Qt::SmoothTransformation);
            }
            resource.setValue(img);
            break;
        }
        case QTextDocument::StyleSheetResource: {
            QString css = QString::fromUtf8(data);
            resource.setValue(checkCSS(css));
            break;
        }
        case EpubDocument::MovieResource: {
            QTemporaryFile *tmp = new QTemporaryFile(QStringLiteral("%1/okrXXXXXX").arg(QDir::tempPath()), this);
            if (!tmp->open()) {
                qCWarning(OkularEpuDebug) << "EPUB : error creating temporary video file";
            }
            if (tmp->write(data, size) == -1) {
                qCWarning(OkularEpuDebug) << "EPUB : error writing data" << tmp->errorString();
            }
            tmp->flush();
            resource.setValue(tmp->fileName());
            break;
        }
        case EpubDocument::AudioResource: {
            QByteArray ba(data, size);
            resource.setValue(ba);
            break;
        }
        default:
            resource.setValue(QString::fromUtf8(data));
            break;
        }

        free(data);
    }

    // add to cache
    addResource(type, name, resource);

    return resource;
}
