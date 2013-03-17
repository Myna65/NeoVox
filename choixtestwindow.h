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
#ifndef CHOIXTESTWINDOW_H
#define CHOIXTESTWINDOW_H

#include <QWidget>
#include <QtCore/QVector>

namespace Ui {
class ChoixTestWindow;
}

class ChoixTestWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChoixTestWindow(QWidget *parent = 0,int modi=0);
    ~ChoixTestWindow();
    
private:
    Ui::ChoixTestWindow *ui;
    QVector<int> ids;
    int modif;
public slots:
    void ouvrirTest();
};

#endif // CHOIXTESTWINDOW_H
