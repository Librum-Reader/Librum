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
    Q_PROPERTY(QString filePath WRITE setFilePath)

public:
    QString getFilePath() const;
    void setFilePath(const QString& newFilePath);

    const application::core::Document* internal() const;

private:
    std::unique_ptr<application::core::Document> m_document;
};

}  // namespace cpp_elements