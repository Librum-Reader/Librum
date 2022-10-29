#pragma once
#include <QKeyEvent>
#include <QKeySequence>
#include <QQuickItem>

namespace cpp_elements
{

class KeySequenceRecorder : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString originalSequence READ originalSequence WRITE
                   setOriginalSequence NOTIFY originalSequenceChanged)
    Q_PROPERTY(QString currentSequence READ currentSequence NOTIFY
                   currentSequenceChanged)

public:
    QString originalSequence() const;
    void setOriginalSequence(const QString& originalSequence);

    QString currentSequence() const;

public slots:
    void resetSequence();

signals:
    void originalSequenceChanged();
    void currentSequenceChanged();
    void returnPressed();
    void escapePressed();
    void reset();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    void setCurrentSequence(const QString& newSequence);
    bool sequenceIsAllModifiers(int keySequence);

    QKeySequence m_originalSequence;
    QKeySequence m_currentSequence;
};

}  // namespace cpp_elements