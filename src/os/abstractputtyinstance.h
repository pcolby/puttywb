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
