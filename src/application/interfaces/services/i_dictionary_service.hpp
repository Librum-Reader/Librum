#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

/**
 *  The DictionaryService handles all dictionary operations like lookups, etc.
 */
class APPLICATION_EXPORT IDictionaryService : public QObject
{
    Q_OBJECT

public:
    virtual ~IDictionaryService() noexcept = default;

    virtual void getDefinitionForWord(const QString& word) = 0;
};

}  // namespace application
