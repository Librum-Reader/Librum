#pragma once

#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include "test_class.hpp"


class TestRequest : public QObject
{
    Q_OBJECT
public:
    explicit TestRequest(QObject *parent = nullptr);
    
public slots:
    void makeRequest();
    
    
private:
    void printResult();
    
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

