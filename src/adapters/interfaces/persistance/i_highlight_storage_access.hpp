#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "highlight_dto.hpp"

namespace adapters
{

/**
 * The UserStorageAccess class makes the API calls to the user storage server.
 */
class ADAPTERS_EXPORT IHighlightStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IHighlightStorageAccess() noexcept = default;

    virtual void createHighlight(const QString& authToken,
                                 const QString& bookUuid,
                                 const dtos::HighlightDto& highlight) = 0;
    virtual void removeHighlight(const QString& authToken,
                                 const QString& bookUuid,
                                 const QString& highlightUuid) = 0;
};

}  // namespace adapters
