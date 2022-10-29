/*
    SPDX-FileCopyrightText: 2019 Bubli <Katarina.Behrens@cib.de>
    SPDX-FileCopyrightText: 2020 Albert Astals Cid <albert.astals.cid@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pdfsettingswidget.h"
#include <KLocalizedString>
#include <KUrlRequester>
#include <QEvent>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "pdfsettings.h"
#include "pdfsignatureutils.h"

PDFSettingsWidget::PDFSettingsWidget(QWidget* parent) :
    QWidget(parent)
{
    m_pdfsw.setupUi(this);
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 1, 0)
    if(Poppler::hasNSSSupport())
    {
        m_pdfsw.loadSignaturesButton->hide();

        KUrlRequester* pDlg = new KUrlRequester();
        pDlg->setObjectName(QStringLiteral("kcfg_DBCertificatePath"));
        pDlg->setMode(KFile::Directory | KFile::ExistingOnly |
                      KFile::LocalOnly);
        pDlg->setEnabled(false);
        m_pdfsw.formLayout->setWidget(1, QFormLayout::FieldRole, pDlg);

        connect(m_pdfsw.customRadioButton, &QRadioButton::toggled, pDlg,
                &KUrlRequester::setEnabled);

        if(!PDFSettings::useDefaultCertDB())
        {
            m_pdfsw.customRadioButton->setChecked(true);
            m_pdfsw.defaultLabel->setVisible(false);
        }

        m_tree = new QTreeWidget(this);
        m_tree->setHeaderLabels(
            { i18nc("Name of the person to whom the cerficate was issued",
                    "Issued to"),
              i18n("E-mail"),
              i18nc("Certificate expiration date", "Expiration date") });
        m_tree->setRootIsDecorated(false);

        m_pdfsw.certificatesPlaceholder->addWidget(m_tree);

        connect(PDFSettings::self(), &PDFSettings::useDefaultDBChanged, this,
                &PDFSettingsWidget::warnRestartNeeded);
        connect(PDFSettings::self(), &PDFSettings::dBCertificatePathChanged,
                this,
                [this]
                {
                    if(!PDFSettings::useDefaultCertDB())
                    {
                        warnRestartNeeded();
                    }
                });
        connect(m_pdfsw.loadSignaturesButton, &QPushButton::clicked, this,
                [this]
                {
                    m_certificatesAsked = false;
                    update();
                });
    }
    else
    {
        QHBoxLayout* lay = new QHBoxLayout(this);
        QLabel* l =
            new QLabel(i18n("You are using a Poppler library built without NSS "
                            "support.\nAdding Digital Signatures isn't "
                            "available for that reason"));
        l->setWordWrap(true);
        lay->addWidget(l);
    }
#else
    m_pdfsw.certDBGroupBox->hide();
    m_pdfsw.certificatesGroup->hide();
    m_pdfsw.loadSignaturesButton->hide();
#endif

#if POPPLER_VERSION_MACRO < QT_VERSION_CHECK(21, 10, 0)
    m_pdfsw.kcfg_CheckOCSPServers->hide();
#endif
}

bool PDFSettingsWidget::event(QEvent* e)
{
    if(m_tree && e->type() == QEvent::Paint && !m_certificatesAsked)
    {
        m_certificatesAsked = true;

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 1, 0)
        PopplerCertificateStore st;
        bool userCancelled;
        const QList<Okular::CertificateInfo*> certs =
            st.signingCertificates(&userCancelled);

        m_pdfsw.loadSignaturesButton->setVisible(userCancelled);

        for(auto cert : certs)
        {
            new QTreeWidgetItem(
                m_tree,
                { cert->subjectInfo(
                      Okular::CertificateInfo::EntityInfoKey::CommonName),
                  cert->subjectInfo(
                      Okular::CertificateInfo::EntityInfoKey::EmailAddress),
                  cert->validityEnd().toString(QStringLiteral("yyyy-MM-dd")) });
        }
        qDeleteAll(certs);

        m_pdfsw.defaultLabel->setText(Poppler::getNSSDir());

        m_tree->resizeColumnToContents(1);
        m_tree->resizeColumnToContents(0);
#endif
    }
    return QWidget::event(e);
}

void PDFSettingsWidget::warnRestartNeeded()
{
    if(!m_warnedAboutRestart)
    {
        m_warnedAboutRestart = true;
        QMessageBox::information(this, i18n("Restart needed"),
                                 i18n("You need to restart Okular after "
                                      "changing the NSS directory settings"));
    }
}
