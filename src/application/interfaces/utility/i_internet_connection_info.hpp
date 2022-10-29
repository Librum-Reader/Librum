#pragma once
#include <QObject>

namespace application
{

class IInternetConnectionInfo : public QObject
{
    Q_OBJECT

public:
    virtual ~IInternetConnectionInfo() noexcept = default;

    virtual void checkAvailability() = 0;

signals:
    void available();
    void unavailable();
};

}  // namespace application