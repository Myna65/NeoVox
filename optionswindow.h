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
#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>

namespace Ui {
class optionswindow;
}

class optionswindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit optionswindow(QWidget *parent = 0);
    ~optionswindow();
    void open();
    
private:
    Ui::optionswindow *ui;
};

#endif // OPTIONSWINDOW_H
