/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>
    SPDX-FileCopyrightText: 2011 David Palacio <dpalacio@orbitalibre.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QStringList>

class QIODevice;

class Directory
{
public:
    /**
     * Creates a new directory object.
     */
    Directory();

    /**
     * Destroys the directory object.
     */
    ~Directory();

    /**
     * Opens given directory.
     */
    bool open(const QString& dirName);

    /**
     * Returns the list of files from the directory.
     */
    QStringList list() const;

    /**
     * Returns a new device for reading the file with the given path.
     */
    QIODevice* createDevice(const QString& path) const;

private:
    /**
     * Iterates over a directory and returns a file list.
     */
    QStringList recurseDir(const QString& dir, int curDepth) const;

    static const int staticMaxDepth = 1;
    QString mDir;
};

#endif
