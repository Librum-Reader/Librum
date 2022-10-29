/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PRINTINTERFACE_H_
#define _OKULAR_PRINTINTERFACE_H_

#include <QObject>
#include "../core/okularcore_export.h"

class QWidget;

namespace Okular
{
/**
 * @short Abstract interface for advanced printing control
 *
 * This interface defines an advanced way of interfacing with the print
 * process.
 *
 * How to use it in a custom Generator:
 * @code
    class MyGenerator : public Okular::Generator, public Okular::PrintInterface
    {
        Q_OBJECT
        Q_INTERFACES( Okular::PrintInterface )

        ...
    };
 * @endcode
 * and - of course - implementing its methods.
 */
class OKULARCORE_EXPORT PrintInterface
{
public:
    PrintInterface()
    {
    }

    /**
     * Destroys the printer interface.
     */
    virtual ~PrintInterface()
    {
    }

    PrintInterface(const PrintInterface&) = delete;
    PrintInterface& operator=(const PrintInterface&) = delete;

    /**
     * Builds and returns a new printing configuration widget.
     *
     * @note don't keep a pointer to the new constructed widget, as it
     * will be handled elsewhere (in the Okular KPart)
     *
     * @note The returned object should be of a PrintOptionsWidget subclass
     * (which is not officially enforced by the signature for binary
     * compatibility reasons).
     */
    virtual QWidget* printConfigurationWidget() const = 0;
};

}  // namespace Okular

Q_DECLARE_INTERFACE(Okular::PrintInterface, "org.kde.okular.PrintInterface/0.1")

#endif
