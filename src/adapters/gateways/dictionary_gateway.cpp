#include "dictionary_gateway.hpp"

namespace adapters::gateways
{

DictionaryGateway::DictionaryGateway(IDictionaryAccess* dictionaryAccess) :
    m_dictionaryAccess { dictionaryAccess }
{
}

void DictionaryGateway::getDefinitionForWord(const QString& word)
{
    m_dictionaryAccess->getDefinitionForWord(word);
}

}  // namespace adapters::gateways