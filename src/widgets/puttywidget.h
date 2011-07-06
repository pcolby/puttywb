#ifndef __PUTTY_WIDGET_H__
#define __PUTTY_WIDGET_H__

#include <QWidget>

class PuTTYWidget : public QWidget {
    Q_OBJECT

  private:
    WId puttyWinId;

  public:
    PuTTYWidget(QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void foo();
};

#endif // __PUTTY_WIDGET_H__
