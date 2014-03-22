#ifndef __PUTTY_INSTANCE_H__
#define __PUTTY_INSTANCE_H__

#include "abstractputtyinstance.h"

#include <qt_windows.h>

class PuttyInstance : public AbstractPuttyInstance {
    private:
        PROCESS_INFORMATION processInfo;
        HWND windowHandle, originalParent;
        LONG_PTR originalLongPtr;
        DWORD error;

    public:
        PuttyInstance(const QString &command, const QString &arguments);

        virtual bool isNull();
        virtual bool isValid();

        virtual bool findWindow();
        virtual bool isWindowClosed();

        virtual bool adopt(QWidget *parent);
        virtual bool disown();

        virtual bool resize(const QSize &size);
        virtual bool setFocus();

        virtual QString windowTitle();

        virtual int lastError();
        virtual QString lastErrorMessage();

    protected:
        static QString errorMessage(const DWORD error);
};

#endif // __PUTTY_INSTANCE_H__
