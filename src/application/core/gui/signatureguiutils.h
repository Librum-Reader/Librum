/*
    SPDX-FileCopyrightText: 2018 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SIGNATUREGUIUTILS_H
#define OKULAR_SIGNATUREGUIUTILS_H

#include <QString>

#include "core/signatureutils.h"

#include <KMessageWidget>

namespace Okular
{
class Document;
class FormFieldSignature;
}

namespace SignatureGuiUtils
{
/**
 * Returns a vector containing signature form fields sorted by date (last is newer).
 */
QVector<const Okular::FormFieldSignature *> getSignatureFormFields(const Okular::Document *doc);
QString getReadableSignatureStatus(Okular::SignatureInfo::SignatureStatus sigStatus);
QString getReadableCertStatus(Okular::SignatureInfo::CertificateStatus certStatus);
QString getReadableHashAlgorithm(Okular::SignatureInfo::HashAlgorithm hashAlg);
QString getReadablePublicKeyType(Okular::CertificateInfo::PublicKeyType type);
QString getReadableKeyUsageCommaSeparated(Okular::CertificateInfo::KeyUsageExtensions kuExtensions);
QString getReadableKeyUsageNewLineSeparated(Okular::CertificateInfo::KeyUsageExtensions kuExtensions);
QString getReadableModificationSummary(const Okular::SignatureInfo &signatureInfo);

std::pair<KMessageWidget::MessageType, QString> documentSignatureMessageWidgetText(const Okular::Document *doc);
}

#endif
