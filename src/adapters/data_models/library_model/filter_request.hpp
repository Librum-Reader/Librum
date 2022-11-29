#pragma once
#include <QObject>
#include <QString>

namespace adapters::data_models
{

struct FilterRequest
{
    QString authors = "";
    QString format = "";
    QString date = "";
    bool onlyBooks = false;
    bool onlyFiles = false;
    bool read = false;
    bool unread = false;
};

}  // namespace adapters::data_models