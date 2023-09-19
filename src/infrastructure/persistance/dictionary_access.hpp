#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include "i_dictionary_access.hpp"

namespace infrastructure::persistence
{

class DictionaryAccess : public adapters::IDictionaryAccess
{
    Q_OBJECT

private:
    QNetworkRequest createRequest(QUrl url);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence