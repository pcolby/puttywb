#ifndef __PUTTY_WIDGET_H__
#define __PUTTY_WIDGET_H__

#include <QWidget>
#include <QTextEdit>

class PuttyWidget : public QWidget {
    Q_OBJECT

  private:
    WId puttyWinId;
    int titleCheckTimerId;
    QTextEdit *te;

  public:
    PuttyWidget(QTextEdit *te, QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    virtual void changeEvent(QEvent *event);
    virtual bool event(QEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void timerEvent(QTimerEvent * event);
    virtual bool winEvent(MSG *message, long *result);

  private slots:
    void foo();
    void foo2();

  signals:
    void windowTitleChanged(const QString &title);
};

#endif // __PUTTY_WIDGET_H__
