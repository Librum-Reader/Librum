#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include "application_export.hpp"
#include "highlight.hpp"

namespace application
{

/**
 * The HighlightStorageGateway class acts as a layer of abstraction before the
 * HighlightStorageAccess class. It maps the highlights provided by the
 * application to the data format required for highlight storage API requests.
 */
class APPLICATION_EXPORT IHighlightStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IHighlightStorageGateway() noexcept = default;

    virtual void createHighlight(
        const QString& authToken, const QUuid& bookUuid,
        const domain::entities::Highlight& highlight) = 0;
    virtual void removeHighlight(const QString& authToken,
                                 const QUuid& bookUuid,
                                 const QUuid& highlightUuid) = 0;
};

}  // namespace application