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
#include <QtCore/qurl.h>

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
    connect(ui->actionAide,SIGNAL(triggered()),this,SLOT(openAide()));
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
