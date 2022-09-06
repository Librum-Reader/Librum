/*
    SPDX-FileCopyrightText: 2018 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "certificatemodel.h"

#include "signatureguiutils.h"

#include <KLocalizedString>

#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QUrl>

CertificateModel::CertificateModel(const Okular::CertificateInfo &certInfo, QObject *parent)
    : QAbstractTableModel(parent)
    , m_certificateInfo(certInfo)
{
    m_certificateProperties = {Version, SerialNumber, Issuer, IssuedOn, ExpiresOn, Subject, PublicKey, KeyUsage};
}

int CertificateModel::columnCount(const QModelIndex &) const
{
    return 2;
}

int CertificateModel::rowCount(const QModelIndex &) const
{
    return m_certificateProperties.size();
}

static QString propertyVisibleName(CertificateModel::Property p)
{
    switch (p) {
    case CertificateModel::Version:
        return i18n("Version");
    case CertificateModel::SerialNumber:
        return i18n("Serial Number");
    case CertificateModel::Issuer:
        return i18n("Issuer");
    case CertificateModel::IssuedOn:
        return i18n("Issued On");
    case CertificateModel::ExpiresOn:
        return i18n("Expires On");
    case CertificateModel::Subject:
        return i18nc("The person/company that made the signature", "Subject");
    case CertificateModel::PublicKey:
        return i18n("Public Key");
    case CertificateModel::KeyUsage:
        return i18n("Key Usage");
    case CertificateModel::IssuerName:
    case CertificateModel::IssuerEmail:
    case CertificateModel::IssuerOrganization:
    case CertificateModel::SubjectName:
    case CertificateModel::SubjectEmail:
    case CertificateModel::SubjectOrganization:
    case CertificateModel::Sha1:
    case CertificateModel::Sha256:
        Q_ASSERT(false);
        qWarning() << "Unimplemented";
    }
    return QString();
}

QString CertificateModel::propertyVisibleValue(CertificateModel::Property p) const
{
    switch (p) {
    case CertificateModel::Version:
        return i18n("V%1", QString::number(m_certificateInfo.version()));
    case CertificateModel::SerialNumber:
        return QString::fromLatin1(m_certificateInfo.serialNumber().toHex(' '));
    case CertificateModel::Issuer:
        return m_certificateInfo.issuerInfo(Okular::CertificateInfo::DistinguishedName);
    case CertificateModel::IssuedOn:
        return m_certificateInfo.validityStart().toString(Qt::DefaultLocaleLongDate);
    case CertificateModel::ExpiresOn:
        return m_certificateInfo.validityEnd().toString(Qt::DefaultLocaleLongDate);
    case CertificateModel::Subject:
        return m_certificateInfo.subjectInfo(Okular::CertificateInfo::DistinguishedName);
    case CertificateModel::PublicKey:
        return i18n("%1 (%2 bits)", SignatureGuiUtils::getReadablePublicKeyType(m_certificateInfo.publicKeyType()), m_certificateInfo.publicKeyStrength());
    case CertificateModel::KeyUsage:
        return SignatureGuiUtils::getReadableKeyUsageCommaSeparated(m_certificateInfo.keyUsageExtensions());
    case CertificateModel::IssuerName:
        return m_certificateInfo.issuerInfo(Okular::CertificateInfo::CommonName);
    case CertificateModel::IssuerEmail:
        return m_certificateInfo.issuerInfo(Okular::CertificateInfo::EmailAddress);
    case CertificateModel::IssuerOrganization:
        return m_certificateInfo.issuerInfo(Okular::CertificateInfo::Organization);
    case CertificateModel::SubjectName:
        return m_certificateInfo.subjectInfo(Okular::CertificateInfo::CommonName);
    case CertificateModel::SubjectEmail:
        return m_certificateInfo.subjectInfo(Okular::CertificateInfo::EmailAddress);
    case CertificateModel::SubjectOrganization:
        return m_certificateInfo.subjectInfo(Okular::CertificateInfo::Organization);
    case CertificateModel::Sha1:
        return QString::fromLatin1(QCryptographicHash::hash(m_certificateInfo.certificateData(), QCryptographicHash::Sha1).toHex(' '));
    case CertificateModel::Sha256:
        return QString::fromLatin1(QCryptographicHash::hash(m_certificateInfo.certificateData(), QCryptographicHash::Sha256).toHex(' '));
    }
    return QString();
}

QVariant CertificateModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (!index.isValid() || row < 0 || row >= m_certificateProperties.count()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (index.column()) {
        case 0:
            return propertyVisibleName(m_certificateProperties[row]);
        case 1:
            return propertyVisibleValue(m_certificateProperties[row]);
        default:
            return QString();
        }
    case PropertyKeyRole:
        return m_certificateProperties[row];
    case PropertyVisibleValueRole:
        return propertyVisibleValue(m_certificateProperties[row]);
    }

    return QVariant();
}

QVariant CertificateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft);
    }

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (section) {
    case 0:
        return i18n("Property");
    case 1:
        return i18n("Value");
    default:
        return QVariant();
    }
}

bool CertificateModel::exportCertificateTo(const QString &path)
{
    const QUrl url = QUrl::fromUserInput(path);
    if (!url.isLocalFile()) {
        return false;
    }
    QFile targetFile(url.toLocalFile());
    if (!targetFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    const QByteArray data = m_certificateInfo.certificateData();
    return targetFile.write(data) == data.size();
}
