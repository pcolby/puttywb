#ifndef __ABSTRACT_PUTTY_INSTANCE_H__
#define __ABSTRACT_PUTTY_INSTANCE_H__

#include <QWidget>
#include <QTextEdit>

class AbstractPuttyInstance {
    public:
        virtual bool isNull() = 0;
        virtual bool isValid() = 0;

        virtual bool findWindow() = 0;
        virtual bool isWindowClosed() = 0;

        virtual bool adopt(QWidget *parent) = 0;
        virtual bool disown() = 0;

        virtual bool resize(const QSize &size) = 0;
        virtual bool setFocus() = 0;

        virtual QString windowTitle() = 0;

        virtual int lastError() = 0;
        virtual QString lastErrorMessage() = 0;
};

#endif // __ABSTRACT_PUTTY_INSTANCE_H__
