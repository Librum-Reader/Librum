#pragma once
#include <QObject>
#include "application_export.hpp"

namespace application
{

/**
 * The DictionaryGateway class acts as a layer of abstraction before the
 * DictionaryAccess class. It maps the data provided by the application to
 * the data type required for dictionary API requests.
 */
class APPLICATION_EXPORT IDictionaryGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IDictionaryGateway() noexcept = default;

    virtual void getDefinitionForWord(const QString& word) = 0;

signals:
    void definitionReceived(bool success, const QJsonObject& definition);
};

}  // namespace application
