#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

  private:
    void addPuTTY(QTextEdit *te);
};

#endif // __MAIN_WINDOW_H__
