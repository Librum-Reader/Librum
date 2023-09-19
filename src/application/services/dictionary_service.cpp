#include "dictionary_service.hpp"

namespace application::services
{

DictionaryService::DictionaryService(IDictionaryGateway* dictionaryGateway) :
    m_dictionaryGateway { dictionaryGateway }
{
}

}  // namespace application::services