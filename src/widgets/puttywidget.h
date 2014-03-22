#ifndef __PUTTY_WIDGET_H__
#define __PUTTY_WIDGET_H__

#include "abstractputtyinstance.h"

#include <QWidget>
#include <QTextEdit>

class PuttyWidget : public QWidget {
    Q_OBJECT

  private:
    AbstractPuttyInstance *putty;
    int titleCheckTimerId;
    QTextEdit *te;

  public:
    PuttyWidget(QTextEdit *te, QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    virtual void changeEvent(QEvent *event);
    //virtual void enterEvent(QEvent *event);
    virtual bool event(QEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void timerEvent(QTimerEvent * event);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#else
    virtual bool winEvent(MSG *message, long *result);
#endif

  protected slots:
    void checkIfPuttyIsClosed();
    void adoptPuttyWindow();

  signals:
    void puttyClosed();
    void windowTitleChanged(const QString &title);
};

#endif // __PUTTY_WIDGET_H__
