/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2019 Oliver Sander <oliver.sander@tu-dresden.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_PDF_FORMFIELDS_H_
#define _OKULAR_GENERATOR_PDF_FORMFIELDS_H_

#include <poppler-form.h>
#include "core/form.h"

class PopplerFormFieldButton : public Okular::FormFieldButton
{
public:
    explicit PopplerFormFieldButton(
        std::unique_ptr<Poppler::FormFieldButton> field);

    // inherited from Okular::FormField
    Okular::NormalizedRect rect() const override;
    int id() const override;
    QString name() const override;
    QString uiName() const override;
    QString fullyQualifiedName() const override;
    bool isReadOnly() const override;
    void setReadOnly(bool value) override;
    bool isVisible() const override;
    void setVisible(bool value) override;
    bool isPrintable() const override;
    void setPrintable(bool value) override;

    // inherited from Okular::FormFieldButton
    ButtonType buttonType() const override;
    QString caption() const override;
    bool state() const override;
    void setState(bool state) override;
    QList<int> siblings() const override;
    void setIcon(Okular::FormField* field) override;
    /*
     * Supported only in newer versions of Poppler library.
     *
     * @since 1.9
     */
    Poppler::FormFieldIcon icon() const;

private:
    std::unique_ptr<Poppler::FormFieldButton> m_field;
    Okular::NormalizedRect m_rect;
    int m_id;
};

class PopplerFormFieldText : public Okular::FormFieldText
{
public:
    explicit PopplerFormFieldText(
        std::unique_ptr<Poppler::FormFieldText> field);

    // inherited from Okular::FormField
    Okular::NormalizedRect rect() const override;
    int id() const override;
    QString name() const override;
    QString uiName() const override;
    QString fullyQualifiedName() const override;
    bool isReadOnly() const override;
    void setReadOnly(bool value) override;
    bool isVisible() const override;
    void setVisible(bool value) override;
    bool isPrintable() const override;
    void setPrintable(bool value) override;

    // inherited from Okular::FormFieldText
    Okular::FormFieldText::TextType textType() const override;
    QString text() const override;
    void setText(const QString& text) override;
    void setAppearanceText(const QString& text) override;
    bool isPassword() const override;
    bool isRichText() const override;
    int maximumLength() const override;
    Qt::Alignment textAlignment() const override;
    bool canBeSpellChecked() const override;

private:
    std::unique_ptr<Poppler::FormFieldText> m_field;
    Okular::NormalizedRect m_rect;
    int m_id;
};

class PopplerFormFieldChoice : public Okular::FormFieldChoice
{
public:
    explicit PopplerFormFieldChoice(
        std::unique_ptr<Poppler::FormFieldChoice> field);

    // inherited from Okular::FormField
    Okular::NormalizedRect rect() const override;
    int id() const override;
    QString name() const override;
    QString uiName() const override;
    QString fullyQualifiedName() const override;
    bool isReadOnly() const override;
    void setReadOnly(bool value) override;
    bool isVisible() const override;
    void setVisible(bool value) override;
    bool isPrintable() const override;
    void setPrintable(bool value) override;

    // inherited from Okular::FormFieldChoice
    ChoiceType choiceType() const override;
    QStringList choices() const override;
    bool isEditable() const override;
    bool multiSelect() const override;
    QList<int> currentChoices() const override;
    void setCurrentChoices(const QList<int>& choices) override;
    QString editChoice() const override;
    void setEditChoice(const QString& text) override;
    Qt::Alignment textAlignment() const override;
    bool canBeSpellChecked() const override;

private:
    std::unique_ptr<Poppler::FormFieldChoice> m_field;
    Okular::NormalizedRect m_rect;
    int m_id;
};

class PopplerFormFieldSignature : public Okular::FormFieldSignature
{
public:
    explicit PopplerFormFieldSignature(
        std::unique_ptr<Poppler::FormFieldSignature> field);
    ~PopplerFormFieldSignature() override;

    // inherited from Okular::FormField
    Okular::NormalizedRect rect() const override;
    int id() const override;
    QString name() const override;
    QString uiName() const override;
    QString fullyQualifiedName() const override;
    bool isReadOnly() const override;
    bool isVisible() const override;

    // inherited from Okular::FormFieldSignature
    SignatureType signatureType() const override;
    const Okular::SignatureInfo& signatureInfo() const override;
    bool sign(const Okular::NewSignatureData& oData,
              const QString& newPath) const override;

private:
    std::unique_ptr<Poppler::FormFieldSignature> m_field;
    Okular::SignatureInfo* m_info;
    Okular::NormalizedRect m_rect;
    int m_id;
};

#endif
