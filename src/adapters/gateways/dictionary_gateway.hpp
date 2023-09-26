#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_dictionary_access.hpp"
#include "i_dictionary_gateway.hpp"

namespace adapters::gateways
{

class ADAPTERS_EXPORT DictionaryGateway : public application::IDictionaryGateway
{
    Q_OBJECT

public:
    DictionaryGateway(IDictionaryAccess* dictionaryAccess);

    void getDefinitionForWord(const QString& word) override;

private:
    IDictionaryAccess* m_dictionaryAccess;
};

}  // namespace adapters::gateways