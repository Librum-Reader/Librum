#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "i_dictionary_controller.hpp"
#include "i_dictionary_service.hpp"
#include "word_definition_dto.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT DictionaryController : public IDictionaryController
{
    Q_OBJECT

public:
    DictionaryController(application::IDictionaryService* dictionaryService);

    void getDefinitionForWord(const QString& word) override;
    dtos::DictionaryEntryDto definition() const override;

private slots:
    void processDefinition(bool success, const QJsonObject& definition);

private:
    dtos::DictionaryEntryDto parseDefinition(const QJsonObject& definition);

    application::IDictionaryService* m_dictionaryService;
    dtos::DictionaryEntryDto m_definition;
    QString m_currentWord;
};

}  // namespace adapters::controllers
