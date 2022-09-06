/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_FORM_H_
#define _OKULAR_FORM_H_

#include "annotations.h"
#include "area.h"
#include "document.h"
#include "okularcore_export.h"
#include "signatureutils.h"

#include <QStringList>

#include <memory>

namespace Okular
{
class Action;
class Page;
class PagePrivate;
class FormFieldPrivate;
class FormFieldButtonPrivate;
class FormFieldTextPrivate;
class FormFieldChoicePrivate;
class FormFieldSignaturePrivate;

/**
 * @short The base interface of a form field.
 *
 * This is the very basic interface to represent a field in a form.
 *
 * This is not meant to be used as a direct base for the form fields in a
 * document, but its abstract subclasses are.
 */
class OKULARCORE_EXPORT FormField
{
    /// @cond PRIVATE
    friend class Page;
    friend class PagePrivate;
    /// @endcond

public:
    /**
     * The types of form field.
     */
    enum FieldType {
        FormButton,   ///< A "button". See @ref FormFieldButton::ButtonType.
        FormText,     ///< A field of variable text. See @ref FormFieldText::TextType.
        FormChoice,   ///< A choice field. See @ref FormFieldChoice::ChoiceType.
        FormSignature ///< A signature.
    };

    virtual ~FormField();

    /**
     * The type of the field.
     */
    FieldType type() const;

    /**
     * The bounding rect of the field, in normalized coordinates.
     */
    virtual NormalizedRect rect() const = 0;

    /**
     * The ID of the field.
     */
    virtual int id() const = 0;

    /**
     * The internal name of the field, to be used when referring to the
     * field in eg scripts.
     */
    virtual QString name() const = 0;

    /**
     * The visible name of the field, to be used in the user interface
     * (eg in error messages, etc).
     */
    virtual QString uiName() const = 0;

    /**
     * The fully qualified name of the field, is used in the JavaScript
     * scripts.
     *
     * @since 1.9
     */
    virtual QString fullyQualifiedName() const = 0;

    /**
     * Whether the field is read-only.
     */
    virtual bool isReadOnly() const;

    /**
     * Whether the field is read-only.
     *
     * @since 1.4
     */
    virtual void setReadOnly(bool value);

    /**
     * Whether this form field is visible.
     */
    virtual bool isVisible() const;

    /**
     * Whether the field is visible.
     *
     * @since 1.5
     */
    virtual void setVisible(bool value);

    /**
      Whether this field is printable.

      @since 1.9
    */
    virtual bool isPrintable() const;

    /**
      Set this field printable

      @since 1.9
    */
    virtual void setPrintable(bool value);

    Action *activationAction() const;

    /**
     * Describes the type of form additional action.
     *
     * @since 1.1
     */
    enum AdditionalActionType {
        FieldModified,  ///< An action to be performed when the user modifies the field
        FormatField,    ///< An action to be performed before the field is formatted to display its value
        ValidateField,  ///< An action to be performed when the field value changes
        CalculateField, ///< An action to be performed when the field needs to be recalculated
    };

    /**
     * Returns the additional action of the given @p type or @c nullptr if no action has been defined.
     *
     * @since 1.1
     */
    Action *additionalAction(AdditionalActionType type) const;

    /* Returns the additional action of the given @p type or @c nullptr if no action has been defined.
     *
     * This is for actions of annotation widgets associated with the FormField
     *
     * @since 1.5
     */
    Action *additionalAction(Annotation::AdditionalActionType type) const;

    /* Returns all the additional actions for this form
     *
     * @since 22.04
     */
    QList<Action *> additionalActions() const;

    /**
     * Returns the page of this form field
     *
     * @since 21.12.2
     */
    Page *page() const;

protected:
    /// @cond PRIVATE
    explicit FormField(FormFieldPrivate &dd);
    Q_DECLARE_PRIVATE(FormField)
    FormFieldPrivate *d_ptr;
    /// @endcond

    void setActivationAction(Action *action);
    void setAdditionalAction(AdditionalActionType type, Action *action);
    void setAdditionalAction(Annotation::AdditionalActionType type, Action *action);

private:
    Q_DISABLE_COPY(FormField)
};

/**
 * @short Interface of a button form field.
 *
 * This is the base interface to reimplement to represent a button field, like
 * a push button, a check box or a radio button.
 *
 * @since 0.7 (KDE 4.1)
 */
class OKULARCORE_EXPORT FormFieldButton : public FormField
{
public:
    /**
     * The types of button field.
     */
    enum ButtonType {
        Push,     ///< A simple push button.
        CheckBox, ///< A check box.
        Radio     ///< A radio button.
    };

    ~FormFieldButton() override;

    /**
      The particular type of the button field.
     */
    virtual ButtonType buttonType() const = 0;

    /**
     * The caption to be used for the button.
     */
    virtual QString caption() const = 0;

    /**
     * The state of the button.
     */
    virtual bool state() const = 0;

    /**
     * Sets the state of the button to the new \p state .
     */
    virtual void setState(bool state);

    /**
     * The list with the IDs of siblings (ie, buttons belonging to the same
     * group as the current one.
     *
     * Valid only for \ref Radio buttons, an empty list otherwise.
     */
    virtual QList<int> siblings() const = 0;

    /**
     * Sets the icon of the Button to the Icon of the field parameter.
     *
     * @since 1.9
     */
    virtual void setIcon(Okular::FormField *field);

protected:
    FormFieldButton();

private:
    Q_DECLARE_PRIVATE(FormFieldButton)
    Q_DISABLE_COPY(FormFieldButton)
};

/**
 * @short Interface of a text form field.
 *
 * This is the base interface to reimplement to represent a text field, ie a
 * field where the user insert text.
 */
class OKULARCORE_EXPORT FormFieldText : public FormField
{
public:
    /**
     * The types of text field.
     */
    enum TextType {
        Normal,    ///< A simple singleline text field.
        Multiline, ///< A multiline text field.
        FileSelect ///< An input field to select the path of a file on disk.
    };

    ~FormFieldText() override;

    /**
     * The particular type of the text field.
     */
    virtual TextType textType() const = 0;

    /**
     * The text of text field.
     */
    virtual QString text() const = 0;

    /**
     * Sets the new @p text in the text field.
     *
     * The default implementation does nothing.
     *
     * Reimplemented only if the setting of new text is supported.
     */
    virtual void setText(const QString &text);

    /**
     * Whether this text field is a password input, eg its text @b must be
     * replaced with asterisks.
     *
     * Always false for @ref FileSelect text fields.
     */
    virtual bool isPassword() const;

    /**
     * Whether this text field should allow rich text.
     */
    virtual bool isRichText() const;

    /**
     * The maximum length allowed for the text of text field, or -1 if
     * there is no limitation for the text.
     */
    virtual int maximumLength() const;

    /**
     * The alignment of the text within the field.
     */
    virtual Qt::Alignment textAlignment() const;

    /**
     * Whether the text inserted manually in the field (where possible)
     * can be spell-checked.
     *
     * @note meaningful only if the field is editable.
     */
    virtual bool canBeSpellChecked() const;

    /**
     * Set the text which should be rendered by the PDF.
     *
     * @since 1.9
     */
    virtual void setAppearanceText(const QString &text) = 0;

protected:
    FormFieldText();

private:
    Q_DECLARE_PRIVATE(FormFieldText)
    Q_DISABLE_COPY(FormFieldText)
};

/**
 * @short Interface of a choice form field.
 *
 * This is the base interface to reimplement to represent a choice field, ie a
 * field where the user can select one (of more) element(s) among a set of
 * choices.
 */
class OKULARCORE_EXPORT FormFieldChoice : public FormField
{
public:
    /**
     * The types of choice field.
     */
    enum ChoiceType {
        ComboBox, ///< A combo box choice field.
        ListBox   ///< A list box choice field.
    };

    ~FormFieldChoice() override;

    /**
     * The particular type of the choice field.
     */
    virtual ChoiceType choiceType() const = 0;

    /**
     * The possible choices of the choice field.
     */
    virtual QStringList choices() const = 0;

    /**
     * Whether this ComboBox is editable, ie the user can type in a custom
     * value.
     *
     * Always false for the other types of choices.
     */
    virtual bool isEditable() const;

    /**
     * Whether more than one choice of this ListBox can be selected at the
     * same time.
     *
     * Always false for the other types of choices.
     */
    virtual bool multiSelect() const;

    /**
     * The currently selected choices.
     *
     * Always one element in the list in case of single choice elements.
     */
    virtual QList<int> currentChoices() const = 0;

    /**
     * Sets the selected choices to @p choices .
     */
    virtual void setCurrentChoices(const QList<int> &choices);

    /**
      The text entered into an editable combo box choice field

      @since 0.16 (KDE 4.10)
    */
    virtual QString editChoice() const;

    /**
      Sets the text entered into an editable combo box choice field

      @since 0.16 (KDE 4.10)
    */
    virtual void setEditChoice(const QString &text);

    /**
     * The alignment of the text within the field.
     */
    virtual Qt::Alignment textAlignment() const;

    /**
     * Whether the text inserted manually in the field (where possible)
     * can be spell-checked.
     *
     * @note meaningful only if the field is editable.
     */
    virtual bool canBeSpellChecked() const;

    /**
     * Returns the export value for a given choice
     *
     * @since 1.11
     */
    QString exportValueForChoice(const QString &choice) const;

protected:
    FormFieldChoice();

    /**
     * The possible choices of the choice field.
     * The key is the display name of the choice,
     * The value is the export value (i.e. for use in javascript, etc) of the choice
     *
     * @since 1.11
     */
    void setExportValues(const QMap<QString, QString> &values);

private:
    Q_DECLARE_PRIVATE(FormFieldChoice)
    Q_DISABLE_COPY(FormFieldChoice)
};

/**
 * @short Interface of a signature form field.
 *
 * This is the base interface to reimplement to represent a signature field.
 */
class OKULARCORE_EXPORT FormFieldSignature : public FormField
{
public:
    /**
     * The types of signature.
     */
    enum SignatureType {
        AdbePkcs7sha1,
        AdbePkcs7detached,
        EtsiCAdESdetached,
        UnknownType,
        UnsignedSignature ///< The signature field has not been signed yet. @since 22.04
    };

    ~FormFieldSignature() override;

    /**
     * The signature type
     */
    virtual SignatureType signatureType() const = 0;

    /**
     * The signature info
     */
    virtual const SignatureInfo &signatureInfo() const = 0;

    /**
      Signs a field of UnsignedSignature type.

      @since 22.04
     */
    virtual bool sign(const NewSignatureData &data, const QString &newPath) const = 0;

protected:
    FormFieldSignature();

private:
    Q_DECLARE_PRIVATE(FormFieldSignature)
    Q_DISABLE_COPY(FormFieldSignature)
};

}

Q_DECLARE_METATYPE(const Okular::FormFieldSignature *);

#endif
