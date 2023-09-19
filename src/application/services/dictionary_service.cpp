#include "dictionary_service.hpp"

namespace application::services
{

DictionaryService::DictionaryService(IDictionaryGateway* dictionaryGateway) :
    m_dictionaryGateway { dictionaryGateway }
{
    connect(m_dictionaryGateway, &IDictionaryGateway::definitionReceived, this,
            &DictionaryService::definitionReceived);
}

void DictionaryService::getDefinitionForWord(const QString& word)
{
    m_dictionaryGateway->getDefinitionForWord(word);
}

}  // namespace application::services