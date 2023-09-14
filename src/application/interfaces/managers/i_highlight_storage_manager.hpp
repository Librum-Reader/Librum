#pragma once
#include <QObject>
#include "application_export.hpp"
#include "book.hpp"
#include "highlight.hpp"

namespace application
{

/**
 * The LibraryStorageManager manages the book storage for the local and remote
 * library.
 */
class APPLICATION_EXPORT IHighlightStorageManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IHighlightStorageManager() noexcept = default;

    virtual void addHighlight(domain::entities::Book& book,
                              const domain::entities::Highlight& highlight) = 0;
    virtual void deleteHighlight(domain::entities::Book& book,
                                 const QUuid& highlightUuid) = 0;
};

}  // namespace application