#pragma once
#include <mupdf/classes.h>
#include <QList>
#include <QString>
#include "application_export.hpp"

namespace application::core
{

struct TOCItemData
{
    QString title;
    int pageNumber;
    float yOffset;
    fz_outline* internal;
};

class TOCItem
{
public:
    explicit TOCItem(const TOCItemData& data, TOCItem* parentItem = nullptr);
    ~TOCItem();

    void appendChild(TOCItem* child);

    TOCItem* child(int row);
    int childCount() const;
    int columnCount() const;
    TOCItemData data() const;
    int row() const;
    TOCItem* parentItem();
    void setParent(TOCItem* parent);
    const QList<TOCItem*>& getChildren() const;

private:
    QList<TOCItem*> m_children;
    TOCItemData m_data;
    TOCItem* m_parentItem;
};

}  // namespace application::core
