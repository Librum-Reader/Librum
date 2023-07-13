#pragma once
#include <QObject>
#include <QString>
#include <memory>
#include "document.hpp"
#include "toc/toc_model.hpp"

namespace cpp_elements
{

class DocumentItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(int pageCount READ getPageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int currentPage READ getCurrentPage WRITE setCurrentPage NOTIFY
                   currentPageChanged)
    Q_PROPERTY(float zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(application::core::TOCModel* tableOfContents READ
                   getTableOfContents NOTIFY tableOfContentsChanged)

public:
    QString getFilePath() const;
    void setFilePath(const QString& newFilePath);

    int getPageCount() const;

    void setCurrentPage(int newCurrentPage);
    int getCurrentPage() const;

    float getZoom() const;
    void setZoom(float newZoom);

    application::core::TOCModel* getTableOfContents() const;

    const application::core::Document* internal() const;

signals:
    void filePathChanged(QString filePath);
    void pageCountChanged(int pageCount);
    void currentPageChanged(int currentPage);
    void zoomChanged(float newZoom);
    void tableOfContentsChanged();

private:
    std::unique_ptr<application::core::Document> m_document;
    QString m_pageCount = 0;
    int m_currentPage = 0;
    float m_zoom = 1;
};

}  // namespace cpp_elements