#include "puttywidget.h"

#include <QWidget>
#include <QTimer>

#include <Windows.h>

PuTTYWidget::PuTTYWidget(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags) {
    // TODO: replace this with an appropraite event handler (just need to figure out what the
    // appropraite event would be!
    QTimer::singleShot(0, this, SLOT(foo()));
}

void PuTTYWidget::foo() {
    // Setup
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // This will prevent the PuTTY window from being shown on startup, but will break
    // the SetWindowLongPtr below (probably easy to fix, just haven't yet).
    //si.wShowWindow = SW_HIDE;
    //si.dwFlags = STARTF_USESHOWWINDOW;

    PROCESS_INFORMATION pi;
    WCHAR cla[1024] = TEXT("putty.exe paul@10.0.0.3");
    CreateProcess(
            TEXT("C:\\Program Files (x86)\\PuTTY\\putty.exe"),
            cla,
            NULL, NULL, FALSE,
            0,
            NULL, NULL,
            &si, &pi
            );

    HWND hWnd = NULL;
    while (hWnd == NULL) {
        HWND hEnumWnd = NULL;
        while ((hEnumWnd = FindWindowEx(NULL, hEnumWnd, NULL, NULL)) != NULL) {
            DWORD pid;
            DWORD tid = GetWindowThreadProcessId(hEnumWnd, &pid);
            if (pid == pi.dwProcessId) {
                WCHAR title[1024];
                GetWindowText(hEnumWnd, title, 1023);
                if (QString::fromUtf16((const ushort *)title).startsWith(QString::fromLatin1("paul"))) {
                    hWnd = hEnumWnd;
                }
            }
        }
        Sleep(50);
    }

    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    SetParent(hWnd, winId());
}
