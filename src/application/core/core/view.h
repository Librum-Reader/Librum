/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_VIEW_H
#define OKULAR_VIEW_H

#include <QObject>
#include "okularcore_export.h"
class QString;
class QVariant;

namespace Okular
{
class Document;
class DocumentPrivate;
class ViewPrivate;

/**
 * @short View on the document
 *
 * The View class represents a "view" on a document.
 * A view can be registered with only a document at a time.
 *
 * @since 0.7 (KDE 4.1)
 */
class OKULARCORE_EXPORT View
{
    /// @cond PRIVATE
    friend class Document;
    friend class DocumentPrivate;
    /// @endcond

public:
    /**
     * The capabilities of a view
     */
    enum ViewCapability
    {
        Zoom,  ///< Possibility to get/set the zoom of the view
        ZoomModality,  ///< Possibility to get/set the zoom mode of the view
        Continuous,  ///< Possibility to toggle continuous mode @since 1.9
        ViewModeModality,  ///< Possibility to get/set the view mode @since 1.9
        TrimMargins  ///< Possibility to toggle trim-margins mode @since 1.9
    };

    /**
     * The access type of a capability
     */
    enum CapabilityFlag
    {
        NoFlag = 0,
        CapabilityRead = 0x01,  ///< Possibility to read a capability
        CapabilityWrite = 0x02,  ///< Possibility to write a capability
        CapabilitySerializable = 0x04  ///< The capability is suitable for being
                                       ///< serialized/deserialized
    };
    Q_DECLARE_FLAGS(CapabilityFlags, CapabilityFlag)

    virtual ~View();

    /**
     * Return the document which this view is associated to,
     * or null if it is not associated with any document.
     */
    Document* viewDocument() const;

    /**
     * Return the name of this view.
     */
    QString name() const;

    /**
     * Query whether the view support the specified @p capability.
     */
    virtual bool supportsCapability(ViewCapability capability) const;

    /**
     * Query the flags for the specified @p capability.
     */
    virtual CapabilityFlags capabilityFlags(ViewCapability capability) const;

    /**
     * Query the value of the specified @p capability.
     */
    virtual QVariant capability(ViewCapability capability) const;

    /**
     * Sets a new value for the specified @p capability.
     */
    virtual void setCapability(ViewCapability capability,
                               const QVariant& option);

protected:
    /**
     * Construct a new view with the specified @p name.
     */
    explicit View(const QString& name);

    /// @cond PRIVATE
    Q_DECLARE_PRIVATE(View)
    ViewPrivate* d_ptr;
    /// @endcond

private:
    Q_DISABLE_COPY(View)
};

}  // namespace Okular

Q_DECLARE_OPERATORS_FOR_FLAGS(Okular::View::CapabilityFlags)

#endif
