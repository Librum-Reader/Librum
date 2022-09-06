/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_FORM_P_H
#define OKULAR_FORM_P_H

#include "form.h"

#include <QString>

namespace Okular
{
class Action;
class FormField;

class FormFieldPrivate
{
public:
    explicit FormFieldPrivate(FormField::FieldType type);
    virtual ~FormFieldPrivate();

    FormFieldPrivate(const FormFieldPrivate &) = delete;
    FormFieldPrivate &operator=(const FormFieldPrivate &) = delete;

    void setDefault();

    virtual void setValue(const QString &) = 0;
    virtual QString value() const = 0;

    FormField::FieldType m_type;
    QString m_default;
    Action *m_activateAction;
    QHash<int, Action *> m_additionalActions;
    QHash<int, Action *> m_additionalAnnotActions;
    Page *m_page = nullptr;

    Q_DECLARE_PUBLIC(FormField)
    FormField *q_ptr;
};

}

#endif
