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

#include "mainwindow.h"
#include "versioninfo.h"

#include <QApplication>
#include <QErrorMessage>
#include <QLocale>
#include <QTranslator>

/// @todo: Add svn Id's to everything.
/// @todo: Add GPL license to everything.
/// @todo: Add Qt and Oxygen-icons credits.

#ifdef DEBUG
  #include <QMessageBox>
  void msgHandler(QtMsgType type, const char *msg);
#endif // DEBUG

#define APPLICATION_NAME    QLatin1String("PuTTY Workbench") // Note, these values are used by the
#define ORGANISATION_NAME   QLatin1String("Paul Colby")      // QSettings default constructor, so
#define ORGANISATION_DOMAIN QLatin1String("colby.id.au")     // should not change between versions.

int main(int argc, char *argv[]) {
    // Setup the primary Qt application object.
    QApplication app(argc, argv);
    app.setApplicationName(APPLICATION_NAME);
    app.setOrganizationName(ORGANISATION_NAME);
    app.setOrganizationDomain(ORGANISATION_DOMAIN);
    app.setApplicationVersion(VersionInfo::getAppVersionStr());

    // Install the QErrorMessage class' Qt message handler.
    QErrorMessage::qtHandler();
    //qInstallMsgHandler(msgHandler); // Useful for setting debugging breakpoints only.

    // Try to load a localised translator.
    QTranslator translator;
    if (translator.load(QLocale::system().name(),app.applicationDirPath()+QLatin1String("/../i18n")))
        app.installTranslator(&translator);

    // Set the icon theme.
    QIcon::setThemeName(QLatin1String("oxygen"));

    // Instantiate the main window.
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

#ifdef DEBUG
// We should use QErrorMessage::qtHandler() in preference to this custom message handler.
// However, this handler is much easier to trace, which can be very useful for debugging.
void msgHandler(QtMsgType type, const char *msg) {
    QMessageBox::Icon icon = QMessageBox::NoIcon;
    QString title;
    switch (type) {
        case QtDebugMsg:
            title = QLatin1String("Debug");
            icon = QMessageBox::Information;
            break;
        case QtWarningMsg:
            title = QLatin1String("Warning");
            icon = QMessageBox::Warning;
            break;
        case QtCriticalMsg:
            title = QLatin1String("Critical");
            icon = QMessageBox::Critical;
            break;
        case QtFatalMsg:
            title = QLatin1String("Fatal");
            icon = QMessageBox::Critical;
            break;
    }
	// shortcut this next line?
    QString text=(strlen(msg)>1024) ? QString::fromUtf8(msg,1024)+QLatin1String(" ...") : QString::fromUtf8(msg); // Limit text msg to no more than 1024 characters.
    QMessageBox msgBox(icon,title,text);
    msgBox.exec();
}
#endif // DEBUG
