#include "mainwindow.h"
#include "puttywidget.h"

#include <QDockWidget>
#include <QMessageBox>
#include <QTextEdit>
#include <QTimer>

#include <Windows.h>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    // Set our central widget.
    QTextEdit *te = new QTextEdit();
    setCentralWidget(te);

    // Customise the docking behaviour.
    setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowNestedDocks|
                   QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

    // Add a couple of dummy PuTTY instances for initial prototyping.
    addPuTTY(te);
    addPuTTY(te);
}

/* Qt event overrides */

/* Protected slots */

/* Private functions */

// This is just a temporary (prototype) function - this code will be moved to a dedicted widget class later.
void MainWindow::addPuTTY(QTextEdit *te) {
    PuttyWidget *putty = new PuttyWidget(te);
    QDockWidget *dw = new QDockWidget(tr("PuTTY"));
    dw->setWidget(putty);
    dw->setFocusPolicy(Qt::WheelFocus);
    dw->setFocusProxy(putty);
    connect(putty, SIGNAL(windowTitleChanged(QString)), dw, SLOT(setWindowTitle(QString)));
    addDockWidget(Qt::BottomDockWidgetArea, dw);
    //addDockWidget(Qt::BottomDockWidgetArea, new PuttyDockWidget());
}

/* Private slots */
