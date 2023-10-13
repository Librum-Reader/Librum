#pragma once
#include <QByteArray>
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

/**
 * The AiExplanationGateway class acts as a layer of abstraction before the
 * AiExplanationAccess class. It maps the data provided by the application to
 * the data type required by the API.
 */
class APPLICATION_EXPORT IAiExplanationGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IAiExplanationGateway() noexcept = default;

    virtual void getExplanation(const QString& authToken, const QString& text,
                                const QString& mode) = 0;

signals:
    void wordReady(const QString& explanation);
    void errorOccured(const int code);
};

}  // namespace application