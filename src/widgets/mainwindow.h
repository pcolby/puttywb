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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

  private:
    struct Menus {
        QMenu *tools, *help;
    } menus;

    struct Actions {
        QAction *options;
        QAction *updates, *about;
    } actions;

  public:
    MainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

  protected:
    virtual void closeEvent(QCloseEvent *event);

  protected slots:
    virtual void about();
    virtual void removePuttyWidget();

  private:
    void addPuTTY(QTextEdit *te);
    void createActions();
    void createMenus();
};

#endif // __MAIN_WINDOW_H__
