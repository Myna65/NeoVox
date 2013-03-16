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
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QtSql>
#include <QDebug>
#include "mainwindow.h"
#include "language.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    Language l;
    if(!l.exec())
    {
        return 0;
    }
    QString langue(l.getCh().c_str());
    QTranslator translator,t2;
    translator.load("qt_fr");
    t2.load("Vox_"+langue);
    a.installTranslator(&translator);
    a.installTranslator(&t2);
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    /*db.setHostName("localhost");
    db.setUserName("vox_user");
    db.setPassword("18301848");
    db.setDatabaseName("Vox");*/
    db.setDatabaseName("Vox_"+langue+".db");
    if(!(db.open()))
    {
        QMessageBox::warning(0, "Erreur lors de l'ouverture du fichier de données", db.lastError().text());
    }
    db.exec("PRAGMA synchronous = OFF;");
    MainWindow* w=MainWindow::Instance();
    w->show();
    int ret=a.exec();
    db.close();
    return ret;
}
