/*
 *  Copyright 2011-2014 Paul Colby
 *
 *  This file is part of PuTTY Workbench (puttywb).
 *
 *  PuTTY Workbench is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PuTTY Workbench is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PuTTY Workbench.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "puttywidget.h"

#include "puttyinstance.h"

#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QResizeEvent>
#include <QWidget>

#define MAX_ADOPTIONAL_TIME  2500 ///< Maximum time to spend trying to adopt the PuTTY window.

#define TITLE_CHECK_INTERVAL  100 ///< Milliseconds between PuTTY title checks.

PuttyWidget::PuttyWidget(QTextEdit *te, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), putty(NULL), titleCheckTimerId(0), te(te)
{
    // Create a new PuTTY instance.
    // TODO: Remove these hard-coded values sometime.
    putty = new PuttyInstance(tr("C:\\Program Files (x86)\\PuTTY\\putty.exe"),
                              tr("putty.exe -P 2022 paul@localhost"));
    if (putty->isNull()) {
        QMessageBox messageBox(QMessageBox::Critical, tr("Error"), tr("Failed to create new PuTTY process."));
        messageBox.setDetailedText(tr("Error: 0x%1\n\nMessage: %2\n\nCommand: %3")
                                   .arg(putty->lastError(),0,16).arg(putty->lastErrorMessage())
                                   .arg(tr("filename / command line args go here...")));
        messageBox.exec();
    } else {
        // Try to adopt the PuTTY instance's window.
        QTimer::singleShot(500, this, SLOT(adoptPuttyWindow()));
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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool PuttyWidget::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_ASSERT(eventType == "windows_generic_MSG");
    MSG * const msg = reinterpret_cast<MSG *>(message);
#else
bool PuttyWidget::winEvent(MSG *message, long *result) {
    MSG * const msg = message;
#endif
    te->append(tr("win event 0x%1").arg(msg->message,0,16));
    switch (msg->message) {
        case WM_PARENTNOTIFY:
            te->append(tr("win event WM_PARENTNOTIFY %1 %2").arg(msg->wParam,0,16).arg(msg->lParam,0,16));
            break;
        case WM_MOUSEACTIVATE:
            if (!putty->setFocus())
                checkIfPuttyIsClosed();
            //*result = MA_ACTIVATE;
            //return false;
            break;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return QWidget::nativeEvent(eventType, message, result);
#else // Qt5's nativeEvent replaced Qt4's winEvent.
    return QWidget::winEvent(message, result);
#endif
}

/* Protected slots */

void PuttyWidget::checkIfPuttyIsClosed() {
    if (putty->isWindowClosed())
        emit puttyClosed();
}

void PuttyWidget::adoptPuttyWindow() {
    // Start a timer, to make sure we don't try forever.
    static QTime timer;
    if (timer.isNull())
        timer.start();

    // Try to adopt the PuTTY instance's window.
    if (putty->adopt(this)) {
        putty->resize(size());
        setWindowTitle(putty->windowTitle());
        titleCheckTimerId = startTimer(TITLE_CHECK_INTERVAL);
    } else if (timer.elapsed() < MAX_ADOPTIONAL_TIME) {
        // If adoption failed, try again later.
        QTimer::singleShot(0, this, SLOT(adoptPuttyWindow()));
    } else {
        // Adoption failed, and time is up :(
        QMessageBox::critical(this, tr("Error"), tr("Failed to adopt the created PuTTY instance."));
    }
}
