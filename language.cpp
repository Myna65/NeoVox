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
#include "language.h"
#include "ui_language.h"

Language::Language(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Language)
{
    ui->setupUi(this);
}

Language::~Language()
{
    delete ui;
}
std::string Language::getCh()
{
    QString c=ui->comboBox->currentText();
    if(c==QString("Anglais"))
    {
        return "en";
    }
    if(c==QString(trUtf8("Néerlandais")))
    {
        return "nl";
    }
    if(c==QString("Allemand"))
    {
        return "de";
    }
    if(c==QString("Espagnol"))
    {
        return "es";
    }
    if(c==QString("Latin"))
    {
        return "la";
    }
    if(c==QString("Grec"))
    {
        return "el";
    }
    return "";
}
