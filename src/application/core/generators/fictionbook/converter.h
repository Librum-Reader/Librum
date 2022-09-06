/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FICTIONBOOK_CONVERTER_H
#define FICTIONBOOK_CONVERTER_H

#include <core/textdocumentgenerator.h>

class QDomElement;
class QTextCharFormat;
class QTextCursor;
class QTextTable;

namespace FictionBook
{
class Converter : public Okular::TextDocumentConverter
{
    Q_OBJECT

public:
    Converter();
    ~Converter() override;

    QTextDocument *convert(const QString &fileName) override;

private:
    bool convertBody(const QDomElement &element);
    bool convertDescription(const QDomElement &element);
    bool convertSection(const QDomElement &element);
    bool convertTitle(const QDomElement &element);
    bool convertParagraph(const QDomElement &element);
    bool convertBinary(const QDomElement &element);
    bool convertCover(const QDomElement &element);
    bool convertImage(const QDomElement &element);
    bool convertEpigraph(const QDomElement &element);
    bool convertPoem(const QDomElement &element);
    bool convertSubTitle(const QDomElement &element);
    bool convertCite(const QDomElement &element);
    bool convertEmptyLine(const QDomElement &element);
    bool convertLink(const QDomElement &element);
    bool convertEmphasis(const QDomElement &element);
    bool convertStrong(const QDomElement &element);
    bool convertStrikethrough(const QDomElement &element);
    bool convertStyle(const QDomElement &element);
    bool convertStanza(const QDomElement &element);
    bool convertCode(const QDomElement &element);
    bool convertSuperScript(const QDomElement &element);
    bool convertSubScript(const QDomElement &element);
    bool convertTable(const QDomElement &element);
    bool convertTableRow(const QDomElement &element, QTextTable &table);
    bool convertTableHeaderCell(const QDomElement &element, QTextTable &table, int &column);
    bool convertTableCell(const QDomElement &element, QTextTable &table, int &column);
    bool convertTableCellHelper(const QDomElement &element, QTextTable &table, int &column, const QTextCharFormat &charFormat);

    bool convertTitleInfo(const QDomElement &element);
    bool convertDocumentInfo(const QDomElement &element);
    bool convertAuthor(const QDomElement &element, QString &firstName, QString &middleName, QString &lastName, QString &email, QString &nickname);
    bool convertDate(const QDomElement &element, QDate &date);
    bool convertTextNode(const QDomElement &element, QString &data);
    bool convertAnnotation(const QDomElement &element, QString &data);

    QTextDocument *mTextDocument;
    QTextCursor *mCursor;

    class TitleInfo;
    TitleInfo *mTitleInfo;

    class DocumentInfo;
    DocumentInfo *mDocumentInfo;

    int mSectionCounter;

    QMap<QString, QTextBlock> mSectionMap;
    QMap<QString, QPair<int, int>> mLocalLinks;
};

}

#endif
