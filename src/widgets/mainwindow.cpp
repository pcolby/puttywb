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
    Sleep(1000); // TODO: Replace this with a semi-busy wait / check cycle.

    QDockWidget *dw = new QDockWidget(tr("PuTTY"));
    dw->setWidget(new QWidget());
    HWND hWnd = NULL;
    do {
        hWnd = FindWindowEx (NULL, hWnd, NULL, NULL);
        if (hWnd != NULL) {
            //QMessageBox::information(0,tr("found"),tr("pid: %1").arg(GetWindowThreadProcessId(hWnd, NULL)));
            DWORD pid;
            DWORD tid = GetWindowThreadProcessId(hWnd, &pid);
            if (pid == pi.dwProcessId) {
                WCHAR title[1024] = TEXT("123");
                GetWindowText(hWnd, title, 1023);
                HWND hOldWnd = 0;//= SetParent(hWnd, dw->winId());
                if (QString::fromUtf16((const ushort *)title).startsWith(QString::fromLatin1("paul"))) {
                    SetWindowLongPtr(hWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
                    HWND hOldWnd = SetParent(hWnd, dw->widget()->winId());
                    for (HWND w = FindWindowEx(hWnd, NULL, NULL, NULL); w!=NULL; w=FindWindowEx(hWnd,w,NULL,NULL)) {
                        WCHAR title[1024] = TEXT("123");
                        GetWindowText(w, title, 1023);
                        QMessageBox::information(0,tr("found child"),QString::fromUtf16((const ushort *)title));
                    }
                }
            }
        }
    } while (hWnd != NULL);

    addDockWidget(Qt::BottomDockWidgetArea, dw);
}

/* Private slots */
