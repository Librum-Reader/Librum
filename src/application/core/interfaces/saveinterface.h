/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_SAVEINTERFACE_H_
#define _OKULAR_SAVEINTERFACE_H_

#include <QObject>
#include "../core/okularcore_export.h"

namespace Okular
{
class AnnotationProxy;

/**
 * @short Abstract interface for saving
 *
 * This interface defines a way to save (or help saving) the document opened
 * by the Generator.
 *
 * How to use it in a custom Generator:
 * @code
    class MyGenerator : public Okular::Generator, public Okular::SaveInterface
    {
        Q_OBJECT
        Q_INTERFACES( Okular::SaveInterface )

        ...
    };
 * @endcode
 * and - of course - implementing its methods.
 */
class OKULARCORE_EXPORT SaveInterface
{
public:
    /**
     * The possible options for the saving.
     */
    enum SaveOption
    {
        NoOption = 0,
        SaveChanges = 1  ///< The possibility to save with the current changes
                         ///< to the document.
    };
    Q_DECLARE_FLAGS(SaveOptions, SaveOption)

    SaveInterface()
    {
    }

    /**
     * Destroys the save interface.
     */
    virtual ~SaveInterface()
    {
    }

    SaveInterface(const SaveInterface&) = delete;
    SaveInterface& operator=(const SaveInterface&) = delete;

    /**
     * Query for the supported saving options.
     *
     * @note NoOption is never queried
     */
    virtual bool supportsOption(SaveOption option) const = 0;

    /**
     * Save to the specified @p fileName with the specified @p options.
     */
    virtual bool save(const QString& fileName, SaveOptions options,
                      QString* errorText) = 0;

    /**
     * Returns the annotation proxy. Generators can return NULL if native
     * annotations are not supported.
     *
     * @note Returning NULL is equivalent to returning an AnnotationProxy
     *       that doesn't support any capability.
     * @since 0.15 (KDE 4.9)
     */
    virtual AnnotationProxy* annotationProxy() const = 0;
};

}  // namespace Okular

Q_DECLARE_INTERFACE(Okular::SaveInterface, "org.kde.okular.SaveInterface/0.3")
Q_DECLARE_OPERATORS_FOR_FLAGS(Okular::SaveInterface::SaveOptions)

#endif
