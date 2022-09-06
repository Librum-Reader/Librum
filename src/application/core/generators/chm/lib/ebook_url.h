#ifndef EBOOK_URL_H
#define EBOOK_URL_H

#include <QUrl>

class EbookURL : public QUrl
{
public:
    EbookURL();
    EbookURL(const QString &url);
};

#endif // EBOOK_URL_H
