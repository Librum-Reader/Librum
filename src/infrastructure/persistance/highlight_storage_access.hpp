#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "i_highlight_storage_access.hpp"

namespace infrastructure::persistence
{

class HighlightStorageAccess : public adapters::IHighlightStorageAccess
{
public:
    HighlightStorageAccess();

    void createHighlight(
        const QString& authToken, const QString& bookUuid,
        const adapters::dtos::HighlightDto& highlight) override;
    void removeHighlight(const QString& authToken, const QString& bookUuid,
                         const QString& highlightUuid) override;

private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence