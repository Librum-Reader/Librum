#include "icon_model.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace adapters::data_models
{

IconModel::IconModel()
{
    QFile file(m_jsonFilePath);
    if(!file.open(QIODevice::ReadOnly))
        return;

    auto data = file.readAll();
    QJsonParseError error;
    auto jsonDocument = QJsonDocument::fromJson(data, &error);
    auto jsonArray = jsonDocument.array();
    for(auto&& item : jsonArray)
    {
        auto iconObj = item.toObject();
        auto name = iconObj["name"].toString();
        QList<QString> keywords;
        for(auto&& keyword : iconObj["keywords"].toArray())
            keywords.append(keyword.toString());

        IconItem iconItem {
            .name = name,
            .keywords = keywords,
        };

        m_data.append(iconItem);
    }
}

QVariant IconModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    auto iconItem = m_data.at(index.row());
    switch(role)
    {
    case NameRole:
        return iconItem.name;
    case KeywordsRole:
        return QVariant::fromValue(iconItem.keywords);
    }

    return QVariant();
}

int IconModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data.size();
}

QHash<int, QByteArray> IconModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { NameRole, "name" },
    };

    return roles;
}

}  // namespace adapters::data_models