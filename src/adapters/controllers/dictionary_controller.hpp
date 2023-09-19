#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "i_dictionary_controller.hpp"
#include "i_dictionary_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT DictionaryController : public IDictionaryController
{
    Q_OBJECT

public:
    DictionaryController(application::IDictionaryService* dictionaryService);

    void getDefinitionForWord(const QString& word) override;

private:
    application::IDictionaryService* m_dictionaryService;
};

}  // namespace adapters::controllers
