#include "puttywidget.h"

#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include <QWidget>

#include <Windows.h>

PuttyWidget::PuttyWidget(QTextEdit *te, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), puttyWinId(NULL), te(te)
{
    // TODO: replace this with an appropraite event handler (just need to figure out what the
    // appropraite event would be!
    // Consider:
    //  1. create putty instance.
    //  2. try to embed putty instance.
    //  3. if not found, set single shot to check again later (with timeout, of course).
    //  ** also check on show event.
    QTimer::singleShot(0, this, SLOT(foo()));
    titleCheckTimerId = startTimer(100);
    setFocusPolicy(Qt::WheelFocus); // Required for title bar clicks.
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
    if (puttyWinId != NULL) {
        SetForegroundWindow(puttyWinId);
        SetFocus(puttyWinId);
    }
}

void PuttyWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (puttyWinId != NULL) {
        // Figure out the PuTTY window adornment dimensions.
        RECT rect;
        GetWindowRect(puttyWinId, &rect);
        POINT topLeft;
        topLeft.x = rect.left;
        topLeft.y = rect.top;
        ScreenToClient(puttyWinId, &topLeft);

        // Resize the PuTTY window to fit this widget.
        SendMessage(puttyWinId, WM_ENTERSIZEMOVE, 0, 0);
        SetWindowPos(puttyWinId, NULL, topLeft.x, topLeft.y, event->size().width() - ( 2 * topLeft.x),
                     event->size().height() - topLeft.y - topLeft.x, SWP_NOZORDER);
        SendMessage(puttyWinId, WM_EXITSIZEMOVE, 0, 0);
    }
}

void PuttyWidget::timerEvent(QTimerEvent * event) {
    if (event->timerId() == titleCheckTimerId) {
        if (puttyWinId != NULL) {
            TCHAR title[1024];
            SetLastError(ERROR_SUCCESS);
            int length = GetWindowText(puttyWinId, title, 1023);
            if (length == 0) {
                const DWORD dwError = GetLastError();
                te->append(tr("closed? %1").arg(dwError,0,16));
                if (dwError == ERROR_INVALID_WINDOW_HANDLE) {
                    puttyWinId = NULL;
                    emit puttyClosed();
                }
            }
            setWindowTitle(QString::fromUtf16((const ushort *)title));
        }
    } else QWidget::timerEvent(event); // Not one of ours.
}

bool PuttyWidget::winEvent(MSG *message, long *result) {
    te->append(tr("win event 0x%1").arg(message->message,0,16));
    switch (message->message) {
        case WM_PARENTNOTIFY:
            te->append(tr("win event WM_PARENTNOTIFY %1 %2").arg(message->wParam,0,16).arg(message->lParam,0,16));
            break;
        case WM_MOUSEACTIVATE:
            if (puttyWinId != NULL) {
                SetForegroundWindow(puttyWinId);
                SetFocus(puttyWinId);
                *result = MA_ACTIVATE;
                return false;
            }
            break;
    }
    return QWidget::winEvent(message, result);
}

/* Private slots */

void PuttyWidget::foo() {
    // Setup
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // This will prevent the PuTTY window from being shown on startup, but will break
    // the SetWindowLongPtr below (probably easy to fix, just haven't yet).
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;

    PROCESS_INFORMATION pi;
    WCHAR cla[1024] = TEXT("putty.exe paul@10.0.0.3");
    CreateProcess(TEXT("C:\\Program Files (x86)\\PuTTY\\putty.exe"),cla,NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    HWND hWnd = NULL;
    while (hWnd == NULL) {
        HWND hEnumWnd = NULL;
        while ((hEnumWnd = FindWindowEx(NULL, hEnumWnd, TEXT("putty"), NULL)) != NULL) {
            if (GetWindowThreadProcessId(hEnumWnd, NULL) == pi.dwThreadId)
                hWnd = hEnumWnd;
        }
        if (hWnd == NULL)
            Sleep(50);
    }

    // Adopt the PuTTY window as our own child.
    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    SetParent(hWnd, winId());
    puttyWinId = hWnd;
}
