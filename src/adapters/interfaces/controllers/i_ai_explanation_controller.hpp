#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The AuthenticationController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for application ai explanation
 * operations.
 */
class ADAPTERS_EXPORT IAiExplanationController : public QObject
{
    Q_OBJECT

public:
    virtual ~IAiExplanationController() noexcept = default;

    Q_INVOKABLE virtual void getExplanation(const QString& text,
                                            const QString& mode) = 0;

signals:
    void wordReady(const QString& word);
    void limitReached();
    void requestTooLong();
};

}  // namespace adapters
