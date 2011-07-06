#ifndef __PUTTY_WIDGET_H__
#define __PUTTY_WIDGET_H__

#include <QWidget>

class PuTTYWidget : public QWidget {
    Q_OBJECT

  public:
    PuTTYWidget(QWidget *parent=0, Qt::WindowFlags flags=0);
private slots:
    void foo();
};

#endif // __PUTTY_WIDGET_H__
