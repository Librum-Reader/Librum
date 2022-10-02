#pragma once
#include <QObject>
#include "observer.h"


namespace application::utility
{

class CoverObserver : public QObject, public Okular::DocumentObserver
{
    Q_OBJECT

public:
    void notifyPageChanged(int page, int flags) override;

signals:
    void pageChanged(int page, int flags);
};

} // namespace application::utility