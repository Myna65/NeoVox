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
#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <QtGui/qlineedit.h>

namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestWindow(QWidget *parent = 0,QVector<int> t=QVector<int>());
    ~TestWindow();
    void motsuivant();
    bool ok;
private:
    Ui::TestWindow *ui;
    QList<QString> fr;
    QList<QString> nl;
    QList<QString> efr;
    QList<QString> enl;
    QList<int> ids;
    QList<int> eids;
    QString curfr,curnl;
    int curid,tid,tot,jus;
    bool sens,rap;
    QLineEdit* lineEdit, *lineEdit_2;
    bool suivant,fini;
public slots:
    void ClicBouton();
};

#endif // TESTWINDOW_H
