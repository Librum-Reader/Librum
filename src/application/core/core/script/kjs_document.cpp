/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <assert.h>
#include <kjs/kjsarguments.h>
#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include <qwidget.h>
#include <QDebug>
#include "../document_p.h"
#include "../form.h"
#include "../page.h"
#include "kjs_data_p.h"
#include "kjs_document_p.h"
#include "kjs_field_p.h"
#include "kjs_ocg_p.h"

using namespace Okular;

static KJSPrototype* g_docProto;

// Document.numPages
static KJSObject docGetNumPages(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSNumber(doc->m_pagesVector.count());
}

// Document.pageNum (getter)
static KJSObject docGetPageNum(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSNumber(doc->m_parent->currentPage());
}

// Document.pageNum (setter)
static void docSetPageNum(KJSContext* ctx, void* object, KJSObject value)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    int page = value.toInt32(ctx);

    if(page == (int)doc->m_parent->currentPage())
    {
        return;
    }

    doc->m_parent->setViewportPage(page);
}

// Document.documentFileName
static KJSObject docGetDocumentFileName(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSString(doc->m_url.fileName());
}

// Document.filesize
static KJSObject docGetFilesize(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSNumber(doc->m_docSize);
}

// Document.path
static KJSObject docGetPath(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSString(doc->m_url.toDisplayString(QUrl::PreferLocalFile));
}

// Document.URL
static KJSObject docGetURL(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    return KJSString(doc->m_url.toDisplayString());
}

// Document.permStatusReady
static KJSObject docGetPermStatusReady(KJSContext*, void*)
{
    return KJSBoolean(true);
}

// Document.dataObjects
static KJSObject docGetDataObjects(KJSContext* ctx, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    const QList<EmbeddedFile*>* files = doc->m_generator->embeddedFiles();

    KJSArray dataObjects(ctx, files ? files->count() : 0);
    if(files)
    {
        QList<EmbeddedFile*>::ConstIterator it = files->begin(),
                                            itEnd = files->end();
        for(int i = 0; it != itEnd; ++it, ++i)
        {
            KJSObject newdata = JSData::wrapFile(ctx, *it);
            dataObjects.setProperty(ctx, QString::number(i), newdata);
        }
    }
    return dataObjects;
}

// Document.external
static KJSObject docGetExternal(KJSContext*, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);
    QWidget* widget = doc->m_widget;

    const bool isShell = (widget && widget->parentWidget() &&
                          widget->parentWidget()->objectName().startsWith(
                              QLatin1String("okular::Shell")));
    return KJSBoolean(!isShell);
}

// Document.numFields
static KJSObject docGetNumFields(KJSContext*, void* object)
{
    const DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    unsigned int numFields = 0;

    for(const Page* pIt : qAsConst(doc->m_pagesVector))
    {
        numFields += pIt->formFields().size();
    }

    return KJSNumber(numFields);
}

static KJSObject docGetInfo(KJSContext* ctx, void* object)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    KJSObject obj;
    QSet<DocumentInfo::Key> keys;
    keys << DocumentInfo::Title << DocumentInfo::Author << DocumentInfo::Subject
         << DocumentInfo::Keywords << DocumentInfo::Creator
         << DocumentInfo::Producer;
    const DocumentInfo docinfo = doc->m_parent->documentInfo(keys);
#define KEY_GET(key, property)                                                \
    do                                                                        \
    {                                                                         \
        const QString data = docinfo.get(key);                                \
        if(!data.isEmpty())                                                   \
        {                                                                     \
            const KJSString newval(data);                                     \
            obj.setProperty(ctx, QStringLiteral(property), newval);           \
            obj.setProperty(ctx, QStringLiteral(property).toLower(), newval); \
        }                                                                     \
    }                                                                         \
    while(0);
    KEY_GET(DocumentInfo::Title, "Title");
    KEY_GET(DocumentInfo::Author, "Author");
    KEY_GET(DocumentInfo::Subject, "Subject");
    KEY_GET(DocumentInfo::Keywords, "Keywords");
    KEY_GET(DocumentInfo::Creator, "Creator");
    KEY_GET(DocumentInfo::Producer, "Producer");
#undef KEY_GET
    return obj;
}

#define DOCINFO_GET_METHOD(key, name)                                      \
    static KJSObject docGet##name(KJSContext*, void* object)               \
    {                                                                      \
        DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object); \
        const DocumentInfo docinfo =                                       \
            doc->m_parent->documentInfo(QSet<DocumentInfo::Key>() << key); \
        return KJSString(docinfo.get(key));                                \
    }

DOCINFO_GET_METHOD(DocumentInfo::Author, Author)
DOCINFO_GET_METHOD(DocumentInfo::Creator, Creator)
DOCINFO_GET_METHOD(DocumentInfo::Keywords, Keywords)
DOCINFO_GET_METHOD(DocumentInfo::Producer, Producer)
DOCINFO_GET_METHOD(DocumentInfo::Title, Title)
DOCINFO_GET_METHOD(DocumentInfo::Subject, Subject)

#undef DOCINFO_GET_METHOD

// Document.getField()
static KJSObject docGetField(KJSContext* context, void* object,
                             const KJSArguments& arguments)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    QString cName = arguments.at(0).toString(context);

    QVector<Page*>::const_iterator pIt = doc->m_pagesVector.constBegin(),
                                   pEnd = doc->m_pagesVector.constEnd();
    for(; pIt != pEnd; ++pIt)
    {
        const QList<Okular::FormField*> pageFields = (*pIt)->formFields();
        for(FormField* form : pageFields)
        {
            if(form->fullyQualifiedName() == cName)
            {
                return JSField::wrapField(context, form, *pIt);
            }
        }
    }
    return KJSUndefined();
}

// Document.getPageLabel()
static KJSObject docGetPageLabel(KJSContext* ctx, void* object,
                                 const KJSArguments& arguments)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);
    int nPage = arguments.at(0).toInt32(ctx);
    Page* p = doc->m_pagesVector.value(nPage);
    return KJSString(p ? p->label() : QString());
}

// Document.getPageRotation()
static KJSObject docGetPageRotation(KJSContext* ctx, void* object,
                                    const KJSArguments& arguments)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);
    int nPage = arguments.at(0).toInt32(ctx);
    Page* p = doc->m_pagesVector.value(nPage);
    return KJSNumber(p ? p->orientation() * 90 : 0);
}

// Document.gotoNamedDest()
static KJSObject docGotoNamedDest(KJSContext* ctx, void* object,
                                  const KJSArguments& arguments)
{
    DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    QString dest = arguments.at(0).toString(ctx);

    DocumentViewport viewport(
        doc->m_generator->metaData(QStringLiteral("NamedViewport"), dest)
            .toString());
    if(!viewport.isValid())
    {
        return KJSUndefined();
    }

    doc->m_parent->setViewport(viewport);

    return KJSUndefined();
}

// Document.syncAnnotScan()
static KJSObject docSyncAnnotScan(KJSContext*, void*, const KJSArguments&)
{
    return KJSUndefined();
}

// Document.getNthFieldName
static KJSObject docGetNthFieldName(KJSContext* ctx, void* object,
                                    const KJSArguments& arguments)
{
    const DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    int numField = arguments.at(0).toInt32(ctx);

    for(const Page* pIt : qAsConst(doc->m_pagesVector))
    {
        const QList<Okular::FormField*> pageFields = pIt->formFields();

        if(numField < pageFields.size())
        {
            const Okular::FormField* form = pageFields[numField];

            return KJSString(form->fullyQualifiedName());
        }

        numField -= pageFields.size();
    }

    return KJSUndefined();
}

static KJSObject docGetOCGs(KJSContext* ctx, void* object, const KJSArguments&)
{
    const DocumentPrivate* doc = reinterpret_cast<DocumentPrivate*>(object);

    QAbstractItemModel* model = doc->m_parent->layersModel();

    KJSArray array(ctx, model->rowCount());

    for(int i = 0; i < model->rowCount(); ++i)
    {
        for(int j = 0; j < model->columnCount(); ++j)
        {
            const QModelIndex index = model->index(i, j);

            KJSObject item = JSOCG::wrapOCGObject(ctx, model, i, j);
            item.setProperty(ctx, QStringLiteral("name"),
                             model->data(index, Qt::DisplayRole).toString());
            item.setProperty(ctx, QStringLiteral("initState"),
                             model->data(index, Qt::CheckStateRole).toBool());

            array.setProperty(ctx, QString::number(i), item);
        }
    }

    return array;
}

void JSDocument::initType(KJSContext* ctx)
{
    assert(g_docProto);

    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    g_docProto->defineProperty(ctx, QStringLiteral("numPages"), docGetNumPages);
    g_docProto->defineProperty(ctx, QStringLiteral("pageNum"), docGetPageNum,
                               docSetPageNum);
    g_docProto->defineProperty(ctx, QStringLiteral("documentFileName"),
                               docGetDocumentFileName);
    g_docProto->defineProperty(ctx, QStringLiteral("filesize"), docGetFilesize);
    g_docProto->defineProperty(ctx, QStringLiteral("path"), docGetPath);
    g_docProto->defineProperty(ctx, QStringLiteral("URL"), docGetURL);
    g_docProto->defineProperty(ctx, QStringLiteral("permStatusReady"),
                               docGetPermStatusReady);
    g_docProto->defineProperty(ctx, QStringLiteral("dataObjects"),
                               docGetDataObjects);
    g_docProto->defineProperty(ctx, QStringLiteral("external"), docGetExternal);
    g_docProto->defineProperty(ctx, QStringLiteral("numFields"),
                               docGetNumFields);

    // info properties
    g_docProto->defineProperty(ctx, QStringLiteral("info"), docGetInfo);
    g_docProto->defineProperty(ctx, QStringLiteral("author"), docGetAuthor);
    g_docProto->defineProperty(ctx, QStringLiteral("creator"), docGetCreator);
    g_docProto->defineProperty(ctx, QStringLiteral("keywords"), docGetKeywords);
    g_docProto->defineProperty(ctx, QStringLiteral("producer"), docGetProducer);
    g_docProto->defineProperty(ctx, QStringLiteral("title"), docGetTitle);
    g_docProto->defineProperty(ctx, QStringLiteral("subject"), docGetSubject);

    g_docProto->defineFunction(ctx, QStringLiteral("getField"), docGetField);
    g_docProto->defineFunction(ctx, QStringLiteral("getPageLabel"),
                               docGetPageLabel);
    g_docProto->defineFunction(ctx, QStringLiteral("getPageRotation"),
                               docGetPageRotation);
    g_docProto->defineFunction(ctx, QStringLiteral("gotoNamedDest"),
                               docGotoNamedDest);
    g_docProto->defineFunction(ctx, QStringLiteral("syncAnnotScan"),
                               docSyncAnnotScan);
    g_docProto->defineFunction(ctx, QStringLiteral("getNthFieldName"),
                               docGetNthFieldName);
    g_docProto->defineFunction(ctx, QStringLiteral("getOCGs"), docGetOCGs);
}

KJSGlobalObject JSDocument::wrapDocument(DocumentPrivate* doc)
{
    if(!g_docProto)
    {
        g_docProto = new KJSPrototype();
    }
    return g_docProto->constructGlobalObject(doc);
}
