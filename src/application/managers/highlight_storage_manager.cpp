#include "highlight_storage_manager.hpp"
#include <QDebug>

namespace application::managers
{

HighlightStorageManager::HighlightStorageManager(
    IHighlightStorageGateway* highlightStorageGateway,
    ILocalLibraryTracker* localLibraryTracker) :
    m_highlightStorageGateway { highlightStorageGateway },
    m_localLibraryTracker { localLibraryTracker }
{
}

void HighlightStorageManager::addHighlight(
    const domain::entities::Book& book,
    const domain::entities::Highlight& highlight)
{
    m_localLibraryTracker->updateTrackedBook(book);
    m_highlightStorageGateway->createHighlight(m_authenticationToken,
                                               book.getUuid(), highlight);
}

void HighlightStorageManager::deleteHighlight(
    const domain::entities::Book& book, const QUuid& highlightUuid)
{
    m_localLibraryTracker->updateTrackedBook(book);
    m_highlightStorageGateway->removeHighlight(m_authenticationToken,
                                               book.getUuid(), highlightUuid);
}

void HighlightStorageManager::setUserData(const QString& email,
                                          const QString& authToken)
{
    m_localLibraryTracker->setLibraryOwner(email);
    m_authenticationToken = authToken;
}

void HighlightStorageManager::clearUserData()
{
    m_localLibraryTracker->setLibraryOwner("");
    m_authenticationToken.clear();
}


}  // namespace application::managers