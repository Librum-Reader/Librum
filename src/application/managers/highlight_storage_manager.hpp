#pragma once
#include "i_highlight_storage_gateway.hpp"
#include "i_highlight_storage_manager.hpp"
#include "i_local_library_tracker.hpp"

namespace application::managers
{

class HighlightStorageManager : public IHighlightStorageManager
{
public:
    HighlightStorageManager(IHighlightStorageGateway* highlightStorageGateway,
                            ILocalLibraryTracker* downloadedBooksTracker);

    void addHighlight(const domain::entities::Book& book,
                      const domain::entities::Highlight& highlight) override;
    void deleteHighlight(const domain::entities::Book& book,
                         const QUuid& highlightUuid) override;

    void setUserData(const QString& email, const QString& authToken) override;
    void clearUserData() override;

private:
    IHighlightStorageGateway* m_highlightStorageGateway;
    ILocalLibraryTracker* m_localLibraryTracker;

    QString m_authenticationToken;
};

}  // namespace application::managers