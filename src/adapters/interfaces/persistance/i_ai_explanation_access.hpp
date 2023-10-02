#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The AiExplanation class makes the API calls to the server to get the
 * explanation for text.
 */
class ADAPTERS_EXPORT IAiExplanationAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IAiExplanationAccess() noexcept = default;

    virtual void getExplanation(const QString& authToken, const QString& query,
                                const QString& mode) = 0;

signals:
    void explanationReceived(const QString& explanation) const;
};

}  // namespace adapters