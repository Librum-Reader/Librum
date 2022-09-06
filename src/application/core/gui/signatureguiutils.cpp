/*
    SPDX-FileCopyrightText: 2018 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "signatureguiutils.h"

#include <KLocalizedString>

#include "core/document.h"
#include "core/form.h"
#include "core/page.h"

namespace SignatureGuiUtils
{
QVector<const Okular::FormFieldSignature *> getSignatureFormFields(const Okular::Document *doc)
{
    uint curPage = 0;
    const uint endPage = doc->pages() - 1;
    QVector<const Okular::FormFieldSignature *> signatureFormFields;
    while (curPage <= endPage) {
        const QList<Okular::FormField *> formFields = doc->page(curPage++)->formFields();
        for (Okular::FormField *f : formFields) {
            if (f->type() == Okular::FormField::FormSignature) {
                signatureFormFields.append(static_cast<Okular::FormFieldSignature *>(f));
            }
        }
    }
    std::sort(signatureFormFields.begin(), signatureFormFields.end(), [](const Okular::FormFieldSignature *a, const Okular::FormFieldSignature *b) {
        const Okular::SignatureInfo &infoA = a->signatureInfo();
        const Okular::SignatureInfo &infoB = b->signatureInfo();
        return infoA.signingTime() < infoB.signingTime();
    });
    return signatureFormFields;
}

QString getReadableSignatureStatus(Okular::SignatureInfo::SignatureStatus sigStatus)
{
    switch (sigStatus) {
    case Okular::SignatureInfo::SignatureValid:
        return i18n("The signature is cryptographically valid.");
    case Okular::SignatureInfo::SignatureInvalid:
        return i18n("The signature is cryptographically invalid.");
    case Okular::SignatureInfo::SignatureDigestMismatch:
        return i18n("Digest Mismatch occurred.");
    case Okular::SignatureInfo::SignatureDecodingError:
        return i18n("The signature CMS/PKCS7 structure is malformed.");
    case Okular::SignatureInfo::SignatureNotFound:
        return i18n("The requested signature is not present in the document.");
    default:
        return i18n("The signature could not be verified.");
    }
}

QString getReadableCertStatus(Okular::SignatureInfo::CertificateStatus certStatus)
{
    switch (certStatus) {
    case Okular::SignatureInfo::CertificateTrusted:
        return i18n("Certificate is Trusted.");
    case Okular::SignatureInfo::CertificateUntrustedIssuer:
        return i18n("Certificate issuer isn't Trusted.");
    case Okular::SignatureInfo::CertificateUnknownIssuer:
        return i18n("Certificate issuer is unknown.");
    case Okular::SignatureInfo::CertificateRevoked:
        return i18n("Certificate has been Revoked.");
    case Okular::SignatureInfo::CertificateExpired:
        return i18n("Certificate has Expired.");
    case Okular::SignatureInfo::CertificateNotVerified:
        return i18n("Certificate has not yet been verified.");
    default:
        return i18n("Unknown issue with Certificate or corrupted data.");
    }
}

QString getReadableHashAlgorithm(Okular::SignatureInfo::HashAlgorithm hashAlg)
{
    switch (hashAlg) {
    case Okular::SignatureInfo::HashAlgorithmMd2:
        return i18n("MD2");
    case Okular::SignatureInfo::HashAlgorithmMd5:
        return i18n("MD5");
    case Okular::SignatureInfo::HashAlgorithmSha1:
        return i18n("SHA1");
    case Okular::SignatureInfo::HashAlgorithmSha256:
        return i18n("SHA256");
    case Okular::SignatureInfo::HashAlgorithmSha384:
        return i18n("SHA384");
    case Okular::SignatureInfo::HashAlgorithmSha512:
        return i18n("SHA512");
    case Okular::SignatureInfo::HashAlgorithmSha224:
        return i18n("SHA224");
    default:
        return i18n("Unknown Algorithm");
    }
}

QString getReadablePublicKeyType(Okular::CertificateInfo::PublicKeyType type)
{
    switch (type) {
    case Okular::CertificateInfo::RsaKey:
        return i18n("RSA");
    case Okular::CertificateInfo::DsaKey:
        return i18n("DSA");
    case Okular::CertificateInfo::EcKey:
        return i18n("EC");
    case Okular::CertificateInfo::OtherKey:
        return i18n("Unknown Type");
    }

    return i18n("Unknown Type");
}

QString getReadableKeyUsage(Okular::CertificateInfo::KeyUsageExtensions kuExtensions, const QString &separator)
{
    QStringList ku;
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuDigitalSignature)) {
        ku << i18n("Digital Signature");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuNonRepudiation)) {
        ku << i18n("Non-Repudiation");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuKeyEncipherment)) {
        ku << i18n("Encrypt Keys");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuDataEncipherment)) {
        ku << i18n("Decrypt Keys");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuKeyAgreement)) {
        ku << i18n("Key Agreement");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuKeyCertSign)) {
        ku << i18n("Sign Certificate");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuClrSign)) {
        ku << i18n("Sign CRL");
    }
    if (kuExtensions.testFlag(Okular::CertificateInfo::KuEncipherOnly)) {
        ku << i18n("Encrypt Only");
    }
    if (ku.isEmpty()) {
        ku << i18n("No Usage Specified");
    }
    return ku.join(separator);
}

QString getReadableKeyUsageCommaSeparated(Okular::CertificateInfo::KeyUsageExtensions kuExtensions)
{
    return getReadableKeyUsage(kuExtensions, i18nc("Joins the various ways a signature key can be used in a longer string", ", "));
}

QString getReadableKeyUsageNewLineSeparated(Okular::CertificateInfo::KeyUsageExtensions kuExtensions)
{
    return getReadableKeyUsage(kuExtensions, QStringLiteral("\n"));
}

QString getReadableModificationSummary(const Okular::SignatureInfo &signatureInfo)
{
    const Okular::SignatureInfo::SignatureStatus signatureStatus = signatureInfo.signatureStatus();
    // signature validation status
    if (signatureStatus == Okular::SignatureInfo::SignatureValid) {
        if (signatureInfo.signsTotalDocument()) {
            return i18n("The document has not been modified since it was signed.");
        } else {
            return i18n(
                "The revision of the document that was covered by this signature has not been modified;\n"
                "however there have been subsequent changes to the document.");
        }
    } else if (signatureStatus == Okular::SignatureInfo::SignatureDigestMismatch) {
        return i18n("The document has been modified in a way not permitted by a previous signer.");
    } else {
        return i18n("The document integrity verification could not be completed.");
    }
}

std::pair<KMessageWidget::MessageType, QString> documentSignatureMessageWidgetText(const Okular::Document *doc)
{
    const uint numPages = doc->pages();
    bool isDigitallySigned = false;
    for (uint i = 0; i < numPages; i++) {
        const QList<Okular::FormField *> formFields = doc->page(i)->formFields();
        for (const Okular::FormField *f : formFields) {
            if (f->type() == Okular::FormField::FormSignature) {
                isDigitallySigned = true;
            }
        }
    }

    if (isDigitallySigned) {
        const QVector<const Okular::FormFieldSignature *> signatureFormFields = SignatureGuiUtils::getSignatureFormFields(doc);
        bool allSignaturesValid = true;
        bool anySignatureUnsigned = false;
        for (const Okular::FormFieldSignature *signature : signatureFormFields) {
            if (signature->signatureType() == Okular::FormFieldSignature::UnsignedSignature) {
                anySignatureUnsigned = true;
            } else {
                const Okular::SignatureInfo &info = signature->signatureInfo();
                if (info.signatureStatus() != Okular::SignatureInfo::SignatureValid) {
                    allSignaturesValid = false;
                }
            }
        }

        if (anySignatureUnsigned) {
            return {KMessageWidget::Information, i18n("This document has unsigned signature fields.")};
        } else if (allSignaturesValid) {
            if (signatureFormFields.last()->signatureInfo().signsTotalDocument()) {
                return {KMessageWidget::Information, i18n("This document is digitally signed.")};
            } else {
                return {KMessageWidget::Warning, i18n("This document is digitally signed. There have been changes since last signed.")};
            }
        } else {
            return {KMessageWidget::Warning, i18n("This document is digitally signed. Some of the signatures could not be validated properly.")};
        }
    }

    return {KMessageWidget::Information, QString()};
}
}
