#include "resultatwindow.h"
#include "ui_resultatwindow.h"
#include <QtGui/QStandardItem>
#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>

ResultatWindow::ResultatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultatWindow)
{
    ui->setupUi(this);
    QSqlQuery q;
    q.exec("SELECT num,id,titre FROM Serie ORDER BY num");
    int i=0;
    while(q.next())
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString("%1 - %2").arg(q.value(0).toString()).arg(q.value(2).toString())));
        QSqlQuery req;
        req.prepare("SELECT SUM(ok),COUNT(ok),tps FROM Result INNER JOIN Test ON Test.id = Result.Test_id WHERE fini=1 AND mot_id IN (SELECT id FROM Mot WHERE serie_id=:s) GROUP BY test_id ORDER BY tps DESC LIMIT 3");
        req.bindValue(":s",q.value(1));
        req.exec();
        int tjus=0,ttot=0,j=0;
        while(req.next())
        {
            int tot=req.value(1).toInt(),jus=req.value(0).toInt();
            tjus++;
            ttot+=(100*jus/tot);
            QDateTime tps = req.value(2).toDateTime();
            j++;
            if(tot!=0)
            {
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString(trUtf8("%1/%2 : %3 % du %4 à %5")).arg(jus).arg(tot).arg(100*jus/tot).arg(tps.toString("dd/MM/yyyy")).arg(tps.toString("hh:mm:ss"))));
            }
        }
        j=4;
        if(tjus!=0)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString("%3 % ").arg(ttot/tjus)));
        }
        i++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

ResultatWindow::~ResultatWindow()
{
    delete ui;
}
