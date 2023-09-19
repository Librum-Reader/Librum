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

public:
    void getDefinitionForWord(const QString& word) override;

private:
    QNetworkRequest createRequest(QUrl url);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence