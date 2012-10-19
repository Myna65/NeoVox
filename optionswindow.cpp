#include "optionswindow.h"
#include "ui_optionswindow.h"
#include "mainwindow.h"
#include <QtCore/qsettings.h>

optionswindow::optionswindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionswindow)
{
    ui->setupUi(this);
    QSettings set("Vox","Vox");
    ui->checkBox->setChecked(set.value("5").toBool());
}

optionswindow::~optionswindow()
{
    delete ui;
}
void optionswindow::open()
{
    int ret=exec();
    if(ret==QDialog::Accepted)
    {
        QSettings set("Vox","Vox");
        set.setValue("5",ui->checkBox->isChecked());
        MainWindow::Instance()->opt.f5=ui->checkBox->isChecked();
    }
}
