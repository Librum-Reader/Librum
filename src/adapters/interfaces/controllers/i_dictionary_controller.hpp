#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "word_definition_dto.hpp"

namespace adapters
{

/**
 * The DictionaryController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for dictionary lookups. It acts as
 * a layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class ADAPTERS_EXPORT IDictionaryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(dtos::DictionaryEntryDto definition READ definition NOTIFY
                   definitionChanged)

public:
    virtual ~IDictionaryController() noexcept = default;

    Q_INVOKABLE virtual void getDefinitionForWord(const QString& word) = 0;
    virtual dtos::DictionaryEntryDto definition() const = 0;

signals:
    void definitionChanged();
    void definitionReceived(bool success, const QJsonObject& definition);
    void gettingDefinitionFailed();
    void gettingDefinitionSucceeded(const QString& word);
};

}  // namespace adapters
