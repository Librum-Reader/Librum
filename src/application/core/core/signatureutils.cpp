/*
    SPDX-FileCopyrightText: 2018 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "signatureutils.h"

using namespace Okular;

CertificateInfo::CertificateInfo()
{
}

CertificateInfo::~CertificateInfo()
{
}

Q_DECLARE_OPERATORS_FOR_FLAGS(CertificateInfo::KeyUsageExtensions)

bool CertificateInfo::isNull() const
{
    return true;
}

int CertificateInfo::version() const
{
    return -1;
}

QByteArray CertificateInfo::serialNumber() const
{
    return QByteArray();
}

QString CertificateInfo::issuerInfo(EntityInfoKey) const
{
    return QString();
}

QString CertificateInfo::subjectInfo(EntityInfoKey) const
{
    return QString();
}

QString CertificateInfo::nickName() const
{
    return QString();
}

QDateTime CertificateInfo::validityStart() const
{
    return QDateTime();
}

QDateTime CertificateInfo::validityEnd() const
{
    return QDateTime();
}

CertificateInfo::KeyUsageExtensions CertificateInfo::keyUsageExtensions() const
{
    return KuNone;
}

QByteArray CertificateInfo::publicKey() const
{
    return QByteArray();
}

CertificateInfo::PublicKeyType CertificateInfo::publicKeyType() const
{
    return OtherKey;
}

int CertificateInfo::publicKeyStrength() const
{
    return -1;
}

bool CertificateInfo::isSelfSigned() const
{
    return false;
}

QByteArray CertificateInfo::certificateData() const
{
    return QByteArray();
}

bool CertificateInfo::checkPassword(const QString& /*password*/) const
{
    return false;
}

SignatureInfo::SignatureInfo()
{
}

SignatureInfo::~SignatureInfo()
{
}

SignatureInfo::SignatureStatus SignatureInfo::signatureStatus() const
{
    return SignatureStatusUnknown;
}

SignatureInfo::CertificateStatus SignatureInfo::certificateStatus() const
{
    return CertificateStatusUnknown;
}

SignatureInfo::HashAlgorithm SignatureInfo::hashAlgorithm() const
{
    return HashAlgorithmUnknown;
}

QString SignatureInfo::signerName() const
{
    return QString();
}

QString SignatureInfo::signerSubjectDN() const
{
    return QString();
}

QString SignatureInfo::location() const
{
    return QString();
}

QString SignatureInfo::reason() const
{
    return QString();
}

QDateTime SignatureInfo::signingTime() const
{
    return QDateTime();
}

QByteArray SignatureInfo::signature() const
{
    return QByteArray();
}

QList<qint64> SignatureInfo::signedRangeBounds() const
{
    return QList<qint64>();
}

bool SignatureInfo::signsTotalDocument() const
{
    return false;
}

const CertificateInfo& SignatureInfo::certificateInfo() const
{
    static CertificateInfo dummy;
    return dummy;
}

CertificateStore::CertificateStore()
{
}

CertificateStore::~CertificateStore()
{
}

QList<CertificateInfo*> CertificateStore::signingCertificates(
    bool* userCancelled) const
{
    *userCancelled = false;
    return QList<CertificateInfo*>();
}

QList<CertificateInfo*> CertificateStore::signingCertificatesForNow(
    bool* userCancelled, bool* nonDateValidCerts) const
{
    const QDateTime now = QDateTime::currentDateTime();
    QList<Okular::CertificateInfo*> certs = signingCertificates(userCancelled);
    auto it = certs.begin();
    *nonDateValidCerts = false;
    while(it != certs.end())
    {
        if((*it)->validityStart() > now || now > (*it)->validityEnd())
        {
            it = certs.erase(it);
            *nonDateValidCerts = true;
        }
        else
        {
            ++it;
        }
    }
    return certs;
}
