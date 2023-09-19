#include "dictionary_controller.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>
#include "word_definition_dto.hpp"

namespace adapters::controllers
{

using namespace dtos;

DictionaryController::DictionaryController(
    application::IDictionaryService* dictionaryService) :
    m_dictionaryService(dictionaryService)
{
    connect(m_dictionaryService,
            &application::IDictionaryService::definitionReceived, this,
            &DictionaryController::processDefinition);
}

void DictionaryController::getDefinitionForWord(const QString& word)
{
    m_dictionaryService->getDefinitionForWord(word);
    m_currentWord = word;
}

DictionaryEntryDto DictionaryController::definition() const
{
    return m_definition;
}

void DictionaryController::processDefinition(bool success,
                                             const QJsonObject& definition)
{
    if(!success)
        return;

    m_definition = parseDefinition(definition);
    emit definitionChanged();
    emit gettingDefinitionSucceeded(m_currentWord);
}

DictionaryEntryDto DictionaryController::parseDefinition(
    const QJsonObject& dictionaryEntry)
{
    QVariantList wordTypes;
    for(auto wordType : dictionaryEntry["en"].toArray())
    {
        QVariantList definitions;
        for(auto definition : wordType.toObject()["definitions"].toArray())
        {
            QVariantList examples;
            for(auto example : definition.toObject()["examples"].toArray())
            {
                examples << QVariant::fromValue(example.toString());
            }

            WordDefinitionDto definitionValue {
                .definition = definition.toObject()["definition"].toString(),
                .examples = examples,
            };

            definitions << QVariant::fromValue(definitionValue);
        }

        WordTypeDto wordTypeValue {
            .partOfSpeech = wordType.toObject()["partOfSpeech"].toString(),
            .definitions = definitions,
        };

        wordTypes << QVariant::fromValue(wordTypeValue);
    }

    return DictionaryEntryDto {
        .wordTypes = wordTypes,
    };
}

}  // namespace adapters::controllers