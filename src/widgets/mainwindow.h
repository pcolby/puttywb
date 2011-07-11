#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

  private:
    struct Menus {
        QMenu *tools, *help;
    } menus;

    struct Actions {
        QAction *options;
        QAction *updates, *about;
    } actions;

  public:
    MainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    virtual void closeEvent(QCloseEvent *event);

  protected slots:
    virtual void about();
    virtual void removePuttyWidget();

  private:
    void addPuTTY(QTextEdit *te);
    void createActions();
    void createMenus();
};

#endif // __MAIN_WINDOW_H__
