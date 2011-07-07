#include "PuttyWidget.h"

#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include <QWidget>

#include <Windows.h>

PuttyWidget::PuttyWidget(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags), puttyWinId(NULL) {
    // TODO: replace this with an appropraite event handler (just need to figure out what the
    // appropraite event would be!
    QTimer::singleShot(0, this, SLOT(foo()));
    titleCheckTimerId = startTimer(1000);
}

/* Qt event overrides */

void PuttyWidget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowTitleChange) {
        emit windowTitleChanged(windowTitle());
    }
}

void PuttyWidget::resizeEvent(QResizeEvent *event) {
    //QWidget::resizeEvent(event);
    if (puttyWinId != NULL) {
        //SetWindowLongPtr(puttyWinId, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
        //SetParent(puttyWinId, winId());
        //SetWindowLongPtr(puttyWinId, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);

        // WINDOW.C
        //SetWindowLongPtr(puttyWinId, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);

        RECT r;
//        GetWindowRect(puttyWinId, &r);
        GetClientRect(puttyWinId, &r);
        //AdjustWindowRect();
//        ClientToParent();

        //QMessageBox::information(0, tr("blah"), tr("%1 %2 %3 %4").arg(r.left).arg(r.top).arg(r.right).arg(r.bottom));


        SendMessage(puttyWinId, WM_ENTERSIZEMOVE, 0, 0);
        SetWindowPos(puttyWinId, NULL, -8, -30, event->size().width()+8+8, event->size().height()+30+8,SWP_NOZORDER);
        //SetWindowPos(puttyWinId, NULL, r.left, r.top, 100,100, SWP_NOZORDER);//event->size().width()+8+8, event->size().height()+30+8,SWP_NOZORDER);
        //SendMessage(puttyWinId, WM_SIZE, 0, MAKEWORD(100, 100));
        //SetWindowLongPtr(puttyWinId, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
        //SendMessage(puttyWinId, WM_SIZE, SIZE_MAXIMIZED, MAKEWORD(100, 100));
        SendMessage(puttyWinId, WM_EXITSIZEMOVE, 0, 0);
        SendMessage(puttyWinId, 0x0180, 0, 0);

        //SetWindowLongPtr(puttyWinId, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
        //SetParent(puttyWinId, winId());
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

    //
    //RECT r;
    //GetClientRect(hWnd, &r);
    //POINT p1, p2;
    //ClientToScreen(hWnd, &p1);
    //ClientToScreen(hWnd, &p1);
    //QMessageBox::information(0, tr("blah"), tr("%1 %2 %3 %4 %5 %6").arg(r.left).arg(r.top).arg(r.right).arg(r.bottom)
      //                       .arg(p1.x).arg(p1.y));

    // Adopt the PuTTY window as our own child.
    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    SetParent(hWnd, winId());
    puttyWinId = hWnd;
}
