/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_CONFIGINTERFACE_H_
#define _OKULAR_CONFIGINTERFACE_H_

#include "../core/okularcore_export.h"

#include <QObject>

class KConfigDialog;

namespace Okular
{
/**
 * @short Abstract interface for configuration control
 *
 * This interface defines a way to configure the Generator itself.
 *
 * How to use it in a custom Generator:
 * @code
    class MyGenerator : public Okular::Generator, public Okular::ConfigInterface
    {
        Q_OBJECT
        Q_INTERFACES( Okular::ConfigInterface )

        ...
    };
 * @endcode
 * and - of course - implementing its methods.
 */
class OKULARCORE_EXPORT ConfigInterface
{
public:
    ConfigInterface()
    {
    }

    /**
     * Destroys the config interface.
     */
    virtual ~ConfigInterface()
    {
    }

    ConfigInterface(const ConfigInterface &) = delete;
    ConfigInterface &operator=(const ConfigInterface &) = delete;

    /**
     * This method is called to tell the generator to re-parse its configuration.
     *
     * Returns true if something has changed.
     *
     * @note this method can be called also when the generator is not the
     * active generator, or when there was not changed in the config added
     * by the generator itself. So the suggestion is to @b check whether
     * something changed, and only in that case return @p true
     */
    virtual bool reparseConfig() = 0;

    /**
     * This method allows the generator to add custom configuration pages to the
     * config @p dialog of okular.
     */
    virtual void addPages(KConfigDialog *dialog) = 0;
};

}

Q_DECLARE_INTERFACE(Okular::ConfigInterface, "org.kde.okular.ConfigInterface/0.1")

#endif
