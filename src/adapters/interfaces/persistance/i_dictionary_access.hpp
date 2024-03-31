#pragma once
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The IDictionaryAccess class makes the API calls to the API of the used
 * dictionary to retrieve definitions, etc. from it.
 */
class ADAPTERS_EXPORT IDictionaryAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IDictionaryAccess() noexcept = default;

    virtual void getDefinitionForWord(const QString& word) = 0;

signals:
    void definitionReceived(bool success, const QJsonObject& definition);
};

}  // namespace adapters
