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
#include "testwindow.h"
#include "ui_testwindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>

TestWindow::TestWindow(QWidget *parent, QVector<int> t) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ok=true;
    suivant=false;
    fini=false;
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    sens=MainWindow::Instance()->isSensFr();
    rap=!MainWindow::Instance()->isTestClassique();
    lineEdit=ui->lineEdit;
    lineEdit_2=ui->lineEdit_2;
    if(rap)
    {
        ui->lineEdit_2->setReadOnly(true);
        ui->pushButton->setText(trUtf8("Afficher la réponse"));
    }
    if(!sens)
    {
        lineEdit=ui->lineEdit_2;
        lineEdit_2=ui->lineEdit;
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit_2->setReadOnly(true);
    }
    else
    {
        ui->lineEdit->setReadOnly(true);
        ui->lineEdit_2->setReadOnly(false);
    }
    QSqlQuery q;
    q.prepare("INSERT INTO Test (tps,fini) VALUES (:d,0)");
    q.bindValue(":d",QDateTime::currentDateTime());
    q.exec();
    q.exec("SELECT MAX(id) FROM Test");
    int id;
    if(q.next())
    {
        id =q.value(0).toInt();
        if(id==0)
            id=1;
    }
    else
    {
        id=1;
    }
    q.prepare("INSERT INTO Jeu (serie_id,test_id) VALUES (:s,:t)");
    q.bindValue(":t",id);
    for(int i=0;i<t.size();i++)
    {
        q.bindValue(":s",t[i]);
        q.exec();
    }
    q.prepare("SELECT fr,nl,id FROM Mot WHERE serie_id IN (SELECT serie_id FROM Jeu WHERE test_id=:t)");
    q.bindValue(":t",id);
    q.exec();
    while(q.next())
    {
        QSqlQuery req;
        req.prepare("SELECT ok FROM Result WHERE mot_id=:m  ORDER BY id DESC LIMIT 5");
        req.bindValue(":m",q.value(2));
        req.exec();
        int i=0,sum=0;
        while(i<5&&req.next())
        {
            sum+=req.value(0).toInt();
            i++;
        }
        if(!MainWindow::Instance()->opt.f5||i!=5||sum!=5)
        {
            fr.push_back(q.value(0).toString());
            nl.push_back(q.value(1).toString());
            ids.push_back(q.value(2).toInt());
        }
    }
    if(fr.size()==0)
    {
        ok=false;
        return;
    }
    motsuivant();
    MainWindow::Instance()->getStatusBar()->showMessage("");
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(ClicBouton()));
    connect(ui->lineEdit_2,SIGNAL(returnPressed()),this,SLOT(ClicBouton()));
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(ClicBouton()));
    tid=id;
    jus=0;
    tot=0;
}
TestWindow::~TestWindow()
{
    delete ui;
}
void TestWindow::motsuivant()
{
    if(nl.size()==0)
    {
        if((enl.size()!=0)&&(QMessageBox::question(MainWindow::Instance(),"Recommencer",trUtf8("Recommencer les mots erronés"),QMessageBox::No|QMessageBox::Yes)==QMessageBox::Yes))
        {
            if(fini==false)
            {
                fini=true;
                QSqlQuery q;
                q.exec("SELECT MAX(id) FROM Test");
                if(q.next())
                {
                    int id=q.value(0).toInt();
                    q.prepare("UPDATE Test SET fini=1 WHERE id=:i");
                    q.bindValue(":i",id);
                    q.exec();
                }
            }
            fr=efr;
            nl=enl;
            ids=eids;
            enl.clear();
            efr.clear();
            eids.clear();
            MainWindow::Instance()->getStatusBar()->showMessage(QString(trUtf8("Score : %1 %   Mots restants : %2   %3 Erreurs")).arg(100*jus/tot).arg(fr.size()).arg(efr.size()));
        }
        else
        {
            if(fini==false)
            {
                fini=true;
                QSqlQuery q;
                q.exec("SELECT MAX(id) FROM Test");
                if(q.next())
                {
                    int id=q.value(0).toInt();
                    q.prepare("UPDATE Test SET fini=1 WHERE id=:i");
                    q.bindValue(":i",id);
                    q.exec();
                }
            }
            QMessageBox::information(MainWindow::Instance(),"Fin",trUtf8("Le test est terminé"));
            MainWindow::Instance()->setDefault();
            if(!rap)
            {
                MainWindow::Instance()->getStatusBar()->showMessage(QString("Score final : %1 %").arg(100*jus/tot),3000);
            }
            return;
        }
    }
    int i=rand()%nl.size();
    lineEdit_2->setText("");
    QList<QString>::iterator it=fr.begin();
    std::advance(it,i);
    if(sens)
        lineEdit->setText(*it);
    curfr=*it;
    fr.erase(it);
    it=nl.begin();
    std::advance(it,i);
    curnl=*it;
    if(!sens)
        lineEdit->setText(*it);
    nl.erase(it);
    QList<int>::iterator it2;
    it2=ids.begin();
    std::advance(it2,i);
    curid=*it2;
    ids.erase(it2);
}
void TestWindow::ClicBouton()
{
    if(rap)
    {
        if(suivant)
        {
            motsuivant();
            suivant=false;
            ui->pushButton->setText(trUtf8("Afficher la réponse"));
        }
        else
        {
            suivant=true;
            ui->pushButton->setText("Mot Suivant");
            if(sens)
                lineEdit_2->setText(curnl);
            else
                lineEdit_2->setText(curfr);
        }
    }
    else
    {
        if(!suivant)
        {
            QSqlQuery q;
            if(sens)
            {
                q.prepare("INSERT INTO Result (mot_id,test_id,ok) VALUES (:mi,:ti,:o)");
                q.bindValue(":ti",tid);
                q.bindValue(":mi",curid);
            }
            tot++;
            QString ref,ref2;
            ref2=lineEdit_2->text().toUpper();
            if(sens)
                ref=curnl.toUpper();
            else
                ref=curfr.toUpper();
            if(ref==ref2)
            {
                jus++;
                q.bindValue(":o",1);
            }
            else
            {
                QString aafficher;
                if(sens)
                    aafficher=curnl;
                else
                    aafficher=curfr;
                ui->pushButton->setText("Mot suivant");
                ui->label_3->setVisible(true);
                ui->label->setVisible(true);
                ui->label_3->setText(aafficher);
                efr.push_back(curfr);
                enl.push_back(curnl);
                eids.push_back(curid);
                q.bindValue(":o",0);
                suivant=true;
            }
            MainWindow::Instance()->getStatusBar()->showMessage(QString("Score : %1 %   Mots restants : %2   %3 Erreurs").arg(100*jus/tot).arg(fr.size()).arg(efr.size()));
            if(sens)
                q.exec();
            if(ref==ref2)
            {
                motsuivant();
            }
        }
        else
        {
            ui->label->setVisible(false);
            ui->label_3->setVisible(false);
            ui->pushButton->setText("Valider");
            motsuivant();
            suivant=false;
        }
    }
}
