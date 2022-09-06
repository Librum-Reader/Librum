/*
    SPDX-FileCopyrightText: 2018 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_CERTIFICATEMODEL_H
#define OKULAR_CERTIFICATEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

#include "core/signatureutils.h"

class CertificateModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CertificateModel(const Okular::CertificateInfo &certInfo, QObject *parent = nullptr);

    enum { PropertyKeyRole = Qt::UserRole, PropertyVisibleValueRole };

    enum Property { Version, SerialNumber, Issuer, IssuedOn, ExpiresOn, Subject, PublicKey, KeyUsage, IssuerName, IssuerEmail, IssuerOrganization, SubjectName, SubjectEmail, SubjectOrganization, Sha1, Sha256 };
    Q_ENUM(Property)

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Q_INVOKABLE QString propertyVisibleValue(CertificateModel::Property p) const;
    Q_INVOKABLE bool exportCertificateTo(const QString &path);

private:
    QVector<Property> m_certificateProperties;
    const Okular::CertificateInfo &m_certificateInfo;
};

#endif
