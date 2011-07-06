#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

  private:
    void addPuTTY();
};

#endif // __MAIN_WINDOW_H__
