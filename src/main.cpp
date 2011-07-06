#include "versioninfo.h"

#include <QMainWindow>
#include <QApplication>
#include <QErrorMessage>
#include <QLocale>
#include <QTranslator>

#ifdef DEBUG
  #include <QMessageBox>
  void msgHandler(QtMsgType type, const char *msg);
#endif // DEBUG

#define APPLICATION_NAME    QString::fromAscii("WiTTY")             // Note, these values are used by the
#define ORGANISATION_NAME   QString::fromAscii("Paul Colby")        // QSettings default constructor, so
#define ORGANISATION_DOMAIN QString::fromAscii("witty.colby.id.au") // should not change between versions.

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

    // Instantiate the main window.
    QMainWindow mainWindow;
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
    QString text=(strlen(msg)>1024) ? QString::fromAscii(msg,1024)+QLatin1String(" ...") : QString::fromUtf8(msg); // Limit text msg to no more than 1024 characters.
    QMessageBox msgBox(icon,title,text);
    msgBox.exec();
}
#endif // DEBUG
