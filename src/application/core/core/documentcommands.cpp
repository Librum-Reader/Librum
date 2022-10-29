/*
    SPDX-FileCopyrightText: 2013 Jon Mease <jon.mease@gmail.com>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <KLocalizedString>
#include "annotations.h"
#include "debug_p.h"
#include "document_p.h"
#include "documentcommands_p.h"
#include "form.h"
#include "page.h"
#include "page_p.h"
#include "utils_p.h"

namespace Okular
{
void moveViewportIfBoundingRectNotFullyVisible(
    Okular::NormalizedRect boundingRect, DocumentPrivate* docPriv,
    int pageNumber)
{
    const Rotation pageRotation =
        docPriv->m_parent->page(pageNumber)->rotation();
    const QTransform rotationMatrix = Okular::buildRotationMatrix(pageRotation);
    boundingRect.transform(rotationMatrix);
    if(!docPriv->isNormalizedRectangleFullyVisible(boundingRect, pageNumber))
    {
        DocumentViewport searchViewport(pageNumber);
        searchViewport.rePos.enabled = true;
        searchViewport.rePos.normalizedX =
            (boundingRect.left + boundingRect.right) / 2.0;
        searchViewport.rePos.normalizedY =
            (boundingRect.top + boundingRect.bottom) / 2.0;
        docPriv->m_parent->setViewport(searchViewport, nullptr, true);
    }
}

Okular::NormalizedRect buildBoundingRectangleForButtons(
    const QList<Okular::FormFieldButton*>& formButtons)
{
    // Initialize coordinates of the bounding rect
    double left = 1.0;
    double top = 1.0;
    double right = 0.0;
    double bottom = 0.0;

    for(const FormFieldButton* formButton : formButtons)
    {
        left = qMin<double>(left, formButton->rect().left);
        top = qMin<double>(top, formButton->rect().top);
        right = qMax<double>(right, formButton->rect().right);
        bottom = qMax<double>(bottom, formButton->rect().bottom);
    }
    Okular::NormalizedRect boundingRect(left, top, right, bottom);
    return boundingRect;
}

AddAnnotationCommand::AddAnnotationCommand(Okular::DocumentPrivate* docPriv,
                                           Okular::Annotation* annotation,
                                           int pageNumber) :
    m_docPriv(docPriv),
    m_annotation(annotation),
    m_pageNumber(pageNumber),
    m_done(false)
{
    setText(i18nc("Add an annotation to the page", "add annotation"));
}

AddAnnotationCommand::~AddAnnotationCommand()
{
    if(!m_done)
    {
        delete m_annotation;
    }
}

void AddAnnotationCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performRemovePageAnnotation(m_pageNumber, m_annotation);
    m_done = false;
}

void AddAnnotationCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performAddPageAnnotation(m_pageNumber, m_annotation);
    m_done = true;
}

bool AddAnnotationCommand::refreshInternalPageReferences(
    const QVector<Okular::Page*>& newPagesVector)
{
    if(m_done)
    {
        // We don't always update m_annotation because even if the annotation
        // has been added to the document it can have been removed later so the
        // annotation pointer is stored inside a following
        // RemoveAnnotationCommand and thus doesn't need updating because it
        // didn't change because of the document reload
        auto a = newPagesVector[m_pageNumber]->annotation(
            m_annotation->uniqueName());
        if(a)
        {
            m_annotation = a;
        }
    }

    return true;
}

RemoveAnnotationCommand::RemoveAnnotationCommand(Okular::DocumentPrivate* doc,
                                                 Okular::Annotation* annotation,
                                                 int pageNumber) :
    m_docPriv(doc),
    m_annotation(annotation),
    m_pageNumber(pageNumber),
    m_done(false)
{
    setText(i18nc("Remove an annotation from the page", "remove annotation"));
}

RemoveAnnotationCommand::~RemoveAnnotationCommand()
{
    if(m_done)
    {
        delete m_annotation;
    }
}

void RemoveAnnotationCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performAddPageAnnotation(m_pageNumber, m_annotation);
    m_done = false;
}

void RemoveAnnotationCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performRemovePageAnnotation(m_pageNumber, m_annotation);
    m_done = true;
}

bool RemoveAnnotationCommand::refreshInternalPageReferences(
    const QVector<Okular::Page*>& newPagesVector)
{
    if(!m_done)
    {
        // We don't always update m_annotation because it can happen that the
        // annotation remove has been undo and that annotation addition has also
        // been undone so the annotation pointer is stored inside a previous
        // AddAnnotationCommand and thus doesn't need updating because it didn't
        // change because of the document reload
        auto a = newPagesVector[m_pageNumber]->annotation(
            m_annotation->uniqueName());
        if(a)
        {
            m_annotation = a;
        }
    }

    return true;
}

ModifyAnnotationPropertiesCommand::ModifyAnnotationPropertiesCommand(
    DocumentPrivate* docPriv, Annotation* annotation, int pageNumber,
    const QDomNode& oldProperties, const QDomNode& newProperties) :
    m_docPriv(docPriv),
    m_annotation(annotation),
    m_pageNumber(pageNumber),
    m_prevProperties(oldProperties),
    m_newProperties(newProperties)
{
    setText(i18nc(
        "Modify an annotation's internal properties (Color, line-width, etc.)",
        "modify annotation properties"));
}

void ModifyAnnotationPropertiesCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_annotation->setAnnotationProperties(m_prevProperties);
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

void ModifyAnnotationPropertiesCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_annotation->setAnnotationProperties(m_newProperties);
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

bool ModifyAnnotationPropertiesCommand::refreshInternalPageReferences(
    const QVector<Okular::Page*>& newPagesVector)
{
    // Same reason for not unconditionally updating m_annotation, the annotation
    // pointer can be stored in an add/Remove command
    auto a =
        newPagesVector[m_pageNumber]->annotation(m_annotation->uniqueName());
    if(a)
    {
        m_annotation = a;
    }

    return true;
}

TranslateAnnotationCommand::TranslateAnnotationCommand(
    DocumentPrivate* docPriv, Annotation* annotation, int pageNumber,
    const Okular::NormalizedPoint& delta, bool completeDrag) :
    m_docPriv(docPriv),
    m_annotation(annotation),
    m_pageNumber(pageNumber),
    m_delta(delta),
    m_completeDrag(completeDrag)
{
    setText(i18nc("Translate an annotation's position on the page",
                  "translate annotation"));
}

void TranslateAnnotationCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(
        translateBoundingRectangle(minusDelta()), m_docPriv, m_pageNumber);
    m_annotation->translate(minusDelta());
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

void TranslateAnnotationCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(
        translateBoundingRectangle(m_delta), m_docPriv, m_pageNumber);
    m_annotation->translate(m_delta);
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

int TranslateAnnotationCommand::id() const
{
    return 1;
}

bool TranslateAnnotationCommand::mergeWith(const QUndoCommand* uc)
{
    TranslateAnnotationCommand* tuc = (TranslateAnnotationCommand*)uc;

    if(tuc->m_annotation != m_annotation)
    {
        return false;
    }

    if(m_completeDrag)
    {
        return false;
    }
    m_delta = Okular::NormalizedPoint(tuc->m_delta.x + m_delta.x,
                                      tuc->m_delta.y + m_delta.y);
    m_completeDrag = tuc->m_completeDrag;
    return true;
}

Okular::NormalizedPoint TranslateAnnotationCommand::minusDelta()
{
    return Okular::NormalizedPoint(-m_delta.x, -m_delta.y);
}

Okular::NormalizedRect TranslateAnnotationCommand::translateBoundingRectangle(
    const Okular::NormalizedPoint& delta)
{
    Okular::NormalizedRect annotBoundingRect =
        m_annotation->boundingRectangle();
    double left =
        qMin<double>(annotBoundingRect.left, annotBoundingRect.left + delta.x);
    double right = qMax<double>(annotBoundingRect.right,
                                annotBoundingRect.right + delta.x);
    double top =
        qMin<double>(annotBoundingRect.top, annotBoundingRect.top + delta.y);
    double bottom = qMax<double>(annotBoundingRect.bottom,
                                 annotBoundingRect.bottom + delta.y);
    Okular::NormalizedRect boundingRect(left, top, right, bottom);
    return boundingRect;
}

bool TranslateAnnotationCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    // Same reason for not unconditionally updating m_annotation, the annotation
    // pointer can be stored in an add/Remove command
    auto a =
        newPagesVector[m_pageNumber]->annotation(m_annotation->uniqueName());
    if(a)
    {
        m_annotation = a;
    }

    return true;
}

AdjustAnnotationCommand::AdjustAnnotationCommand(
    Okular::DocumentPrivate* docPriv, Okular::Annotation* annotation,
    int pageNumber, const Okular::NormalizedPoint& delta1,
    const Okular::NormalizedPoint& delta2, bool completeDrag) :
    m_docPriv(docPriv),
    m_annotation(annotation),
    m_pageNumber(pageNumber),
    m_delta1(delta1),
    m_delta2(delta2),
    m_completeDrag(completeDrag)
{
    setText(i18nc("Change an annotation's size", "adjust annotation"));
}

void AdjustAnnotationCommand::undo()
{
    const NormalizedPoint minusDelta1 =
        Okular::NormalizedPoint(-m_delta1.x, -m_delta1.y);
    const NormalizedPoint minusDelta2 =
        Okular::NormalizedPoint(-m_delta2.x, -m_delta2.y);
    moveViewportIfBoundingRectNotFullyVisible(
        adjustBoundingRectangle(minusDelta1, minusDelta2), m_docPriv,
        m_pageNumber);
    m_annotation->adjust(minusDelta1, minusDelta2);
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

void AdjustAnnotationCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(
        adjustBoundingRectangle(m_delta1, m_delta2), m_docPriv, m_pageNumber);
    m_annotation->adjust(m_delta1, m_delta2);
    m_docPriv->performModifyPageAnnotation(m_pageNumber, m_annotation, true);
}

int AdjustAnnotationCommand::id() const
{
    return 5;
}

bool AdjustAnnotationCommand::mergeWith(const QUndoCommand* uc)
{
    AdjustAnnotationCommand* tuc = (AdjustAnnotationCommand*)uc;

    if(tuc->m_annotation != m_annotation)
    {
        return false;
    }

    if(m_completeDrag)
    {
        return false;
    }
    m_delta1 = Okular::NormalizedPoint(tuc->m_delta1.x + m_delta1.x,
                                       tuc->m_delta1.y + m_delta1.y);
    m_delta2 = Okular::NormalizedPoint(tuc->m_delta2.x + m_delta2.x,
                                       tuc->m_delta2.y + m_delta2.y);
    m_completeDrag = tuc->m_completeDrag;
    return true;
}

Okular::NormalizedRect AdjustAnnotationCommand::adjustBoundingRectangle(
    const Okular::NormalizedPoint& delta1,
    const Okular::NormalizedPoint& delta2)
{
    const Okular::NormalizedRect annotBoundingRect =
        m_annotation->boundingRectangle();
    const double left =
        qMin<double>(annotBoundingRect.left, annotBoundingRect.left + delta1.x);
    const double right = qMax<double>(annotBoundingRect.right,
                                      annotBoundingRect.right + delta2.x);
    const double top =
        qMin<double>(annotBoundingRect.top, annotBoundingRect.top + delta1.y);
    const double bottom = qMax<double>(annotBoundingRect.bottom,
                                       annotBoundingRect.bottom + delta2.y);
    return Okular::NormalizedRect(left, top, right, bottom);
}

bool AdjustAnnotationCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    // Same reason for not unconditionally updating m_annotation, the annotation
    // pointer can be stored in an add/Remove command
    auto a =
        newPagesVector[m_pageNumber]->annotation(m_annotation->uniqueName());
    if(a)
    {
        m_annotation = a;
    }

    return true;
}

EditTextCommand::EditTextCommand(const QString& newContents, int newCursorPos,
                                 const QString& prevContents, int prevCursorPos,
                                 int prevAnchorPos) :
    m_newContents(newContents),
    m_newCursorPos(newCursorPos),
    m_prevContents(prevContents),
    m_prevCursorPos(prevCursorPos),
    m_prevAnchorPos(prevAnchorPos)
{
    setText(i18nc("Generic text edit command", "edit text"));

    //// Determine edit type
    // If There was a selection then edit was not a simple single character
    // backspace, delete, or insert
    if(m_prevCursorPos != m_prevAnchorPos)
    {
        qCDebug(OkularCoreDebug) << "OtherEdit, selection";
        m_editType = OtherEdit;
    }
    else if(newContentsRightOfCursor() == oldContentsRightOfCursor() &&
            newContentsLeftOfCursor() ==
                oldContentsLeftOfCursor().left(
                    oldContentsLeftOfCursor().length() - 1) &&
            oldContentsLeftOfCursor().rightRef(1) != QLatin1Char('\n'))
    {
        qCDebug(OkularCoreDebug) << "CharBackspace";
        m_editType = CharBackspace;
    }
    else if(newContentsLeftOfCursor() == oldContentsLeftOfCursor() &&
            newContentsRightOfCursor() ==
                oldContentsRightOfCursor().right(
                    oldContentsRightOfCursor().length() - 1) &&
            oldContentsRightOfCursor().leftRef(1) != QLatin1Char('\n'))
    {
        qCDebug(OkularCoreDebug) << "CharDelete";
        m_editType = CharDelete;
    }
    else if(newContentsRightOfCursor() == oldContentsRightOfCursor() &&
            newContentsLeftOfCursor().left(newContentsLeftOfCursor().length() -
                                           1) == oldContentsLeftOfCursor() &&
            newContentsLeftOfCursor().rightRef(1) != QLatin1Char('\n'))
    {
        qCDebug(OkularCoreDebug) << "CharInsert";
        m_editType = CharInsert;
    }
    else
    {
        qCDebug(OkularCoreDebug) << "OtherEdit";
        m_editType = OtherEdit;
    }
}

bool EditTextCommand::mergeWith(const QUndoCommand* uc)
{
    EditTextCommand* euc = (EditTextCommand*)uc;

    // Only attempt merge of euc into this if our new state matches euc's old
    // state and the editTypes match and are not type OtherEdit
    if(m_newContents == euc->m_prevContents &&
       m_newCursorPos == euc->m_prevCursorPos &&
       m_editType == euc->m_editType && m_editType != OtherEdit)
    {
        m_newContents = euc->m_newContents;
        m_newCursorPos = euc->m_newCursorPos;
        return true;
    }
    return false;
}

QString EditTextCommand::oldContentsLeftOfCursor()
{
    return m_prevContents.left(m_prevCursorPos);
}

QString EditTextCommand::oldContentsRightOfCursor()
{
    return m_prevContents.right(m_prevContents.length() - m_prevCursorPos);
}

QString EditTextCommand::newContentsLeftOfCursor()
{
    return m_newContents.left(m_newCursorPos);
}

QString EditTextCommand::newContentsRightOfCursor()
{
    return m_newContents.right(m_newContents.length() - m_newCursorPos);
}

EditAnnotationContentsCommand::EditAnnotationContentsCommand(
    DocumentPrivate* docPriv, Annotation* annotation, int pageNumber,
    const QString& newContents, int newCursorPos, const QString& prevContents,
    int prevCursorPos, int prevAnchorPos) :
    EditTextCommand(newContents, newCursorPos, prevContents, prevCursorPos,
                    prevAnchorPos),
    m_docPriv(docPriv),
    m_annotation(annotation),
    m_pageNumber(pageNumber)
{
    setText(i18nc("Edit an annotation's text contents",
                  "edit annotation contents"));
}

void EditAnnotationContentsCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performSetAnnotationContents(m_prevContents, m_annotation,
                                            m_pageNumber);
    Q_EMIT m_docPriv->m_parent->annotationContentsChangedByUndoRedo(
        m_annotation, m_prevContents, m_prevCursorPos, m_prevAnchorPos);
}

void EditAnnotationContentsCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_annotation->boundingRectangle(),
                                              m_docPriv, m_pageNumber);
    m_docPriv->performSetAnnotationContents(m_newContents, m_annotation,
                                            m_pageNumber);
    Q_EMIT m_docPriv->m_parent->annotationContentsChangedByUndoRedo(
        m_annotation, m_newContents, m_newCursorPos, m_newCursorPos);
}

int EditAnnotationContentsCommand::id() const
{
    return 2;
}

bool EditAnnotationContentsCommand::mergeWith(const QUndoCommand* uc)
{
    EditAnnotationContentsCommand* euc = (EditAnnotationContentsCommand*)uc;
    // Only attempt merge of euc into this if they modify the same annotation
    if(m_annotation == euc->m_annotation)
    {
        return EditTextCommand::mergeWith(uc);
    }
    else
    {
        return false;
    }
}

bool EditAnnotationContentsCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    auto a =
        newPagesVector[m_pageNumber]->annotation(m_annotation->uniqueName());
    if(a)
    {
        m_annotation = a;
    }

    return true;
}

EditFormTextCommand::EditFormTextCommand(
    Okular::DocumentPrivate* docPriv, Okular::FormFieldText* form,
    int pageNumber, const QString& newContents, int newCursorPos,
    const QString& prevContents, int prevCursorPos, int prevAnchorPos) :
    EditTextCommand(newContents, newCursorPos, prevContents, prevCursorPos,
                    prevAnchorPos),
    m_docPriv(docPriv),
    m_form(form),
    m_pageNumber(pageNumber)
{
    setText(i18nc("Edit an form's text contents", "edit form contents"));
}

void EditFormTextCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    m_form->setText(m_prevContents);
    Q_EMIT m_docPriv->m_parent->formTextChangedByUndoRedo(
        m_pageNumber, m_form, m_prevContents, m_prevCursorPos, m_prevAnchorPos);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

void EditFormTextCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    m_form->setText(m_newContents);
    Q_EMIT m_docPriv->m_parent->formTextChangedByUndoRedo(
        m_pageNumber, m_form, m_newContents, m_newCursorPos, m_newCursorPos);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

int EditFormTextCommand::id() const
{
    return 3;
}

bool EditFormTextCommand::mergeWith(const QUndoCommand* uc)
{
    EditFormTextCommand* euc = (EditFormTextCommand*)uc;
    // Only attempt merge of euc into this if they modify the same form
    if(m_form == euc->m_form)
    {
        return EditTextCommand::mergeWith(uc);
    }
    else
    {
        return false;
    }
}

bool EditFormTextCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    m_form =
        dynamic_cast<FormFieldText*>(Okular::PagePrivate::findEquivalentForm(
            newPagesVector[m_pageNumber], m_form));

    return m_form;
}

EditFormListCommand::EditFormListCommand(Okular::DocumentPrivate* docPriv,
                                         FormFieldChoice* form, int pageNumber,
                                         const QList<int>& newChoices,
                                         const QList<int>& prevChoices) :
    m_docPriv(docPriv),
    m_form(form),
    m_pageNumber(pageNumber),
    m_newChoices(newChoices),
    m_prevChoices(prevChoices)
{
    setText(i18nc("Edit a list form's choices", "edit list form choices"));
}

void EditFormListCommand::undo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    m_form->setCurrentChoices(m_prevChoices);
    Q_EMIT m_docPriv->m_parent->formListChangedByUndoRedo(m_pageNumber, m_form,
                                                          m_prevChoices);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

void EditFormListCommand::redo()
{
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    m_form->setCurrentChoices(m_newChoices);
    Q_EMIT m_docPriv->m_parent->formListChangedByUndoRedo(m_pageNumber, m_form,
                                                          m_newChoices);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

bool EditFormListCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    m_form =
        dynamic_cast<FormFieldChoice*>(Okular::PagePrivate::findEquivalentForm(
            newPagesVector[m_pageNumber], m_form));

    return m_form;
}

EditFormComboCommand::EditFormComboCommand(
    Okular::DocumentPrivate* docPriv, FormFieldChoice* form, int pageNumber,
    const QString& newContents, int newCursorPos, const QString& prevContents,
    int prevCursorPos, int prevAnchorPos) :
    EditTextCommand(newContents, newCursorPos, prevContents, prevCursorPos,
                    prevAnchorPos),
    m_docPriv(docPriv),
    m_form(form),
    m_pageNumber(pageNumber),
    m_newIndex(-1),
    m_prevIndex(-1)
{
    setText(
        i18nc("Edit a combo form's selection", "edit combo form selection"));

    // Determine new and previous choice indices (if any)
    for(int i = 0; i < m_form->choices().size(); i++)
    {
        if(m_form->choices().at(i) == m_prevContents)
        {
            m_prevIndex = i;
        }

        if(m_form->choices().at(i) == m_newContents)
        {
            m_newIndex = i;
        }
    }
}

void EditFormComboCommand::undo()
{
    if(m_prevIndex != -1)
    {
        m_form->setCurrentChoices(QList<int>() << m_prevIndex);
    }
    else
    {
        m_form->setEditChoice(m_prevContents);
    }
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    Q_EMIT m_docPriv->m_parent->formComboChangedByUndoRedo(
        m_pageNumber, m_form, m_prevContents, m_prevCursorPos, m_prevAnchorPos);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

void EditFormComboCommand::redo()
{
    if(m_newIndex != -1)
    {
        m_form->setCurrentChoices(QList<int>() << m_newIndex);
    }
    else
    {
        m_form->setEditChoice(m_newContents);
    }
    moveViewportIfBoundingRectNotFullyVisible(m_form->rect(), m_docPriv,
                                              m_pageNumber);
    Q_EMIT m_docPriv->m_parent->formComboChangedByUndoRedo(
        m_pageNumber, m_form, m_newContents, m_newCursorPos, m_newCursorPos);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

int EditFormComboCommand::id() const
{
    return 4;
}

bool EditFormComboCommand::mergeWith(const QUndoCommand* uc)
{
    EditFormComboCommand* euc = (EditFormComboCommand*)uc;
    // Only attempt merge of euc into this if they modify the same form
    if(m_form == euc->m_form)
    {
        bool shouldMerge = EditTextCommand::mergeWith(uc);
        if(shouldMerge)
        {
            m_newIndex = euc->m_newIndex;
        }
        return shouldMerge;
    }
    else
    {
        return false;
    }
}

bool EditFormComboCommand::refreshInternalPageReferences(
    const QVector<Page*>& newPagesVector)
{
    m_form =
        dynamic_cast<FormFieldChoice*>(Okular::PagePrivate::findEquivalentForm(
            newPagesVector[m_pageNumber], m_form));

    return m_form;
}

EditFormButtonsCommand::EditFormButtonsCommand(
    Okular::DocumentPrivate* docPriv, int pageNumber,
    const QList<FormFieldButton*>& formButtons,
    const QList<bool>& newButtonStates) :
    m_docPriv(docPriv),
    m_pageNumber(pageNumber),
    m_formButtons(formButtons),
    m_newButtonStates(newButtonStates),
    m_prevButtonStates(QList<bool>())
{
    setText(i18nc("Edit the state of a group of form buttons",
                  "edit form button states"));
    for(const FormFieldButton* formButton : qAsConst(m_formButtons))
    {
        m_prevButtonStates.append(formButton->state());
    }
}

void EditFormButtonsCommand::undo()
{
    clearFormButtonStates();
    for(int i = 0; i < m_formButtons.size(); i++)
    {
        bool checked = m_prevButtonStates.at(i);
        if(checked)
        {
            m_formButtons.at(i)->setState(checked);
        }
    }

    Okular::NormalizedRect boundingRect =
        buildBoundingRectangleForButtons(m_formButtons);
    moveViewportIfBoundingRectNotFullyVisible(boundingRect, m_docPriv,
                                              m_pageNumber);
    Q_EMIT m_docPriv->m_parent->formButtonsChangedByUndoRedo(m_pageNumber,
                                                             m_formButtons);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

void EditFormButtonsCommand::redo()
{
    clearFormButtonStates();
    for(int i = 0; i < m_formButtons.size(); i++)
    {
        bool checked = m_newButtonStates.at(i);
        if(checked)
        {
            m_formButtons.at(i)->setState(checked);
        }
    }

    Okular::NormalizedRect boundingRect =
        buildBoundingRectangleForButtons(m_formButtons);
    moveViewportIfBoundingRectNotFullyVisible(boundingRect, m_docPriv,
                                              m_pageNumber);
    Q_EMIT m_docPriv->m_parent->formButtonsChangedByUndoRedo(m_pageNumber,
                                                             m_formButtons);
    m_docPriv->notifyFormChanges(m_pageNumber);
}

bool EditFormButtonsCommand::refreshInternalPageReferences(
    const QVector<Okular::Page*>& newPagesVector)
{
    const QList<FormFieldButton*> oldFormButtons = m_formButtons;
    m_formButtons.clear();
    for(FormFieldButton* oldFormButton : oldFormButtons)
    {
        FormFieldButton* button = dynamic_cast<FormFieldButton*>(
            Okular::PagePrivate::findEquivalentForm(
                newPagesVector[m_pageNumber], oldFormButton));
        if(!button)
        {
            return false;
        }
        m_formButtons << button;
    }

    return true;
}

void EditFormButtonsCommand::clearFormButtonStates()
{
    for(FormFieldButton* formButton : qAsConst(m_formButtons))
    {
        formButton->setState(false);
    }
}

}  // namespace Okular
