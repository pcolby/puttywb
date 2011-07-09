#include "mainwindow.h"
#include "puttywidget.h"

#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QTextEdit>
#include <QTimer>

#ifdef DEBUG
#define VERSION_STRING_REGEX "([^.]+\\.[^.]+\\.[^.]+\\.[^.]+)"
#else
#define VERSION_STRING_REGEX "([^.]+\\.[^.]+\\.[^.]+)\\."
#endif // DEBUG

#define SETTINGS_GEOMETRY QLatin1String("geometry")

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    // Set the main window title.
    QRegExp versionMatch(QLatin1String(VERSION_STRING_REGEX));
    const QString versionStr=(QApplication::applicationVersion().contains(versionMatch)) ? versionMatch.cap(1) : QString::null;
    setWindowTitle(tr("%1 %2").arg(QApplication::applicationName()).arg(versionStr));

    // Add the main menu.
    QMenu *profileMenu=menuBar()->addMenu(tr("&Tools"));
    profileMenu->addAction(tr("&Options..."))->setEnabled(false); // Not implemented yet.
    QMenu *helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&Check for updates..."))->setEnabled(false); // Not implemented yet.
    QAction *aboutAction=helpMenu->addAction(tr("&About %1...").arg(QApplication::applicationName()));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    // Set our central widget.
    QTextEdit *te = new QTextEdit();
    setCentralWidget(te);

    // Customise the docking behaviour.
    setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowNestedDocks|
                   QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

    // Add a couple of dummy PuTTY instances for initial prototyping.
    addPuTTY(te);
    addPuTTY(te);

    // Restore the window's previous size and position.
    QSettings settings;
    QVariant geometry=settings.value(SETTINGS_GEOMETRY);
    if (geometry.isValid()) restoreGeometry(geometry.toByteArray());
    else setGeometry(40,40,800,550); // Default to 800x550, at position (40,40).
}

/* Qt event overrides */

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the window's current size and position.
    QSettings settings;
    settings.setValue(SETTINGS_GEOMETRY,saveGeometry());
    QMainWindow::closeEvent(event);
}

/* Protected slots */

void MainWindow::about() {
    // Just a placeholder "About" dialog for now.
    QMessageBox::about(this,tr("About %1").arg(QApplication::applicationName()),
                       tr("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
}

void MainWindow::removePuttyWidget() {
    removeDockWidget(static_cast<QDockWidget *>(sender()->parent()));
}

/* Private functions */

// This is just a temporary (prototype) function - this code will be moved to a dedicted widget class later.
void MainWindow::addPuTTY(QTextEdit *te) {
    PuttyWidget *putty = new PuttyWidget(te);
    QDockWidget *dw = new QDockWidget(tr("PuTTY"));
    dw->setWidget(putty);
    dw->setFocusPolicy(Qt::WheelFocus);
    dw->setFocusProxy(putty);
    connect(putty, SIGNAL(puttyClosed()), this, SLOT(removePuttyWidget()));
    connect(putty, SIGNAL(windowTitleChanged(QString)), dw, SLOT(setWindowTitle(QString)));
    addDockWidget(Qt::BottomDockWidgetArea, dw);
}

/* Private slots */
