#pragma once
#include "application_export.hpp"
#include "i_dictionary_gateway.hpp"
#include "i_dictionary_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT DictionaryService : public IDictionaryService
{
    Q_OBJECT

public:
    DictionaryService(IDictionaryGateway* dictionaryGateway);

private:
    IDictionaryGateway* m_dictionaryGateway;
};

}  // namespace application::services