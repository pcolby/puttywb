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

#ifndef __VERSION_INFO_H__
#define __VERSION_INFO_H__
#include <QString>

#ifdef Q_OS_WIN // VersionInfo is a Windows-only class.
class VersionInfo {
  public:
    static bool getAppVersion(qint16 &major, qint16 &minor, qint16 &error, qint16 &build);
    static QString getAppVersionStr();
};
#endif // Q_OS_WIN

#endif // __VERSION_INFO_H__
