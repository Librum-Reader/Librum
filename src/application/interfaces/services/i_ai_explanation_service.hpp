#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

class APPLICATION_EXPORT IAiExplanationService : public QObject
{
    Q_OBJECT

public:
    virtual ~IAiExplanationService() noexcept = default;

    virtual void getExplanation(const QString& text, const QString& mode) = 0;

public slots:
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;

signals:
    void wordReady(const QString& word);
    void limitReached();
    void requestTooLong();
};

}  // namespace application