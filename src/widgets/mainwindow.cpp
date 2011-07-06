#include "mainwindow.h"

#include <QDockWidget>
#include <QMessageBox>
#include <QTimer>

#include <Windows.h>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    // Set our central widget.
    setCentralWidget(new QWidget());

    // Customise the docking behaviour.
    setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowNestedDocks|
                   QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

    // Add a couple of dummy PuTTY instances for initial prototyping.
    addPuTTY();
    addPuTTY();
}

/* Qt event overrides */

/* Protected slots */

/* Private functions */

// This is just a temporary (prototype) function - this code will be moved to a dedicted widget class later.
void MainWindow::addPuTTY() {
    // Setup
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

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

    QDockWidget *dw = new QDockWidget(tr("PuTTY"));
    dw->setWidget(new QWidget());
    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    SetParent(hWnd, dw->widget()->winId());

    addDockWidget(Qt::BottomDockWidgetArea, dw);
}

/* Private slots */
