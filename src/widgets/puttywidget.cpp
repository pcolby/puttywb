#include "PuttyWidget.h"

#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include <QWidget>

#include <Windows.h>

#define PUTTY_WINDOW_BORDER 1 ///< Number of pixels to leave visible around embedded PuTTY windows.

PuttyWidget::PuttyWidget(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags), puttyWinId(NULL) {
    // TODO: replace this with an appropraite event handler (just need to figure out what the
    // appropraite event would be!
    QTimer::singleShot(0, this, SLOT(foo()));
    titleCheckTimerId = startTimer(1000);
    setFocusPolicy(Qt::WheelFocus);
}

/* Qt event overrides */

void PuttyWidget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowTitleChange) {
        emit windowTitleChanged(windowTitle());
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
        SetWindowPos(puttyWinId, NULL,
                     topLeft.x + PUTTY_WINDOW_BORDER, topLeft.y + PUTTY_WINDOW_BORDER,
                     event->size().width() - 2 * ( topLeft.x + PUTTY_WINDOW_BORDER),
                     event->size().height() - topLeft.y - topLeft.x - ( 2 * PUTTY_WINDOW_BORDER ),
                     SWP_NOZORDER);
        SendMessage(puttyWinId, WM_EXITSIZEMOVE, 0, 0);

        SetFocus(puttyWinId); // TODO: fix up the focus handling (it's pretty broken right now).
    }
}

void PuttyWidget::timerEvent(QTimerEvent * event) {
    if (event->timerId() == titleCheckTimerId) {
        if (puttyWinId != NULL) {
            TCHAR title[1024];
            GetWindowText(puttyWinId, title, 1023);
            setWindowTitle(QString::fromUtf16((const ushort *)title));
        }
    } else QWidget::timerEvent(event); // Not one of ours.
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
        Sleep(50);
    }

    // Adopt the PuTTY window as our own child.
    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    SetParent(hWnd, winId());
    puttyWinId = hWnd;
}
