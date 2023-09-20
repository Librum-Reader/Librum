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
        // If the search failed for a word that starts with a lowercase letter,
        // try again with the capitalized version of the word.
        // The first letter can oftentimes cause problems because words start
        // with an uppercase letter after a period, but the dictionary API is
        // case-sensitive so we change it to lowercase by default. This can
        // cause problems for names or different languages though.
        if(!m_searchedForWord[0].isUpper())
        {
            auto capitalizedWord =
                m_searchedForWord[0].toUpper() + m_searchedForWord.mid(1);
            getDefinitionForWord(capitalizedWord);
            return;
        }

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
    QString language;
    if(dictionaryEntry.contains("en"))
        language = "en";
    else
        language = dictionaryEntry.begin().key();

    for(auto wordType : dictionaryEntry[language].toArray())
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
            .language = wordType.toObject()["language"].toString(),
            .definitions = definitions,
        };

        wordTypes << QVariant::fromValue(wordTypeValue);
    }

    return DictionaryEntryDto {
        .wordTypes = wordTypes,
    };
}

}  // namespace adapters::controllers