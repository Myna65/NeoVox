////////////////////////////////////////////////////////////////////////////
//    NeoVox, apprentissage de vocabulaire de langue étrangère.
//    Copyright (C) 2012-2013 Meynaert Nathan
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choixtestwindow.h"
#include "resultatwindow.h"
#include "introwindow.h"
#include "optionswindow.h"
#include "about.h"
#include "welcome.h"
#include <QtGui/qactiongroup.h>
#include <QtCore/qsettings.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/QCloseEvent>
#include <QtGui/qdesktopservices.h>
#include <QtGui/qfiledialog.h>
#include <QtCore/qurl.h>
#include <QtXml/QtXml>
#include <QtSql/QtSql>

MainWindow* MainWindow::instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QActionGroup* group = new QActionGroup(this), *group2 = new QActionGroup(this);
    ui->setupUi(this);
    ui->actionFran_ais_N_erlandais->setActionGroup(group);
    ui->actionN_erlandais_Fran_ais->setActionGroup(group);
    ui->actionTest_classique->setActionGroup(group2);
    ui->actionTest_rapide->setActionGroup(group2);
    setWindowIcon(QPixmap(":/ico/ico.png"));
    connect(ui->actionTenter_un_test,SIGNAL(triggered()),this,SLOT(openTest()));
    connect(ui->actionR_sultats,SIGNAL(triggered()),this,SLOT(openResult()));
    connect(ui->actionNouvelle_s_rie,SIGNAL(triggered()),this,SLOT(openIntro()));
    connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(openOpt()));
    connect(ui->action_propos_de_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->action_propos_de_NeoVox,SIGNAL(triggered()),this,SLOT(openAbout()));
    connect(ui->actionEditer_une_s_rie,SIGNAL(triggered()),this,SLOT(openModif()));
    connect(ui->actionRenum_roter_les_s_ries,SIGNAL(triggered()),this,SLOT(openRenum()));
    connect(ui->actionSupprimer_une_s_rie,SIGNAL(triggered()),this,SLOT(openSuppr()));
    connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(ui->actionImprimer_une_le_on,SIGNAL(triggered()),this,SLOT(openImpr()));
    connect(ui->actionExporter_du_voabulaire,SIGNAL(triggered()),this,SLOT(openExport()));
    connect(ui->actionAide,SIGNAL(triggered()),this,SLOT(openAide()));
    connect(ui->actionImporter_du_vocabulaire,SIGNAL(triggered()),this,SLOT(openImport()));
    QSettings set("NeoVox","NeoVox");
    opt.f5=set.value("5").toBool();
    setDefault();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openTest()
{

    setCentralWidget(new ChoixTestWindow(this));
}
void MainWindow::openResult()
{

    setCentralWidget(new ResultatWindow(this));
}
void MainWindow::openIntro()
{
    setCentralWidget(new Introwindow(this));
}

MainWindow* MainWindow::Instance()
{
    if(instance==0)
    {
        instance = new MainWindow();
    }
    return instance;
}
QStatusBar* MainWindow::getStatusBar()
{
    return ui->statusBar;
}
bool MainWindow::isSensFr()
{
    return ui->actionFran_ais_N_erlandais->isChecked();
}
bool MainWindow::isTestClassique()
{
    return ui->actionTest_classique->isChecked();
}
void MainWindow::openOpt()
{
    optionswindow optw;
    optw.open();
}
void MainWindow::openAbout()
{
    About ab(this);
    ab.exec();
}
void MainWindow::openModif()
{
    setCentralWidget(new ChoixTestWindow(this,1));
}
void MainWindow::openRenum()
{
    setCentralWidget(new ChoixTestWindow(this,2));
}
void MainWindow::openSuppr()
{
    setCentralWidget(new ChoixTestWindow(this,3));
}
void MainWindow::setDefault()
{
    setCentralWidget(new Welcome(tr(":/png/nl.png")));
}
void MainWindow::openImpr()
{
    setCentralWidget(new ChoixTestWindow(this,4));
}
void MainWindow::openExport()
{
    setCentralWidget(new ChoixTestWindow(this,5));
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this,"Quitter ?",trUtf8("Êtes-vous sûr de vouloir quitter ?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
void MainWindow::openAide()
{
    QDesktopServices::openUrl(QUrl("file://"+QApplication::applicationDirPath()+"/Manuel.pdf"));
}
void MainWindow::openImport()
{
    QString path=QFileDialog::getOpenFileName(MainWindow::Instance(),"Choisir le fichier de vocabulaire",QDir::homePath(),"Fichiers XML (*.xml);;Tous les fichiers (*)");
    if(path=="")
        return;
    QDomDocument dom;
    QFile doc(path);
    if(!doc.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(MainWindow::Instance(),"Erreur",trUtf8("Impossible d'ouvrir le fichier spécifié.<br/> Vérifiez le chemin et les permissions du fichier"));
        doc.close();
        return;
    }
    if(!dom.setContent(&doc))
    {
        QMessageBox::critical(MainWindow::Instance(),"Erreur",trUtf8("Le fichier spécifié ne semble pas être un fichier XML valide."));
        doc.close();
        return;
    }
    doc.close();
    QDomElement voc=dom.firstChildElement("vocabulaire");
    if(voc.isNull())
    {
        QMessageBox::critical(MainWindow::Instance(),"Erreur",trUtf8("Le fichier XML spécifié ne semble pas être un fichier de vocabulaire"));
        return;
    }
    QDomElement ser=voc.firstChildElement("serie");
    for(;!ser.isNull();ser=ser.nextSiblingElement("serie"))
    {
        QSqlQuery req;
        int num;
        req.exec("SELECT MAX(num)+1 FROM Serie");
        if(req.next())
        {
            num=req.value(0).toInt();
            if(num==0)
                num++;
        }
        else
            num=1;
        req.prepare("INSERT INTO Serie (num,titre) VALUES (:n,:t)");
        req.bindValue(":t",ser.attribute("titre"));
        req.bindValue(":n",num);
        req.exec();
        req.prepare("SELECT id FROM Serie WHERE num=:n");
        req.bindValue(":n",num);
        req.exec();
        req.next();
        int id=req.value(0).toInt();
        QDomElement mot=ser.firstChildElement("mot");
        for(;!mot.isNull();mot=mot.nextSiblingElement("mot"))
        {
            req.prepare("INSERT INTO Mot (fr,nl,serie_id) VALUES (:f,:n,:s)");
            req.bindValue(":s",id);
            req.bindValue(":f",mot.attribute("fr"));
            req.bindValue(":n",mot.attribute("nl"));
            req.exec();
        }

    }
    MainWindow::Instance()->setDefault();
}
