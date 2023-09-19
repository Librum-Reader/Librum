#include "dictionary_controller.hpp"

namespace adapters::controllers
{

DictionaryController::DictionaryController(
    application::IDictionaryService* dictionaryService) :
    m_dictionaryService(dictionaryService)
{
}

void DictionaryController::getDefinitionForWord(const QString& word)
{
    m_dictionaryService->getDefinitionForWord(word);
}

}  // namespace adapters::controllers