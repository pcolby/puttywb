/*
 *  Copyright 2011-2014 Paul Colby
 *
 *  This file is part of PuTTY Workbench (puttywb).
 *
 *  PuTTY Workbench is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PuTTY Workbench is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PuTTY Workbench.  If not, see <http://www.gnu.org/licenses/>.
 */

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
