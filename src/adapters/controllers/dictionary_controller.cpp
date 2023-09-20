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
    emit startedGettingDefinition(word);
    m_dictionaryService->getDefinitionForWord(word);
    m_searchedForWord = word;
}

DictionaryEntryDto DictionaryController::definition() const
{
    return m_definition;
}

void DictionaryController::clearData()
{
    m_definition = DictionaryEntryDto {};
    m_previousDefinitions.clear();
    m_currentWord.clear();
    m_searchedForWord.clear();
    emit definitionChanged();
}

void DictionaryController::goToPreviousWord()
{
    if(m_previousDefinitions.isEmpty())
        return;

    auto previous = m_previousDefinitions.pop();
    m_currentWord = previous.first;
    emit startedGettingDefinition(m_currentWord);

    m_definition = previous.second;
    emit definitionChanged();
    gettingDefinitionSucceeded();
}

void DictionaryController::processDefinition(bool success,
                                             const QJsonObject& definition)
{
    if(!success)
    {
        emit gettingDefinitionFailed();
        return;
    }

    if(!m_currentWord.isEmpty())
        m_previousDefinitions.push({ m_currentWord, m_definition });

    m_definition = parseDefinition(definition);
    m_currentWord = m_searchedForWord;
    emit definitionChanged();
    gettingDefinitionSucceeded();
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