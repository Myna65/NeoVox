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
