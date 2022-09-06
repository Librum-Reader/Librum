/*
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FAXDOCUMENT_H
#define FAXDOCUMENT_H

#include <QImage>

/**
 * Loads a G3/G4 fax document and provides methods
 * to convert it into a QImage.
 */
class FaxDocument
{
public:
    /**
     * Describes the type of the fax document.
     */
    enum DocumentType {
        G3, ///< G3 encoded fax document
        G4  ///< G4 encoded fax document
    };

    /**
     * Creates a new fax document from the given @p fileName.
     *
     * @param fileName The name of the fax file.
     * @param type The type of the fax document.
     */
    explicit FaxDocument(const QString &fileName, DocumentType type = G3);

    /**
     * Destroys the fax document.
     */
    ~FaxDocument();

    FaxDocument(const FaxDocument &) = delete;
    FaxDocument &operator=(const FaxDocument &) = delete;

    /**
     * Loads the document.
     *
     * @return @c true if the document can be loaded successfully, @c false otherwise.
     */
    bool load();

    /**
     * Returns the document as an image.
     */
    QImage image() const;

private:
    class Private;
    Private *const d;
};

#endif
