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
    t2.load("Vox"+langue);
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
    MainWindow* w=MainWindow::Instance();
    w->show();
    int ret=a.exec();
    db.close();
    return ret;
}
