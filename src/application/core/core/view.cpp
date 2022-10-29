/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "view.h"
#include "view_p.h"

// local includes
#include "document_p.h"

using namespace Okular;

ViewPrivate::ViewPrivate() :
    document(nullptr)
{
}

ViewPrivate::~ViewPrivate()
{
}

View::View(const QString& name) :
    d_ptr(new ViewPrivate())
{
    d_ptr->name = name;
}

View::~View()
{
    if(d_ptr->document)
    {
        d_ptr->document->m_views.remove(this);
    }

    delete d_ptr;
}

Document* View::viewDocument() const
{
    return d_ptr->document ? d_ptr->document->m_parent : nullptr;
}

QString View::name() const
{
    return d_ptr->name;
}

bool View::supportsCapability(View::ViewCapability capability) const
{
    Q_UNUSED(capability)
    return false;
}

View::CapabilityFlags View::capabilityFlags(
    View::ViewCapability capability) const
{
    Q_UNUSED(capability)
    return NoFlag;
}

QVariant View::capability(View::ViewCapability capability) const
{
    Q_UNUSED(capability)
    return QVariant();
}

void View::setCapability(View::ViewCapability capability,
                         const QVariant& option)
{
    Q_UNUSED(capability)
    Q_UNUSED(option)
}
