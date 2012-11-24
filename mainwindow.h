////////////////////////////////////////////////////////////////////////////
//    <one line to give the program's name and a brief idea of what it does.>
//    Copyright (C) <year>  <name of author>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>

struct Options
{
    bool f5;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static MainWindow* Instance();
    QStatusBar* getStatusBar();
    bool isTestClassique();
    bool isSensFr();
    void setDefault();
    void closeEvent(QCloseEvent *event);
    Options opt;
private:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    static MainWindow* instance;
public slots:
    void openTest();
    void openResult();
    void openIntro();
    void openOpt();
    void openAbout();
    void openModif();
    void openRenum();
    void openSuppr();
    void openImpr();
    void openAide();
    void openExport();
    void openImport();
};

#endif // MAINWINDOW_H
