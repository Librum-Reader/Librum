/*
    SPDX-FileCopyrightText: 2018 Intevation GmbH <intevation@intevation.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QApplication>
#include "../form.h"
#include "event_p.h"

using namespace Okular;

class Event::Private
{
public:
    explicit Private(EventType eventType) :
        m_target(nullptr),
        m_targetPage(nullptr),
        m_source(nullptr),
        m_sourcePage(nullptr),
        m_eventType(eventType),
        m_returnCode(false),
        m_shiftModifier(false),
        m_willCommit(false)
    {
    }

    void* m_target;
    Page* m_targetPage;
    FormField* m_source;
    Page* m_sourcePage;
    EventType m_eventType;
    QString m_targetName;
    QVariant m_value;
    bool m_returnCode;
    bool m_shiftModifier;
    bool m_willCommit;
};

Event::Event() :
    d(new Private(UnknownEvent))
{
}

Event::Event(EventType eventType) :
    d(new Private(eventType))
{
}

Event::EventType Event::eventType() const
{
    return d->m_eventType;
}

QString Event::name() const
{
    switch(d->m_eventType)
    {
    case(FieldCalculate):
        return QStringLiteral("Calculate");
    case(FieldFormat):
        return QStringLiteral("Format");
    case(FieldKeystroke):
        return QStringLiteral("Keystroke");
    case(FieldFocus):
        return QStringLiteral("Focus");
    case(FieldValidate):
        return QStringLiteral("Validate");
    case(UnknownEvent):
    default:
        return QStringLiteral("Unknown");
    }
}

QString Event::type() const
{
    switch(d->m_eventType)
    {
    case(FieldCalculate):
    case(FieldFormat):
    case(FieldKeystroke):
    case(FieldFocus):
    case(FieldValidate):
        return QStringLiteral("Field");
    case(UnknownEvent):
    default:
        return QStringLiteral("Unknown");
    }
}

QString Event::targetName() const
{
    if(!d->m_targetName.isNull())
    {
        return d->m_targetName;
    }

    return QStringLiteral("JavaScript for: ") + type() + name();
}

void Event::setTargetName(const QString& val)
{
    d->m_targetName = val;
}

FormField* Event::source() const
{
    return d->m_source;
}

void Event::setSource(FormField* val)
{
    d->m_source = val;
}

Page* Event::sourcePage() const
{
    return d->m_sourcePage;
}

void Event::setSourcePage(Page* val)
{
    d->m_sourcePage = val;
}

void* Event::target() const
{
    return d->m_target;
}

void Event::setTarget(void* target)
{
    d->m_target = target;
}

Page* Event::targetPage() const
{
    return d->m_targetPage;
}

void Event::setTargetPage(Page* val)
{
    d->m_targetPage = val;
}

QVariant Event::value() const
{
    return d->m_value;
}

void Event::setValue(const QVariant& val)
{
    d->m_value = val;
}

bool Event::returnCode() const
{
    return d->m_returnCode;
}

void Event::setReturnCode(bool returnCode)
{
    d->m_returnCode = returnCode;
}

bool Event::shiftModifier() const
{
    return d->m_shiftModifier;
}

void Event::setShiftModifier(bool shiftModifier)
{
    d->m_shiftModifier = shiftModifier;
}

bool Event::willCommit() const
{
    return d->m_willCommit;
}

void Event::setWillCommit(bool willCommit)
{
    d->m_willCommit = willCommit;
}

// static
std::shared_ptr<Event> Event::createFormCalculateEvent(
    FormField* target, Page* targetPage, FormField* source, Page* sourcePage,
    const QString& targetName)
{
    std::shared_ptr<Event> ret(new Event(Event::FieldCalculate));
    ret->setSource(source);
    ret->setSourcePage(sourcePage);
    ret->setTarget(target);
    ret->setTargetPage(targetPage);
    ret->setTargetName(targetName);

    FormFieldText* fft = dynamic_cast<FormFieldText*>(target);
    if(fft)
    {
        ret->setValue(QVariant(fft->text()));
    }
    return ret;
}

// static
std::shared_ptr<Event> Event::createFormatEvent(FormField* target,
                                                Page* targetPage,
                                                const QString& targetName)
{
    std::shared_ptr<Event> ret(new Event(Event::FieldFormat));
    ret->setTarget(target);
    ret->setTargetPage(targetPage);
    ret->setTargetName(targetName);

    FormFieldText* fft = dynamic_cast<FormFieldText*>(target);
    if(fft)
    {
        ret->setValue(QVariant(fft->text()));
    }
    return ret;
}

// static
std::shared_ptr<Event> Event::createKeystrokeEvent(FormField* target,
                                                   Page* targetPage)
{
    std::shared_ptr<Event> ret(new Event(Event::FieldKeystroke));
    ret->setTarget(target);
    ret->setTargetPage(targetPage);

    FormFieldText* fft = dynamic_cast<FormFieldText*>(target);
    if(fft)
    {
        ret->setReturnCode(true);
        ret->setValue(QVariant(fft->text()));
    }
    return ret;
}

std::shared_ptr<Event> Event::createFormFocusEvent(FormField* target,
                                                   Page* targetPage,
                                                   const QString& targetName)
{
    std::shared_ptr<Event> ret(new Event(Event::FieldFocus));
    ret->setTarget(target);
    ret->setTargetPage(targetPage);
    ret->setTargetName(targetName);
    ret->setShiftModifier(QApplication::keyboardModifiers() &
                          Qt::ShiftModifier);

    FormFieldText* fft = dynamic_cast<FormFieldText*>(target);
    if(fft)
    {
        ret->setValue(QVariant(fft->text()));
    }
    return ret;
}

std::shared_ptr<Event> Event::createFormValidateEvent(FormField* target,
                                                      Page* targetPage,
                                                      const QString& targetName)
{
    std::shared_ptr<Event> ret(new Event(Event::FieldValidate));
    ret->setTarget(target);
    ret->setTargetPage(targetPage);
    ret->setTargetName(targetName);
    ret->setShiftModifier(QApplication::keyboardModifiers() &
                          Qt::ShiftModifier);

    FormFieldText* fft = dynamic_cast<FormFieldText*>(target);
    if(fft)
    {
        ret->setValue(QVariant(fft->text()));
        ret->setReturnCode(true);
    }
    return ret;
}
