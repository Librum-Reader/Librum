/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of TreeView.
**
** $QT_BEGIN_LICENSE:GPL-MARKETPLACE-QT$
**
** Marketplace License Usage
** Users, who have licensed the Software under the Qt Marketplace license
** agreement, may use this file in accordance with the Qt Marketplace license
** agreement provided with the Software or, alternatively, in accordance with
** the terms contained in a written agreement between the licensee and The Qt
** Company. For licensing terms and conditions see
** https://www.qt.io/terms-conditions/#marketplace and
** https://www.qt.io/terms-conditions. For further information use the contact
** form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlextensionplugin.h>

QT_BEGIN_NAMESPACE

void qml_register_types_QtQuick_TreeView();

class QQuickTreeViewPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QQuickTreeViewPlugin(QObject *parent = nullptr) : QQmlExtensionPlugin(parent)
    {
        volatile auto registration = &qml_register_types_QtQuick_TreeView;
        Q_UNUSED(registration);
    }
    void initializeEngine(QQmlEngine *, const char *) override {};
    void registerTypes(const char *) override {};
};


QT_END_NAMESPACE

#include "qquicktreeviewplugin.moc"
