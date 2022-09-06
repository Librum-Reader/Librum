/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GUIINTERFACE_H_
#define _OKULAR_GUIINTERFACE_H_

#include "../core/okularcore_export.h"

#include <QObject>

#include <KXMLGUIClient>

namespace Okular
{
/**
 * @short Abstract interface for user interface control
 *
 * This interface defines an way to interact with the Okular user interface,
 * e.g. adding actions in the menus.
 *
 * How to use it in a custom Generator:
 * @code
    class MyGenerator : public Okular::Generator, public Okular::GuiInterface
    {
        Q_OBJECT
        Q_INTERFACES( Okular::GuiInterface )

        ...
    };
 * @endcode
 * and - of course - implementing its methods.
 */
class OKULARCORE_EXPORT GuiInterface : protected KXMLGUIClient
{
public:
    GuiInterface()
    {
    }

    /**
     * Destroys the gui interface.
     */
    ~GuiInterface() override
    {
    }

    GuiInterface(const GuiInterface &) = delete;
    GuiInterface &operator=(const GuiInterface &) = delete;

    /**
     * This method requests the XML GUI Client provided by the interface.
     */
    KXMLGUIClient *guiClient()
    {
        return this;
    }
};

}

Q_DECLARE_INTERFACE(Okular::GuiInterface, "org.kde.okular.GuiInterface/0.1")

#endif
