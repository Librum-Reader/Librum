/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2018 Intevation GmbH <intevation@intevation.de>
    SPDX-FileCopyrightText: 2019 Oliver Sander <oliver.sander@tu-dresden.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "formfields.h"
#include <poppler-qt5.h>
#include "core/action.h"
#include "generator_pdf.h"
#include "pdfsettings.h"
#include "pdfsignatureutils.h"

extern Okular::Action* createLinkFromPopplerLink(
    const Poppler::Link* popplerLink, bool deletePopplerLink = true);
#define SET_ANNOT_ACTIONS                                                    \
    setAdditionalAction(Okular::Annotation::CursorEntering,                  \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::CursorEnteringAction)));    \
    setAdditionalAction(Okular::Annotation::CursorLeaving,                   \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::CursorLeavingAction)));     \
    setAdditionalAction(Okular::Annotation::MousePressed,                    \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::MousePressedAction)));      \
    setAdditionalAction(Okular::Annotation::MouseReleased,                   \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::MouseReleasedAction)));     \
    setAdditionalAction(Okular::Annotation::FocusIn,                         \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::FocusInAction)));           \
    setAdditionalAction(Okular::Annotation::FocusOut,                        \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::Annotation::FocusOutAction)));

#define SET_ACTIONS                                                          \
    setActivationAction(                                                     \
        createLinkFromPopplerLink(m_field->activationAction()));             \
    setAdditionalAction(Okular::FormField::FieldModified,                    \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::FormField::FieldModified)));            \
    setAdditionalAction(Okular::FormField::FormatField,                      \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::FormField::FormatField)));              \
    setAdditionalAction(Okular::FormField::ValidateField,                    \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::FormField::ValidateField)));            \
    setAdditionalAction(Okular::FormField::CalculateField,                   \
                        createLinkFromPopplerLink(m_field->additionalAction( \
                            Poppler::FormField::CalculateField)));           \
    SET_ANNOT_ACTIONS

PopplerFormFieldButton::PopplerFormFieldButton(
    std::unique_ptr<Poppler::FormFieldButton> field) :
    Okular::FormFieldButton(),
    m_field(std::move(field))
{
    m_rect = Okular::NormalizedRect::fromQRectF(m_field->rect());
    m_id = m_field->id();
    SET_ACTIONS
}

Okular::NormalizedRect PopplerFormFieldButton::rect() const
{
    return m_rect;
}

int PopplerFormFieldButton::id() const
{
    return m_id;
}

QString PopplerFormFieldButton::name() const
{
    return m_field->name();
}

QString PopplerFormFieldButton::uiName() const
{
    return m_field->uiName();
}

QString PopplerFormFieldButton::fullyQualifiedName() const
{
    return m_field->fullyQualifiedName();
}

bool PopplerFormFieldButton::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldButton::setReadOnly(bool value)
{
    m_field->setReadOnly(value);
}

bool PopplerFormFieldButton::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldButton::setVisible(bool value)
{
    m_field->setVisible(value);
}

bool PopplerFormFieldButton::isPrintable() const
{
    return m_field->isPrintable();
}

void PopplerFormFieldButton::setPrintable(bool value)
{
    m_field->setPrintable(value);
}

Okular::FormFieldButton::ButtonType PopplerFormFieldButton::buttonType() const
{
    switch(m_field->buttonType())
    {
    case Poppler::FormFieldButton::Push:
        return Okular::FormFieldButton::Push;
    case Poppler::FormFieldButton::CheckBox:
        return Okular::FormFieldButton::CheckBox;
    case Poppler::FormFieldButton::Radio:
        return Okular::FormFieldButton::Radio;
    }
    return Okular::FormFieldButton::Push;
}

QString PopplerFormFieldButton::caption() const
{
    return m_field->caption();
}

bool PopplerFormFieldButton::state() const
{
    return m_field->state();
}

void PopplerFormFieldButton::setState(bool state)
{
    m_field->setState(state);
}

QList<int> PopplerFormFieldButton::siblings() const
{
    return m_field->siblings();
}

Poppler::FormFieldIcon PopplerFormFieldButton::icon() const
{
    return m_field->icon();
}

void PopplerFormFieldButton::setIcon(Okular::FormField* field)
{
    if(field->type() == Okular::FormField::FormButton)
    {
        PopplerFormFieldButton* button =
            static_cast<PopplerFormFieldButton*>(field);
        m_field->setIcon(button->icon());
    }
}

PopplerFormFieldText::PopplerFormFieldText(
    std::unique_ptr<Poppler::FormFieldText> field) :
    Okular::FormFieldText(),
    m_field(std::move(field))
{
    m_rect = Okular::NormalizedRect::fromQRectF(m_field->rect());
    m_id = m_field->id();
    SET_ACTIONS
}

Okular::NormalizedRect PopplerFormFieldText::rect() const
{
    return m_rect;
}

int PopplerFormFieldText::id() const
{
    return m_id;
}

QString PopplerFormFieldText::name() const
{
    return m_field->name();
}

QString PopplerFormFieldText::uiName() const
{
    return m_field->uiName();
}

QString PopplerFormFieldText::fullyQualifiedName() const
{
    return m_field->fullyQualifiedName();
}

bool PopplerFormFieldText::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldText::setReadOnly(bool value)
{
    m_field->setReadOnly(value);
}

bool PopplerFormFieldText::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldText::setVisible(bool value)
{
    m_field->setVisible(value);
}

bool PopplerFormFieldText::isPrintable() const
{
    return m_field->isPrintable();
}

void PopplerFormFieldText::setPrintable(bool value)
{
    m_field->setPrintable(value);
}

Okular::FormFieldText::TextType PopplerFormFieldText::textType() const
{
    switch(m_field->textType())
    {
    case Poppler::FormFieldText::Normal:
        return Okular::FormFieldText::Normal;
    case Poppler::FormFieldText::Multiline:
        return Okular::FormFieldText::Multiline;
    case Poppler::FormFieldText::FileSelect:
        return Okular::FormFieldText::FileSelect;
    }
    return Okular::FormFieldText::Normal;
}

QString PopplerFormFieldText::text() const
{
    return m_field->text();
}

void PopplerFormFieldText::setText(const QString& text)
{
    m_field->setText(text);
}

void PopplerFormFieldText::setAppearanceText(const QString& text)
{
    m_field->setAppearanceText(text);
}

bool PopplerFormFieldText::isPassword() const
{
    return m_field->isPassword();
}

bool PopplerFormFieldText::isRichText() const
{
    return m_field->isRichText();
}

int PopplerFormFieldText::maximumLength() const
{
    return m_field->maximumLength();
}

Qt::Alignment PopplerFormFieldText::textAlignment() const
{
    return Qt::AlignTop | m_field->textAlignment();
}

bool PopplerFormFieldText::canBeSpellChecked() const
{
    return m_field->canBeSpellChecked();
}

PopplerFormFieldChoice::PopplerFormFieldChoice(
    std::unique_ptr<Poppler::FormFieldChoice> field) :
    Okular::FormFieldChoice(),
    m_field(std::move(field))
{
    m_rect = Okular::NormalizedRect::fromQRectF(m_field->rect());
    m_id = m_field->id();
    SET_ACTIONS

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(0, 87, 0)
    QMap<QString, QString> values;
    const auto fieldChoicesWithExportValues =
        m_field->choicesWithExportValues();
    for(const QPair<QString, QString>& value : fieldChoicesWithExportValues)
    {
        values.insert(value.first, value.second);
    }
    setExportValues(values);
#endif
}

Okular::NormalizedRect PopplerFormFieldChoice::rect() const
{
    return m_rect;
}

int PopplerFormFieldChoice::id() const
{
    return m_id;
}

QString PopplerFormFieldChoice::name() const
{
    return m_field->name();
}

QString PopplerFormFieldChoice::uiName() const
{
    return m_field->uiName();
}

QString PopplerFormFieldChoice::fullyQualifiedName() const
{
    return m_field->fullyQualifiedName();
}

bool PopplerFormFieldChoice::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldChoice::setReadOnly(bool value)
{
    m_field->setReadOnly(value);
}

bool PopplerFormFieldChoice::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldChoice::setVisible(bool value)
{
    m_field->setVisible(value);
}

bool PopplerFormFieldChoice::isPrintable() const
{
    return m_field->isPrintable();
}

void PopplerFormFieldChoice::setPrintable(bool value)
{
    m_field->setPrintable(value);
}

Okular::FormFieldChoice::ChoiceType PopplerFormFieldChoice::choiceType() const
{
    switch(m_field->choiceType())
    {
    case Poppler::FormFieldChoice::ComboBox:
        return Okular::FormFieldChoice::ComboBox;
    case Poppler::FormFieldChoice::ListBox:
        return Okular::FormFieldChoice::ListBox;
    }
    return Okular::FormFieldChoice::ListBox;
}

QStringList PopplerFormFieldChoice::choices() const
{
    return m_field->choices();
}

bool PopplerFormFieldChoice::isEditable() const
{
    return m_field->isEditable();
}

bool PopplerFormFieldChoice::multiSelect() const
{
    return m_field->multiSelect();
}

QList<int> PopplerFormFieldChoice::currentChoices() const
{
    return m_field->currentChoices();
}

void PopplerFormFieldChoice::setCurrentChoices(const QList<int>& choices)
{
    m_field->setCurrentChoices(choices);
}

QString PopplerFormFieldChoice::editChoice() const
{
    return m_field->editChoice();
}

void PopplerFormFieldChoice::setEditChoice(const QString& text)
{
    m_field->setEditChoice(text);
}

Qt::Alignment PopplerFormFieldChoice::textAlignment() const
{
    return Qt::AlignTop | m_field->textAlignment();
}

bool PopplerFormFieldChoice::canBeSpellChecked() const
{
    return m_field->canBeSpellChecked();
}

PopplerFormFieldSignature::PopplerFormFieldSignature(
    std::unique_ptr<Poppler::FormFieldSignature> field) :
    Okular::FormFieldSignature(),
    m_field(std::move(field))
{
    m_rect = Okular::NormalizedRect::fromQRectF(m_field->rect());
    m_id = m_field->id();
    int validateOptions =
        Poppler::FormFieldSignature::ValidateVerifyCertificate;
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
    if(!PDFSettings::checkOCSPServers())
    {
        validateOptions =
            validateOptions |
            Poppler::FormFieldSignature::ValidateWithoutOCSPRevocationCheck;
    }
#endif
    m_info = new PopplerSignatureInfo(m_field->validate(
        static_cast<Poppler::FormFieldSignature::ValidateOptions>(
            validateOptions)));
    SET_ACTIONS
}

PopplerFormFieldSignature::~PopplerFormFieldSignature()
{
    delete m_info;
}

Okular::NormalizedRect PopplerFormFieldSignature::rect() const
{
    return m_rect;
}

int PopplerFormFieldSignature::id() const
{
    return m_id;
}

QString PopplerFormFieldSignature::name() const
{
    return m_field->name();
}

QString PopplerFormFieldSignature::uiName() const
{
    return m_field->uiName();
}

QString PopplerFormFieldSignature::fullyQualifiedName() const
{
    return m_field->fullyQualifiedName();
}

bool PopplerFormFieldSignature::isReadOnly() const
{
    return m_field->isReadOnly();
}

bool PopplerFormFieldSignature::isVisible() const
{
    return m_field->isVisible();
}

PopplerFormFieldSignature::SignatureType
    PopplerFormFieldSignature::signatureType() const
{
    switch(m_field->signatureType())
    {
    case Poppler::FormFieldSignature::AdbePkcs7sha1:
        return Okular::FormFieldSignature::AdbePkcs7sha1;
    case Poppler::FormFieldSignature::AdbePkcs7detached:
        return Okular::FormFieldSignature::AdbePkcs7detached;
    case Poppler::FormFieldSignature::EtsiCAdESdetached:
        return Okular::FormFieldSignature::EtsiCAdESdetached;
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(22, 2, 0)
    case Poppler::FormFieldSignature::UnsignedSignature:
        return Okular::FormFieldSignature::UnsignedSignature;
#endif
    default:
        return Okular::FormFieldSignature::UnknownType;
    }
}

const Okular::SignatureInfo& PopplerFormFieldSignature::signatureInfo() const
{
    return *m_info;
}

bool PopplerFormFieldSignature::sign(const Okular::NewSignatureData& oData,
                                     const QString& newPath) const
{
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(22, 2, 0)
    Poppler::PDFConverter::NewSignatureData pData;
    PDFGenerator::okularToPoppler(oData, &pData);
    return m_field->sign(newPath, pData) ==
           Poppler::FormFieldSignature::SigningSuccess;
#else
    Q_UNUSED(oData)
    Q_UNUSED(newPath)
    return false;
#endif
}
