#include "dictionary_gateway.hpp"

namespace adapters::gateways
{

DictionaryGateway::DictionaryGateway(IDictionaryAccess* dictionaryAccess) :
    m_dictionaryAccess { dictionaryAccess }
{
}

}  // namespace adapters::gateways