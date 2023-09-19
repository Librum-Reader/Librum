#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The DictionaryAccess class makes the API calls to the API of the used
 * dictionary to retrieve definitions, etc. from it.
 */
class ADAPTERS_EXPORT IDictionaryAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IDictionaryAccess() noexcept = default;

    virtual void getDefinitionForWord(const QString& word) = 0;
};

}  // namespace adapters
