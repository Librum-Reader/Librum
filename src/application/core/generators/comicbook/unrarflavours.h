/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef UNRARFLAVOURS_H
#define UNRARFLAVOURS_H

#include <QString>
#include <QStringList>

struct ProcessArgs {
    ProcessArgs();
    ProcessArgs(const QStringList &appArgs, bool useLsar);

    QStringList appArgs;
    bool useLsar;
};

class QStringList;

class UnrarFlavour
{
public:
    virtual ~UnrarFlavour();

    UnrarFlavour(const UnrarFlavour &) = delete;
    UnrarFlavour &operator=(const UnrarFlavour &) = delete;

    virtual QStringList processListing(const QStringList &data) = 0;
    virtual QString name() const = 0;

    virtual ProcessArgs processListArgs(const QString &fileName) const = 0;
    virtual ProcessArgs processOpenArchiveArgs(const QString &fileName, const QString &path) const = 0;

    void setFileName(const QString &fileName);

protected:
    UnrarFlavour();

    QString fileName() const;

private:
    QString mFileName;
};

class NonFreeUnrarFlavour : public UnrarFlavour
{
public:
    NonFreeUnrarFlavour();

    QStringList processListing(const QStringList &data) override;
    QString name() const override;

    ProcessArgs processListArgs(const QString &fileName) const override;
    ProcessArgs processOpenArchiveArgs(const QString &fileName, const QString &path) const override;
};

class FreeUnrarFlavour : public UnrarFlavour
{
public:
    FreeUnrarFlavour();

    QStringList processListing(const QStringList &data) override;
    QString name() const override;

    ProcessArgs processListArgs(const QString &fileName) const override;
    ProcessArgs processOpenArchiveArgs(const QString &fileName, const QString &path) const override;
};

class UnarFlavour : public UnrarFlavour
{
public:
    UnarFlavour();

    QStringList processListing(const QStringList &data) override;
    QString name() const override;

    ProcessArgs processListArgs(const QString &fileName) const override;
    ProcessArgs processOpenArchiveArgs(const QString &fileName, const QString &path) const override;
};

#endif
