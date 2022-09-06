/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kjs_field_p.h"

#include <kjs/kjsarguments.h>
#include <kjs/kjsinterpreter.h>
#include <kjs/kjsprototype.h>

#include <QHash>

#include <QDebug>
#include <QTimer>

#include "../debug_p.h"
#include "../document_p.h"
#include "../form.h"
#include "../page.h"
#include "../page_p.h"
#include "kjs_display_p.h"

using namespace Okular;

#define OKULAR_NAME QStringLiteral("okular_name")

static KJSPrototype *g_fieldProto;

typedef QHash<FormField *, Page *> FormCache;
Q_GLOBAL_STATIC(FormCache, g_fieldCache)
typedef QHash<QString, FormField *> ButtonCache;
Q_GLOBAL_STATIC(ButtonCache, g_buttonCache)

// Helper for modified fields
static void updateField(FormField *field)
{
    Page *page = g_fieldCache->value(field);
    if (page) {
        Document *doc = PagePrivate::get(page)->m_doc->m_parent;
        const int pageNumber = page->number();
        QTimer::singleShot(0, doc, [doc, pageNumber] { doc->refreshPixmaps(pageNumber); });
        Q_EMIT doc->refreshFormWidget(field);
    } else {
        qWarning() << "Could not get page of field" << field;
    }
}

// Field.doc
static KJSObject fieldGetDoc(KJSContext *context, void *)
{
    return context->interpreter().globalObject();
}

// Field.name
static KJSObject fieldGetName(KJSContext *, void *object)
{
    const FormField *field = reinterpret_cast<FormField *>(object);
    return KJSString(field->fullyQualifiedName());
}

// Field.readonly (getter)
static KJSObject fieldGetReadOnly(KJSContext *, void *object)
{
    const FormField *field = reinterpret_cast<FormField *>(object);
    return KJSBoolean(field->isReadOnly());
}

// Field.readonly (setter)
static void fieldSetReadOnly(KJSContext *context, void *object, KJSObject value)
{
    FormField *field = reinterpret_cast<FormField *>(object);
    bool b = value.toBoolean(context);
    field->setReadOnly(b);

    updateField(field);
}

static QString fieldGetTypeHelper(const FormField *field)
{
    switch (field->type()) {
    case FormField::FormButton: {
        const FormFieldButton *button = static_cast<const FormFieldButton *>(field);
        switch (button->buttonType()) {
        case FormFieldButton::Push:
            return QStringLiteral("button");
        case FormFieldButton::CheckBox:
            return QStringLiteral("checkbox");
        case FormFieldButton::Radio:
            return QStringLiteral("radiobutton");
        }
        break;
    }
    case FormField::FormText:
        return QStringLiteral("text");
    case FormField::FormChoice: {
        const FormFieldChoice *choice = static_cast<const FormFieldChoice *>(field);
        switch (choice->choiceType()) {
        case FormFieldChoice::ComboBox:
            return QStringLiteral("combobox");
        case FormFieldChoice::ListBox:
            return QStringLiteral("listbox");
        }
        break;
    }
    case FormField::FormSignature:
        return QStringLiteral("signature");
    }
    return QString();
}

// Field.type
static KJSObject fieldGetType(KJSContext *, void *object)
{
    const FormField *field = reinterpret_cast<FormField *>(object);

    return KJSString(fieldGetTypeHelper(field));
}

// Field.value (getter)
static KJSObject fieldGetValue(KJSContext * /*context*/, void *object)
{
    FormField *field = reinterpret_cast<FormField *>(object);

    switch (field->type()) {
    case FormField::FormButton: {
        const FormFieldButton *button = static_cast<const FormFieldButton *>(field);
        if (button->state()) {
            return KJSString(QStringLiteral("Yes"));
        }
        return KJSString(QStringLiteral("Off"));
    }
    case FormField::FormText: {
        const FormFieldText *text = static_cast<const FormFieldText *>(field);
        return KJSString(text->text());
    }
    case FormField::FormChoice: {
        const FormFieldChoice *choice = static_cast<const FormFieldChoice *>(field);
        const QList<int> currentChoices = choice->currentChoices();
        if (currentChoices.count() == 1) {
            return KJSString(choice->exportValueForChoice(choice->choices().at(currentChoices[0])));
        }
        break;
    }
    case FormField::FormSignature: {
        break;
    }
    }

    return KJSUndefined();
}

// Field.value (setter)
static void fieldSetValue(KJSContext *context, void *object, KJSObject value)
{
    FormField *field = reinterpret_cast<FormField *>(object);

    switch (field->type()) {
    case FormField::FormButton: {
        FormFieldButton *button = static_cast<FormFieldButton *>(field);
        const QString text = value.toString(context);
        if (text == QStringLiteral("Yes")) {
            button->setState(true);
            updateField(field);
        } else if (text == QStringLiteral("Off")) {
            button->setState(false);
            updateField(field);
        }
        break;
    }
    case FormField::FormText: {
        FormFieldText *textField = static_cast<FormFieldText *>(field);
        const QString text = value.toString(context);
        if (text != textField->text()) {
            textField->setText(text);
            updateField(field);
        }
        break;
    }
    case FormField::FormChoice: {
        FormFieldChoice *choice = static_cast<FormFieldChoice *>(field);
        Q_UNUSED(choice); // ###
        break;
    }
    case FormField::FormSignature: {
        break;
    }
    }
}

// Field.hidden (getter)
static KJSObject fieldGetHidden(KJSContext *, void *object)
{
    const FormField *field = reinterpret_cast<FormField *>(object);
    return KJSBoolean(!field->isVisible());
}

// Field.hidden (setter)
static void fieldSetHidden(KJSContext *context, void *object, KJSObject value)
{
    FormField *field = reinterpret_cast<FormField *>(object);
    bool b = value.toBoolean(context);
    field->setVisible(!b);

    updateField(field);
}

// Field.display (getter)
static KJSObject fieldGetDisplay(KJSContext *, void *object)
{
    const FormField *field = reinterpret_cast<FormField *>(object);
    bool visible = field->isVisible();
    if (visible) {
        return KJSNumber(field->isPrintable() ? FormDisplay::FormVisible : FormDisplay::FormNoPrint);
    }
    return KJSNumber(field->isPrintable() ? FormDisplay::FormNoView : FormDisplay::FormHidden);
}

// Field.display (setter)
static void fieldSetDisplay(KJSContext *context, void *object, KJSObject value)
{
    FormField *field = reinterpret_cast<FormField *>(object);
    const unsigned int b = value.toInt32(context);
    switch (b) {
    case FormDisplay::FormVisible:
        field->setVisible(true);
        field->setPrintable(true);
        break;
    case FormDisplay::FormHidden:
        field->setVisible(false);
        field->setPrintable(false);
        break;
    case FormDisplay::FormNoPrint:
        field->setVisible(true);
        field->setPrintable(false);
        break;
    case FormDisplay::FormNoView:
        field->setVisible(false);
        field->setPrintable(true);
        break;
    }
    updateField(field);
}

//  Instead of getting the Icon, we pick the field.
static KJSObject fieldButtonGetIcon(KJSContext *ctx, void *object, const KJSArguments &)
{
    FormField *field = reinterpret_cast<FormField *>(object);

    KJSObject fieldObject;
    fieldObject.setProperty(ctx, OKULAR_NAME, field->fullyQualifiedName());
    g_buttonCache->insert(field->fullyQualifiedName(), field);

    return fieldObject;
}

/*
 * Now we send to the button what Icon should be drawn on it
 */
static KJSObject fieldButtonSetIcon(KJSContext *ctx, void *object, const KJSArguments &arguments)
{
    FormField *field = reinterpret_cast<FormField *>(object);

    const QString fieldName = arguments.at(0).property(ctx, OKULAR_NAME).toString(ctx);

    if (field->type() == Okular::FormField::FormButton) {
        FormFieldButton *button = static_cast<FormFieldButton *>(field);
        button->setIcon(g_buttonCache->value(fieldName));
    }

    updateField(field);

    return KJSUndefined();
}

void JSField::initType(KJSContext *ctx)
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    if (!g_fieldProto) {
        g_fieldProto = new KJSPrototype();
    }

    g_fieldProto->defineProperty(ctx, QStringLiteral("doc"), fieldGetDoc);
    g_fieldProto->defineProperty(ctx, QStringLiteral("name"), fieldGetName);
    g_fieldProto->defineProperty(ctx, QStringLiteral("readonly"), fieldGetReadOnly, fieldSetReadOnly);
    g_fieldProto->defineProperty(ctx, QStringLiteral("type"), fieldGetType);
    g_fieldProto->defineProperty(ctx, QStringLiteral("value"), fieldGetValue, fieldSetValue);
    g_fieldProto->defineProperty(ctx, QStringLiteral("hidden"), fieldGetHidden, fieldSetHidden);
    g_fieldProto->defineProperty(ctx, QStringLiteral("display"), fieldGetDisplay, fieldSetDisplay);

    g_fieldProto->defineFunction(ctx, QStringLiteral("buttonGetIcon"), fieldButtonGetIcon);
    g_fieldProto->defineFunction(ctx, QStringLiteral("buttonSetIcon"), fieldButtonSetIcon);
}

KJSObject JSField::wrapField(KJSContext *ctx, FormField *field, Page *page)
{
    // ### cache unique wrapper
    KJSObject f = g_fieldProto->constructObject(ctx, field);
    f.setProperty(ctx, QStringLiteral("page"), page->number());
    g_fieldCache->insert(field, page);
    return f;
}

void JSField::clearCachedFields()
{
    if (g_fieldCache.exists()) {
        g_fieldCache->clear();
    }

    if (g_buttonCache.exists()) {
        g_buttonCache->clear();
    }
}
