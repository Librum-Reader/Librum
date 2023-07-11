#pragma once
#include <QObject>
#include <QString>
#include <memory>
#include "document.hpp"

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

public:
    QString getFilePath() const;
    void setFilePath(const QString& newFilePath);

    int getPageCount() const;

    void setCurrentPage(int newCurrentPage);
    int getCurrentPage() const;

    float getZoom() const;
    void setZoom(float newZoom);

    const application::core::Document* internal() const;

signals:
    void filePathChanged(QString filePath);
    void pageCountChanged(int pageCount);
    void currentPageChanged(int currentPage);
    void zoomChanged(float newZoom);

private:
    std::unique_ptr<application::core::Document> m_document;
    QString m_pageCount;
    int m_currentPage;
    float m_zoom;
};

}  // namespace cpp_elements