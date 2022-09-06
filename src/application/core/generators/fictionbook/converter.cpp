/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "converter.h"

#include <QAbstractTextDocumentLayout>
#include <QDate>
#include <QDomElement>
#include <QDomText>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextTable>
#include <QUrl>

#include <KLocalizedString>

#include <core/action.h>
#include <core/document.h>

#include "document.h"

using namespace FictionBook;

class Converter::TitleInfo
{
public:
    QStringList mGenres;
    QString mAuthor;
    QString mTitle;
    QString mAnnotation;
    QString mKeywords;
    QDate mDate;
    QDomElement mCoverPage;
    QString mLanguage;
};

class Converter::DocumentInfo
{
public:
    QString mAuthor;
    QString mProducer;
    QDate mDate;
    QString mId;
    QString mVersion;
};

Converter::Converter()
    : mTextDocument(nullptr)
    , mCursor(nullptr)
    , mTitleInfo(nullptr)
    , mDocumentInfo(nullptr)
{
}

Converter::~Converter()
{
    delete mTitleInfo;
    delete mDocumentInfo;
}

QTextDocument *Converter::convert(const QString &fileName)
{
    Document fbDocument(fileName);
    if (!fbDocument.open()) {
        Q_EMIT error(fbDocument.lastErrorString(), -1);
        return nullptr;
    }

    mTextDocument = new QTextDocument;
    mCursor = new QTextCursor(mTextDocument);
    mSectionCounter = 0;
    mLocalLinks.clear();
    mSectionMap.clear();

    const QDomDocument document = fbDocument.content();

    /**
     * Set the correct page size
     */
    mTextDocument->setPageSize(QSizeF(600, 800));

    QTextFrameFormat frameFormat;
    frameFormat.setMargin(20);

    QTextFrame *rootFrame = mTextDocument->rootFrame();
    rootFrame->setFrameFormat(frameFormat);

    /**
     * Parse the content of the document
     */
    const QDomElement documentElement = document.documentElement();

    if (documentElement.tagName() != QLatin1String("FictionBook")) {
        Q_EMIT error(i18n("Document is not a valid FictionBook"), -1);
        delete mCursor;
        return nullptr;
    }

    /**
     * First we read all images, so we can calculate the size later.
     */
    QDomElement element = documentElement.firstChildElement();
    while (!element.isNull()) {
        if (element.tagName() == QLatin1String("binary")) {
            if (!convertBinary(element)) {
                delete mCursor;
                return nullptr;
            }
        }

        element = element.nextSiblingElement();
    }

    /**
     * Read the rest: description (could be only one) and bodies (one or more)
     */
    element = documentElement.firstChildElement();
    while (!element.isNull()) {
        if (element.tagName() == QLatin1String("description")) {
            if (!convertDescription(element)) {
                delete mCursor;
                return nullptr;
            }

            if (mTitleInfo && !mTitleInfo->mCoverPage.isNull()) {
                convertCover(mTitleInfo->mCoverPage);
                mCursor->insertBlock();
            }

            QTextFrame *topFrame = mCursor->currentFrame();

            QTextFrameFormat frameFormat;
            frameFormat.setBorder(2);
            frameFormat.setPadding(8);
            frameFormat.setBackground(Qt::lightGray);

            if (mTitleInfo && !mTitleInfo->mTitle.isEmpty()) {
                mCursor->insertFrame(frameFormat);

                QTextCharFormat charFormat;
                charFormat.setFontPointSize(22);
                charFormat.setFontWeight(QFont::Bold);
                mCursor->insertText(mTitleInfo->mTitle, charFormat);

                mCursor->setPosition(topFrame->lastPosition());
            }

            if (mTitleInfo && !mTitleInfo->mAuthor.isEmpty()) {
                frameFormat.setBorder(1);
                mCursor->insertFrame(frameFormat);

                QTextCharFormat charFormat;
                charFormat.setFontPointSize(10);
                mCursor->insertText(mTitleInfo->mAuthor, charFormat);

                mCursor->setPosition(topFrame->lastPosition());
                mCursor->insertBlock();
            }

            if (mTitleInfo && !mTitleInfo->mAnnotation.isEmpty()) {
                frameFormat.setBorder(0);
                mCursor->insertFrame(frameFormat);

                QTextCharFormat charFormat;
                charFormat.setFontPointSize(10);
                charFormat.setFontItalic(true);
                mCursor->insertText(mTitleInfo->mAnnotation, charFormat);

                mCursor->setPosition(topFrame->lastPosition());
                mCursor->insertBlock();
            }
        } else if (element.tagName() == QLatin1String("body")) {
            mCursor->insertBlock();

            if (!convertBody(element)) {
                delete mCursor;
                return nullptr;
            }
        }

        element = element.nextSiblingElement();
    }

    /**
     * Add document info.
     */
    if (mTitleInfo) {
        if (!mTitleInfo->mTitle.isEmpty()) {
            Q_EMIT addMetaData(Okular::DocumentInfo::Title, mTitleInfo->mTitle);
        }

        if (!mTitleInfo->mAuthor.isEmpty()) {
            Q_EMIT addMetaData(Okular::DocumentInfo::Author, mTitleInfo->mAuthor);
        }

        if (!mTitleInfo->mKeywords.isEmpty()) {
            Q_EMIT addMetaData(Okular::DocumentInfo::Keywords, mTitleInfo->mKeywords);
        }
    }

    if (mDocumentInfo) {
        if (!mDocumentInfo->mProducer.isEmpty()) {
            Q_EMIT addMetaData(Okular::DocumentInfo::Producer, mDocumentInfo->mProducer);
        }

        if (mDocumentInfo->mDate.isValid()) {
            Q_EMIT addMetaData(Okular::DocumentInfo::CreationDate, QLocale().toString(mDocumentInfo->mDate, QLocale::ShortFormat));
        }
    }

    QMapIterator<QString, QPair<int, int>> it(mLocalLinks);
    while (it.hasNext()) {
        it.next();

        const QTextBlock block = mSectionMap[it.key()];
        if (!block.isValid()) { // local link without existing target
            continue;
        }

        Okular::DocumentViewport viewport = calculateViewport(mTextDocument, block);

        Okular::GotoAction *action = new Okular::GotoAction(QString(), viewport);

        Q_EMIT addAction(action, it.value().first, it.value().second);
    }

    delete mCursor;

    return mTextDocument;
}

bool Converter::convertBody(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("section")) {
            mCursor->insertBlock();
            if (!convertSection(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("image")) {
            if (!convertImage(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("title")) {
            if (!convertTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("epigraph")) {
            if (!convertEpigraph(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertDescription(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("title-info")) {
            if (!convertTitleInfo(child)) {
                return false;
            }
        }
        if (child.tagName() == QLatin1String("document-info")) {
            if (!convertDocumentInfo(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertTitleInfo(const QDomElement &element)
{
    delete mTitleInfo;
    mTitleInfo = new TitleInfo;

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("genre")) {
            QString genre;
            if (!convertTextNode(child, genre)) {
                return false;
            }

            if (!genre.isEmpty()) {
                mTitleInfo->mGenres.append(genre);
            }
        } else if (child.tagName() == QLatin1String("author")) {
            QString firstName, middleName, lastName, dummy;

            if (!convertAuthor(child, firstName, middleName, lastName, dummy, dummy)) {
                return false;
            }

            if (mTitleInfo->mAuthor.isEmpty()) {
                mTitleInfo->mAuthor = QStringLiteral("%1 %2 %3").arg(firstName, middleName, lastName).simplified();
            } else {
                mTitleInfo->mAuthor += QStringLiteral(", %1 %2 %3").arg(firstName, middleName, lastName).simplified();
            }
        } else if (child.tagName() == QLatin1String("book-title")) {
            if (!convertTextNode(child, mTitleInfo->mTitle)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("keywords")) {
            QString keywords;
            if (!convertTextNode(child, keywords)) {
                return false;
            }

            mTitleInfo->mKeywords = keywords;
        } else if (child.tagName() == QLatin1String("annotation")) {
            if (!convertAnnotation(child, mTitleInfo->mAnnotation)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("date")) {
            if (!convertDate(child, mTitleInfo->mDate)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("coverpage")) {
            mTitleInfo->mCoverPage = child;
        } else if (child.tagName() == QLatin1String("lang")) {
            if (!convertTextNode(child, mTitleInfo->mLanguage)) {
                return false;
            }
        }
        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertDocumentInfo(const QDomElement &element)
{
    delete mDocumentInfo;
    mDocumentInfo = new DocumentInfo;

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("author")) {
            QString firstName, middleName, lastName, email, nickname;

            if (!convertAuthor(child, firstName, middleName, lastName, email, nickname)) {
                return false;
            }

            mDocumentInfo->mAuthor = QStringLiteral("%1 %2 %3 <%4> (%5)").arg(firstName, middleName, lastName, email, nickname);
        } else if (child.tagName() == QLatin1String("program-used")) {
            if (!convertTextNode(child, mDocumentInfo->mProducer)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("date")) {
            if (!convertDate(child, mDocumentInfo->mDate)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("id")) {
            if (!convertTextNode(child, mDocumentInfo->mId)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("version")) {
            if (!convertTextNode(child, mDocumentInfo->mVersion)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}
bool Converter::convertAuthor(const QDomElement &element, QString &firstName, QString &middleName, QString &lastName, QString &email, QString &nickname)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("first-name")) {
            if (!convertTextNode(child, firstName)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("middle-name")) {
            if (!convertTextNode(child, middleName)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("last-name")) {
            if (!convertTextNode(child, lastName)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("email")) {
            if (!convertTextNode(child, email)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("nickname")) {
            if (!convertTextNode(child, nickname)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertTextNode(const QDomElement &element, QString &data)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        QDomText text = child.toText();
        if (!text.isNull()) {
            data = text.data();
        }

        child = child.nextSibling();
    }

    return true;
}

bool Converter::convertDate(const QDomElement &element, QDate &date)
{
    if (element.hasAttribute(QStringLiteral("value"))) {
        date = QDate::fromString(element.attribute(QStringLiteral("value")), Qt::ISODate);
    }

    return true;
}

bool Converter::convertAnnotation(const QDomElement &element, QString &data)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        QString text = child.text();
        if (!text.isNull()) {
            data = child.text();
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertSection(const QDomElement &element)
{
    if (element.hasAttribute(QStringLiteral("id"))) {
        mSectionMap.insert(element.attribute(QStringLiteral("id")), mCursor->block());
    }

    mSectionCounter++;

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("title")) {
            if (!convertTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("epigraph")) {
            if (!convertEpigraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("image")) {
            if (!convertImage(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("section")) {
            if (!convertSection(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("p")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("poem")) {
            if (!convertPoem(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("subtitle")) {
            if (!convertSubTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("cite")) {
            if (!convertCite(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("empty-line")) {
            if (!convertEmptyLine(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("code")) {
            if (!convertCode(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("table")) {
            if (!convertTable(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    mSectionCounter--;

    return true;
}

bool Converter::convertTitle(const QDomElement &element)
{
    QTextFrame *topFrame = mCursor->currentFrame();

    QTextFrameFormat frameFormat;
    frameFormat.setBorder(1);
    frameFormat.setPadding(8);
    frameFormat.setBackground(Qt::lightGray);

    mCursor->insertFrame(frameFormat);

    QDomElement child = element.firstChildElement();

    bool firstParagraph = true;
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("p")) {
            if (firstParagraph) {
                firstParagraph = false;
            } else {
                mCursor->insertBlock();
            }

            QTextCharFormat origFormat = mCursor->charFormat();

            QTextCharFormat titleFormat(origFormat);
            titleFormat.setFontPointSize(22 - (mSectionCounter * 2));
            titleFormat.setFontWeight(QFont::Bold);
            mCursor->setCharFormat(titleFormat);

            if (!convertParagraph(child)) {
                return false;
            }

            mCursor->setCharFormat(origFormat);

            Q_EMIT addTitle(mSectionCounter, child.text(), mCursor->block());

        } else if (child.tagName() == QLatin1String("empty-line")) {
            if (!convertEmptyLine(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    mCursor->setPosition(topFrame->lastPosition());

    return true;
}

bool Converter::convertParagraph(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("emphasis")) {
                if (!convertEmphasis(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("strong")) {
                if (!convertStrong(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("style")) {
                if (!convertStyle(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("a")) {
                if (!convertLink(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("image")) {
                if (!convertImage(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("strikethrough")) {
                if (!convertStrikethrough(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("code")) {
                if (!convertCode(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("sup")) {
                if (!convertSuperScript(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("sub")) {
                if (!convertSubScript(childElement)) {
                    return false;
                }
            }
        } else if (child.isText()) {
            const QDomText childText = child.toText();
            mCursor->insertText(childText.data().simplified());
        }

        child = child.nextSibling();
    }

    return true;
}

bool Converter::convertEmphasis(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat italicFormat(origFormat);
    italicFormat.setFontItalic(true);
    mCursor->setCharFormat(italicFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertStrikethrough(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat strikeoutFormat(origFormat);
    strikeoutFormat.setFontStrikeOut(true);
    mCursor->setCharFormat(strikeoutFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertStrong(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat boldFormat(origFormat);
    boldFormat.setFontWeight(QFont::Bold);
    mCursor->setCharFormat(boldFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertStyle(const QDomElement &element)
{
    if (!convertParagraph(element)) {
        return false;
    }

    return true;
}

bool Converter::convertBinary(const QDomElement &element)
{
    const QString id = element.attribute(QStringLiteral("id"));

    const QDomText textNode = element.firstChild().toText();
    QByteArray data = textNode.data().toLatin1();
    data = QByteArray::fromBase64(data);

    mTextDocument->addResource(QTextDocument::ImageResource, QUrl(id), QImage::fromData(data));

    return true;
}

bool Converter::convertCover(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("image")) {
            if (!convertImage(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertImage(const QDomElement &element)
{
    QString href = element.attributeNS(QStringLiteral("http://www.w3.org/1999/xlink"), QStringLiteral("href"));

    if (href.startsWith(QLatin1Char('#'))) {
        href = href.mid(1);
    }

    const QImage img = qvariant_cast<QImage>(mTextDocument->resource(QTextDocument::ImageResource, QUrl(href)));

    QTextImageFormat format;
    format.setName(href);

    if (img.width() > 560) {
        format.setWidth(560);
    }

    format.setHeight(img.height());

    mCursor->insertImage(format);

    return true;
}

bool Converter::convertEpigraph(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("p")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("poem")) {
            if (!convertPoem(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("cite")) {
            if (!convertCite(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("empty-line")) {
            if (!convertEmptyLine(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("text-author")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertPoem(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("title")) {
            if (!convertTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("epigraph")) {
            if (!convertEpigraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("empty-line")) {
            if (!convertEmptyLine(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("stanza")) {
            if (!convertStanza(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("text-author")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertSubTitle(const QDomElement &element)
{
    QTextFrame *topFrame = mCursor->currentFrame();

    QTextFrameFormat frameFormat;
    frameFormat.setBorder(1);
    frameFormat.setPadding(8);
    frameFormat.setBackground(Qt::lightGray);
    frameFormat.setTopMargin(16);

    mCursor->insertFrame(frameFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setPosition(topFrame->lastPosition());

    return true;
}

bool Converter::convertCite(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("p")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("poem")) {
            if (!convertParagraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("text-author")) {
            QTextBlockFormat format;
            format.setTextIndent(10);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("empty-line")) {
            if (!convertEmptyLine(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("subtitle")) {
            if (!convertSubTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("table")) {
            if (!convertTable(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertEmptyLine(const QDomElement &)
{
    mCursor->insertText(QStringLiteral("\n\n"));
    return true;
}

bool Converter::convertLink(const QDomElement &element)
{
    QString href = element.attributeNS(QStringLiteral("http://www.w3.org/1999/xlink"), QStringLiteral("href"));
    QString type = element.attributeNS(QStringLiteral("http://www.w3.org/1999/xlink"), QStringLiteral("type"));

    if (type == QLatin1String("note")) {
        mCursor->insertText(QStringLiteral("["));
    }

    int startPosition = mCursor->position();

    QTextCharFormat origFormat(mCursor->charFormat());

    QTextCharFormat format(mCursor->charFormat());
    format.setForeground(Qt::blue);
    format.setFontUnderline(true);
    mCursor->setCharFormat(format);

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("emphasis")) {
                if (!convertEmphasis(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("strong")) {
                if (!convertStrong(childElement)) {
                    return false;
                }
            } else if (childElement.tagName() == QLatin1String("style")) {
                if (!convertStyle(childElement)) {
                    return false;
                }
            }
        } else if (child.isText()) {
            const QDomText text = child.toText();
            if (!text.isNull()) {
                mCursor->insertText(text.data());
            }
        }

        child = child.nextSibling();
    }
    mCursor->setCharFormat(origFormat);

    int endPosition = mCursor->position();

    if (type == QLatin1String("note")) {
        mCursor->insertText(QStringLiteral("]"));
    }

    if (href.startsWith(QLatin1Char('#'))) { // local link
        mLocalLinks.insert(href.mid(1), QPair<int, int>(startPosition, endPosition));
    } else {
        // external link
        Okular::BrowseAction *action = new Okular::BrowseAction(QUrl(href));
        Q_EMIT addAction(action, startPosition, endPosition);
    }

    return true;
}

bool Converter::convertStanza(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("title")) {
            if (!convertTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("subtitle")) {
            if (!convertSubTitle(child)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("v")) {
            QTextBlockFormat format;
            format.setTextIndent(50);
            mCursor->insertBlock(format);
            if (!convertParagraph(child)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertCode(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat codeFormat(origFormat);
    codeFormat.setFontFamily(QStringLiteral("monospace"));
    mCursor->setCharFormat(codeFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertSuperScript(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat superScriptFormat(origFormat);
    superScriptFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    mCursor->setCharFormat(superScriptFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertSubScript(const QDomElement &element)
{
    QTextCharFormat origFormat = mCursor->charFormat();

    QTextCharFormat subScriptFormat(origFormat);
    subScriptFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    mCursor->setCharFormat(subScriptFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    mCursor->setCharFormat(origFormat);

    return true;
}

bool Converter::convertTable(const QDomElement &element)
{
    QTextFrame *topFrame = mCursor->currentFrame();

    QTextTable *table = nullptr;

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("tr")) {
            if (table) {
                table->appendRows(1);
            } else {
                QTextTableFormat tableFormat;
                tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
                table = mCursor->insertTable(1, 1, tableFormat);
            }

            if (!convertTableRow(child, *table)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    mCursor->setPosition(topFrame->lastPosition());

    return true;
}

bool Converter::convertTableRow(const QDomElement &element, QTextTable &table)
{
    QDomElement child = element.firstChildElement();
    int column = 0;
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("th")) {
            if (!convertTableHeaderCell(child, table, column)) {
                return false;
            }
        } else if (child.tagName() == QLatin1String("td")) {
            if (!convertTableCell(child, table, column)) {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertTableHeaderCell(const QDomElement &element, QTextTable &table, int &column)
{
    QTextCharFormat charFormat;
    charFormat.setFontWeight(QFont::Bold);
    return convertTableCellHelper(element, table, column, charFormat);
}

bool Converter::convertTableCell(const QDomElement &element, QTextTable &table, int &column)
{
    QTextCharFormat charFormat;
    return convertTableCellHelper(element, table, column, charFormat);
}

bool Converter::convertTableCellHelper(const QDomElement &element, QTextTable &table, int &column, const QTextCharFormat &charFormat)
{
    int row = table.rows() - 1;

    int colspan = qMax(element.attribute(QStringLiteral("colspan")).toInt(), 1);
    // TODO: rowspan
    // int rowspan = qMax(element.attribute(QStringLiteral("rowspan")).toInt(), 1);

    int columnsToAppend = column + colspan - table.columns();
    if (columnsToAppend > 0) {
        table.appendColumns(columnsToAppend);
    }

    table.mergeCells(row, column, 1, colspan);

    int cellCursorPosition = table.cellAt(row, column).firstPosition();
    mCursor->setPosition(cellCursorPosition);

    Qt::Alignment alignment;

    QString halign = element.attribute(QStringLiteral("halign"));
    if (halign == QStringLiteral("center")) {
        alignment |= Qt::AlignHCenter;
    } else if (halign == QStringLiteral("right")) {
        alignment |= Qt::AlignRight;
    } else {
        alignment |= Qt::AlignLeft;
    }

    QString valign = element.attribute(QStringLiteral("valign"));
    if (valign == QStringLiteral("center")) {
        alignment |= Qt::AlignVCenter;
    } else if (valign == QStringLiteral("bottom")) {
        alignment |= Qt::AlignBottom;
    } else {
        alignment |= Qt::AlignTop;
    }

    QTextBlockFormat format;
    format.setAlignment(alignment);
    mCursor->insertBlock(format, charFormat);

    if (!convertParagraph(element)) {
        return false;
    }

    column += colspan;
    return true;
}
