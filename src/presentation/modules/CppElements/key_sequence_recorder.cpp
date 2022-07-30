#include "key_sequence_recorder.hpp"


namespace cpp_elements
{

QString KeySequenceRecorder::originalSequence() const
{
    return m_originalSequence.toString();
}

void KeySequenceRecorder::setOriginalSequence(const QString& originalSequence)
{
    if (originalSequence == m_originalSequence.toString())
        return;
    
    m_originalSequence = QKeySequence(originalSequence, QKeySequence::PortableText);
    setCurrentSequence(QString());
    emit originalSequenceChanged();
}

QString KeySequenceRecorder::currentSequence() const
{
    return m_currentSequence.toString();
}

void KeySequenceRecorder::setCurrentSequence(const QString& newSequence)
{
    if (newSequence == m_currentSequence.toString())
        return;
    
    m_currentSequence = QKeySequence(newSequence, QKeySequence::PortableText);
    emit currentSequenceChanged();
}

bool KeySequenceRecorder::sequenceIsAllModifiers(int keySequence)
{
    return keySequence >= Qt::Key_Shift && keySequence <= Qt::Key_Alt;
}

void KeySequenceRecorder::resetSequence()
{
    setCurrentSequence(originalSequence());
    emit reset();
}

void KeySequenceRecorder::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        int modifiers(0);
        
        if(event->key() == Qt::Key_Return)
        {
            emit returnPressed();
            event->accept();
            return;
        }
        if(event->key() == Qt::Key_Escape)
        {
            emit escapePressed();
            event->accept();
            return;
        }
        if (event->modifiers().testFlag(Qt::ControlModifier))
            modifiers |= Qt::CTRL;
        if (event->modifiers().testFlag(Qt::ShiftModifier))
            modifiers |= Qt::SHIFT;
        if (event->modifiers().testFlag(Qt::AltModifier))
            modifiers |= Qt::ALT;
        if (event->modifiers().testFlag(Qt::MetaModifier))
            modifiers |= Qt::META;
        
        
        if (!sequenceIsAllModifiers(event->key()))
        {
            int keyCode(event->key() | modifiers);
            const QKeySequence sequence(keyCode);
            
            setCurrentSequence(sequence.toString());
        }
    }
    
    event->accept();
}

void KeySequenceRecorder::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
}

void KeySequenceRecorder::focusInEvent(QFocusEvent *event)
{
    event->accept();
}

void KeySequenceRecorder::focusOutEvent(QFocusEvent *event)
{
    event->accept();
}

} // namespace cpp_elements