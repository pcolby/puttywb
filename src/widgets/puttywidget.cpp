#include "puttywidget.h"

#include "puttyinstance.h"

#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include <QWidget>

// TODO: apply this.
#define MAX_ADOPTIONAL_TIME 1000 ///< Maximum time to spend trying to adopt the PuTTY window.

#define TITLE_CHECK_INTERVAL 100 ///< Milliseconds between PuTTY title checks.

PuttyWidget::PuttyWidget(QTextEdit *te, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), putty(NULL), titleCheckTimerId(0), te(te)
{
    // Create a new PuTTY instance.
    // TODO: Remove these hard-coded values sometime.
    putty = new PuttyInstance(tr("C:\\Program Files (x86)\\PuTTY\\putty.exe"),
                              tr("putty.exe -P 2022 paul@localhost"));
    if (putty->isNull()) {
        // Failed to create, so show an error.
    } else {
        // Try to adopt the PuTTY instance's window.
        adoptPuttyWindow(); // Will schedule it's own retry if it fails.
    }

    // Set this widget's focus policy (required to receive title bar clicks).
    setFocusPolicy(Qt::WheelFocus); // Qt::WheelFocus is all-encompassing.
}

/* Qt event overrides */

void PuttyWidget::changeEvent(QEvent *event) {
    te->append(tr("change event"));
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowTitleChange) {
        emit windowTitleChanged(windowTitle());
    }
}

// Coming soon...
/*void PuttyWidget::enterEvent(QEvent *event) {
    QWidget::enterEvent(event);
}*/

// TODO: remove this function... it's just here for the early dev stage.
bool PuttyWidget::event(QEvent *event) {
    if (event->type() != QEvent::Timer)
        te->append(tr("event %1").arg((int)event->type()));
    return QWidget::event(event);
}

// We need this to handle title bar clicks.
void PuttyWidget::focusInEvent(QFocusEvent *event) {
    te->append(tr("focus in event"));
    QWidget::focusInEvent(event);
    if (!putty->setFocus())
        checkIfPuttyIsClosed();
}

void PuttyWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!putty->resize(event->size()))
        checkIfPuttyIsClosed();
}

void PuttyWidget::timerEvent(QTimerEvent * event) {
    if (event->timerId() == titleCheckTimerId)
        setWindowTitle(putty->windowTitle());
    else QWidget::timerEvent(event); // Not one of ours.
}

bool PuttyWidget::winEvent(MSG *message, long *result) {
    te->append(tr("win event 0x%1").arg(message->message,0,16));
    switch (message->message) {
        case WM_PARENTNOTIFY:
            te->append(tr("win event WM_PARENTNOTIFY %1 %2").arg(message->wParam,0,16).arg(message->lParam,0,16));
            break;
        case WM_MOUSEACTIVATE:
            if (!putty->setFocus())
                checkIfPuttyIsClosed();
            //*result = MA_ACTIVATE;
            //return false;
            break;
    }
    return QWidget::winEvent(message, result);
}

/* Protected slots */

void PuttyWidget::checkIfPuttyIsClosed() {
    if (putty->isWindowClosed())
        emit puttyClosed();
}

void PuttyWidget::adoptPuttyWindow() {
    // Try to adopt the PuTTY instance's window.
    if (putty->adopt(this)) {
        putty->resize(size());
        setWindowTitle(putty->windowTitle());
        titleCheckTimerId = startTimer(TITLE_CHECK_INTERVAL);
    } else {
        // If not, try again later.
        QTimer::singleShot(0, this, SLOT(adoptPuttyWindow()));
    }
}
