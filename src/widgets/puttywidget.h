#ifndef __PUTTY_WIDGET_H__
#define __PUTTY_WIDGET_H__

#include <QWidget>

class PuTTYWidget : public QWidget {
    Q_OBJECT

  private:
    WId puttyWinId;
    int titleCheckTimerId;

  public:
    PuTTYWidget(QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent * event);

  private slots:
    void foo();

  signals:
    void windowTitleChanged(const QString &title);
};

#endif // __PUTTY_WIDGET_H__
