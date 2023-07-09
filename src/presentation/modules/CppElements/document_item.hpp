#pragma once
#include <QObject>
#include <memory>
#include "document.hpp"

namespace cpp_elements
{

class DocumentItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath WRITE setFilePath)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

public:
    DocumentItem() = default;

    QString filePath() const;
    void setFilePath(const QString& newFilePath);

    int pageCount() const;
    void setPageCount(int newPageCount);

    const application::core::Document* internal() const;

signals:
    void filePathChanged();
    void pageCountChanged();

private:
    std::unique_ptr<application::core::Document> m_document;
};

}  // namespace cpp_elements