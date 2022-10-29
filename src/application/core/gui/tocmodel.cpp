/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tocmodel.h"
#include <qdom.h>
#include <QApplication>
#include <QFont>
#include <QList>
#include <QTreeView>
#include "core/document.h"
#include "core/page.h"

Q_DECLARE_METATYPE(QModelIndex)

struct TOCItem
{
    TOCItem();
    TOCItem(TOCItem* parent, const QDomElement& e);
    ~TOCItem();

    TOCItem(const TOCItem&) = delete;
    TOCItem& operator=(const TOCItem&) = delete;

    QString text;
    Okular::DocumentViewport viewport;
    QString extFileName;
    QString url;
    bool highlight : 1;
    TOCItem* parent;
    QList<TOCItem*> children;
    TOCModelPrivate* model;
};

class TOCModelPrivate
{
public:
    explicit TOCModelPrivate(TOCModel* qq);
    ~TOCModelPrivate();

    void addChildren(const QDomNode& parentNode, TOCItem* parentItem);
    QModelIndex indexForItem(TOCItem* item) const;
    void findViewport(const Okular::DocumentViewport& viewport, TOCItem* item,
                      QList<TOCItem*>& list) const;

    TOCModel* q;
    TOCItem* root;
    bool dirty : 1;
    Okular::Document* document;
    QList<TOCItem*> itemsToOpen;
    QList<TOCItem*> currentPage;
    TOCModel* m_oldModel;
    QVector<QModelIndex> m_oldTocExpandedIndexes;
};

TOCItem::TOCItem() :
    highlight(false),
    parent(nullptr),
    model(nullptr)
{
}

TOCItem::TOCItem(TOCItem* _parent, const QDomElement& e) :
    highlight(false),
    parent(_parent)
{
    parent->children.append(this);
    model = parent->model;
    text = e.tagName();

    // viewport loading
    if(e.hasAttribute(QStringLiteral("Viewport")))
    {
        // if the node has a viewport, set it
        viewport =
            Okular::DocumentViewport(e.attribute(QStringLiteral("Viewport")));
    }
    else if(e.hasAttribute(QStringLiteral("ViewportName")))
    {
        // if the node references a viewport, get the reference and set it
        const QString& page = e.attribute(QStringLiteral("ViewportName"));
        QString viewport_string =
            model->document->metaData(QStringLiteral("NamedViewport"), page)
                .toString();
        if(!viewport_string.isEmpty())
        {
            viewport = Okular::DocumentViewport(viewport_string);
        }
    }

    extFileName = e.attribute(QStringLiteral("ExternalFileName"));
    url = e.attribute(QStringLiteral("URL"));
}

TOCItem::~TOCItem()
{
    qDeleteAll(children);
}

TOCModelPrivate::TOCModelPrivate(TOCModel* qq) :
    q(qq),
    root(new TOCItem),
    dirty(false),
    m_oldModel(nullptr)
{
    root->model = this;
}

TOCModelPrivate::~TOCModelPrivate()
{
    delete root;
    delete m_oldModel;
}

void TOCModelPrivate::addChildren(const QDomNode& parentNode,
                                  TOCItem* parentItem)
{
    TOCItem* currentItem = nullptr;
    QDomNode n = parentNode.firstChild();
    while(!n.isNull())
    {
        // convert the node to an element (sure it is)
        QDomElement e = n.toElement();

        // insert the entry as top level (listview parented) or 2nd+ level
        currentItem = new TOCItem(parentItem, e);

        // descend recursively and advance to the next node
        if(e.hasChildNodes())
        {
            addChildren(n, currentItem);
        }

        // open/keep close the item
        bool isOpen = false;
        if(e.hasAttribute(QStringLiteral("Open")))
        {
            isOpen = QVariant(e.attribute(QStringLiteral("Open"))).toBool();
        }
        if(isOpen)
        {
            itemsToOpen.append(currentItem);
        }

        n = n.nextSibling();
        Q_EMIT q->countChanged();
    }
}

QModelIndex TOCModelPrivate::indexForItem(TOCItem* item) const
{
    if(item->parent)
    {
        int id = item->parent->children.indexOf(item);
        if(id >= 0 && id < item->parent->children.count())
        {
            return q->createIndex(id, 0, item);
        }
    }
    return QModelIndex();
}

void TOCModelPrivate::findViewport(const Okular::DocumentViewport& viewport,
                                   TOCItem* item, QList<TOCItem*>& list) const
{
    TOCItem* todo = item;

    while(todo)
    {
        const TOCItem* current = todo;
        todo = nullptr;
        TOCItem* pos = nullptr;

        for(TOCItem* child : current->children)
        {
            if(child->viewport.isValid())
            {
                if(child->viewport.pageNumber <= viewport.pageNumber)
                {
                    pos = child;
                    if(child->viewport.pageNumber == viewport.pageNumber)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        if(pos)
        {
            list.append(pos);
            todo = pos;
        }
    }
}

TOCModel::TOCModel(Okular::Document* document, QObject* parent) :
    QAbstractItemModel(parent),
    d(new TOCModelPrivate(this))
{
    d->document = document;

    qRegisterMetaType<QModelIndex>();
}

TOCModel::~TOCModel()
{
    delete d;
}

QHash<int, QByteArray> TOCModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[PageRole] = "page";
    roles[PageLabelRole] = "pageLabel";
    roles[HighlightRole] = "highlight";
    roles[HighlightedParentRole] = "highlightedParent";
    return roles;
}

int TOCModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant TOCModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    TOCItem* item = static_cast<TOCItem*>(index.internalPointer());
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return item->text;
        break;
    case Qt::FontRole:
        if(item->highlight)
        {
            QFont font;
            font.setBold(true);

            TOCItem* lastHighlighted = d->currentPage.last();

            // in the mobile version our parent is not a QTreeView; embolden the
            // last highlighted item
            // TODO misusing parent() here, fix
            QTreeView* view = dynamic_cast<QTreeView*>(QObject::parent());
            if(!view)
            {
                if(item == lastHighlighted)
                {
                    return font;
                }
                return QVariant();
            }

            if(view->isExpanded(index))
            {
                // if this is the last highlighted node, its child is on a page
                // below, thus it gets emboldened
                if(item == lastHighlighted)
                {
                    return font;
                }
            }
            else
            {
                return font;
            }
        }
        break;
    case HighlightRole:
        return item->highlight;
    case PageRole:
        if(item->viewport.isValid())
        {
            return item->viewport.pageNumber + 1;
        }
        break;
    case PageLabelRole:
        if(item->viewport.isValid() &&
           item->viewport.pageNumber < int(d->document->pages()))
        {
            return d->document->page(item->viewport.pageNumber)->label();
        }
        break;
    }
    return QVariant();
}

bool TOCModel::hasChildren(const QModelIndex& parent) const
{
    if(!parent.isValid())
    {
        return true;
    }

    TOCItem* item = static_cast<TOCItem*>(parent.internalPointer());
    return !item->children.isEmpty();
}

QVariant TOCModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if(orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    if(section == 0 && role == Qt::DisplayRole)
    {
        return QStringLiteral("Topics");
    }

    return QVariant();
}

QModelIndex TOCModel::index(int row, int column,
                            const QModelIndex& parent) const
{
    if(row < 0 || column != 0)
    {
        return QModelIndex();
    }

    TOCItem* item = parent.isValid()
                        ? static_cast<TOCItem*>(parent.internalPointer())
                        : d->root;
    if(row < item->children.count())
    {
        return createIndex(row, column, item->children.at(row));
    }

    return QModelIndex();
}

QModelIndex TOCModel::parent(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }

    TOCItem* item = static_cast<TOCItem*>(index.internalPointer());
    return d->indexForItem(item->parent);
}

int TOCModel::rowCount(const QModelIndex& parent) const
{
    TOCItem* item = parent.isValid()
                        ? static_cast<TOCItem*>(parent.internalPointer())
                        : d->root;
    return item->children.count();
}

static QModelIndex indexForIndex(const QModelIndex& oldModelIndex,
                                 QAbstractItemModel* newModel)
{
    QModelIndex newModelIndex;
    if(oldModelIndex.parent().isValid())
    {
        newModelIndex =
            newModel->index(oldModelIndex.row(), oldModelIndex.column(),
                            indexForIndex(oldModelIndex.parent(), newModel));
    }
    else
    {
        newModelIndex =
            newModel->index(oldModelIndex.row(), oldModelIndex.column());
    }
    return newModelIndex;
}

void TOCModel::fill(const Okular::DocumentSynopsis* toc)
{
    if(!toc)
    {
        return;
    }

    clear();
    Q_EMIT layoutAboutToBeChanged();
    d->addChildren(*toc, d->root);
    d->dirty = true;
    Q_EMIT layoutChanged();
    if(equals(d->m_oldModel))
    {
        for(const QModelIndex& oldIndex : qAsConst(d->m_oldTocExpandedIndexes))
        {
            const QModelIndex index = indexForIndex(oldIndex, this);
            if(!index.isValid())
            {
                continue;
            }

            // TODO misusing parent() here, fix
            QMetaObject::invokeMethod(QObject::parent(), "expand",
                                      Qt::QueuedConnection,
                                      Q_ARG(QModelIndex, index));
        }
    }
    else
    {
        for(TOCItem* item : qAsConst(d->itemsToOpen))
        {
            const QModelIndex index = d->indexForItem(item);
            if(!index.isValid())
            {
                continue;
            }

            // TODO misusing parent() here, fix
            QMetaObject::invokeMethod(QObject::parent(), "expand",
                                      Qt::QueuedConnection,
                                      Q_ARG(QModelIndex, index));
        }
    }
    d->itemsToOpen.clear();
    delete d->m_oldModel;
    d->m_oldModel = nullptr;
    d->m_oldTocExpandedIndexes.clear();
}

void TOCModel::clear()
{
    if(!d->dirty)
    {
        return;
    }

    beginResetModel();
    qDeleteAll(d->root->children);
    d->root->children.clear();
    d->currentPage.clear();
    endResetModel();
    d->dirty = false;
}

void TOCModel::setCurrentViewport(const Okular::DocumentViewport& viewport)
{
    for(TOCItem* item : qAsConst(d->currentPage))
    {
        QModelIndex index = d->indexForItem(item);
        if(!index.isValid())
        {
            continue;
        }

        item->highlight = false;
        Q_EMIT dataChanged(index, index);
    }
    d->currentPage.clear();

    QList<TOCItem*> newCurrentPage;
    d->findViewport(viewport, d->root, newCurrentPage);

    d->currentPage = newCurrentPage;

    for(TOCItem* item : qAsConst(d->currentPage))
    {
        QModelIndex index = d->indexForItem(item);
        if(!index.isValid())
        {
            continue;
        }

        item->highlight = true;
        Q_EMIT dataChanged(index, index);
    }
}

bool TOCModel::isEmpty() const
{
    return d->root->children.isEmpty();
}

bool TOCModel::equals(const TOCModel* model) const
{
    if(model)
    {
        return checkequality(model);
    }
    else
    {
        return false;
    }
}

void TOCModel::setOldModelData(TOCModel* model,
                               const QVector<QModelIndex>& list)
{
    delete d->m_oldModel;
    d->m_oldModel = model;
    d->m_oldTocExpandedIndexes = list;
}

bool TOCModel::hasOldModelData() const
{
    return (d->m_oldModel != nullptr);
}

TOCModel* TOCModel::clearOldModelData() const
{
    TOCModel* oldModel = d->m_oldModel;
    d->m_oldModel = nullptr;
    d->m_oldTocExpandedIndexes.clear();
    return oldModel;
}

QString TOCModel::externalFileNameForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return QString();
    }

    TOCItem* item = static_cast<TOCItem*>(index.internalPointer());
    return item->extFileName;
}

Okular::DocumentViewport TOCModel::viewportForIndex(
    const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return Okular::DocumentViewport();
    }

    TOCItem* item = static_cast<TOCItem*>(index.internalPointer());
    return item->viewport;
}

QString TOCModel::urlForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return QString();
    }

    TOCItem* item = static_cast<TOCItem*>(index.internalPointer());
    return item->url;
}

bool TOCModel::checkequality(const TOCModel* model, const QModelIndex& parentA,
                             const QModelIndex& parentB) const
{
    if(rowCount(parentA) != model->rowCount(parentB))
    {
        return false;
    }
    for(int i = 0; i < rowCount(parentA); i++)
    {
        QModelIndex indxA = index(i, 0, parentA);
        QModelIndex indxB = model->index(i, 0, parentB);
        if(indxA.data() != indxB.data())
        {
            return false;
        }
        if(hasChildren(indxA) != model->hasChildren(indxB))
        {
            return false;
        }
        if(!checkequality(model, indxA, indxB))
        {
            return false;
        }
    }
    return true;
}

#include "moc_tocmodel.cpp"
