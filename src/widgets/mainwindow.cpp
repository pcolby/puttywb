#include "mainwindow.h"
#include "puttywidget.h"

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
    PuttyWidget *putty = new PuttyWidget();
    QDockWidget *dw = new QDockWidget(tr("PuTTY"));
    dw->setWidget(putty);
    connect(putty, SIGNAL(windowTitleChanged(QString)), dw, SLOT(setWindowTitle(QString)));
    addDockWidget(Qt::BottomDockWidgetArea, dw);
}

/* Private slots */
