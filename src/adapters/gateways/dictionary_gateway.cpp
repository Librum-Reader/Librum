#include "dictionary_gateway.hpp"

namespace adapters::gateways
{

DictionaryGateway::DictionaryGateway(IDictionaryAccess* dictionaryAccess) :
    m_dictionaryAccess { dictionaryAccess }
{
    connect(m_dictionaryAccess, &IDictionaryAccess::definitionReceived, this,
            &DictionaryGateway::definitionReceived);
}

void DictionaryGateway::getDefinitionForWord(const QString& word)
{
    m_dictionaryAccess->getDefinitionForWord(word);
}

}  // namespace adapters::gateways