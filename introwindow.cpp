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
#include "introwindow.h"
#include "ui_introwindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlError>
#include <QtGui/QKeyEvent>

Introwindow::Introwindow(QWidget *parent,bool modif, int ser) :
    QWidget(parent),
    ui(new Ui::introwindow),
    model(this),
    delegate(this)
{
    modi=modif;
    ui->setupUi(this);
    ui->numRoDeLaSRieLabel->setVisible(false);
    ui->numRoDeLaSRieSpinBox->setVisible(false);
    ui->tableView->setModel(&model);
    ui->tableView->setItemDelegate(&delegate);
    ui->tableView->hideColumn(2);
    QSqlQuery req;
    req.exec("SELECT MAX(num)+1 FROM Serie");
    if(req.next())
    {
        ui->numRoDeLaSRieSpinBox->setValue(req.value(0).toInt());
        if(ui->numRoDeLaSRieSpinBox->value()==0)
            ui->numRoDeLaSRieSpinBox->setValue(1);
    }
    else
    {
        ui->numRoDeLaSRieSpinBox->setValue(1);
    }
    connect(ui->numRoDeLaSRieSpinBox,SIGNAL(editingFinished()),this,SLOT(valNum()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(clickMot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(clickSerie()));
    connect(ui->motFranAisLineEdit,SIGNAL(returnPressed()),ui->motNErlandaisLineEdit,SLOT(setFocus()));
    connect(ui->motNErlandaisLineEdit,SIGNAL(returnPressed()),this,SLOT(clickMot()));
    if(modif)
    {
        ui->pushButton_2->setText("Enregister les modifications");
        id=ser;
        QSqlQuery q;
        q.prepare("SELECT Mot.id,nl,fr,titre FROM Mot INNER JOIN Serie ON serie_id=Serie.id WHERE Serie.id=:i ORDER BY Mot.id DESC");
        q.bindValue(":i",id);
        q.exec();
        q.next();
        ui->titreDeLaSRieLineEdit->setText(q.value(3).toString());
        do
        {
            model.insertRow();
            model.setData(model.index(0,0),q.value(2));
            model.setData(model.index(0,1),q.value(1));
            model.setData(model.index(0,2),q.value(0));
        }while(q.next());

    }
}

Introwindow::~Introwindow()
{
    delete ui;
}

void Introwindow::valNum()
{
    QSqlQuery req;
    req.prepare("SELECT titre FROM Serie WHERE num=:n");
    req.bindValue(":n",ui->numRoDeLaSRieSpinBox->value());
    req.exec();
    if(req.next())
    {
        QMessageBox::warning(this,"Attention",QString("Attention, le numéro de cette leçon est déja attrivué à la leçon %1<br/>Un numéro non encore attribué remplace celui que vous venez de proposer.").arg(req.value(0).toString()));
        req.exec("SELECT MAX(num)+1 FROM Serie");
        if(req.next())
        {
            ui->numRoDeLaSRieSpinBox->setValue(req.value(0).toInt());
        }
    }
}
void Introwindow::clickMot()
{
    QString fr=ui->motFranAisLineEdit->text(),nl=ui->motNErlandaisLineEdit->text();
    if(fr==""||nl=="")
    {
        QMessageBox::critical(this,"Champs non remplis",trUtf8("Veulliez remplir les champs néerlandais et français"));
        return;
    }
    model.insertRow(1);
    model.setData(model.index(0,0),fr);
    model.setData(model.index(0,1),nl);
    ui->motFranAisLineEdit->setText("");
    ui->motNErlandaisLineEdit->setText("");
    ui->motFranAisLineEdit->setFocus();
}
void Introwindow::clickSerie()
{
    if(!modi)
    {
        QString titre=ui->titreDeLaSRieLineEdit->text(),num=ui->numRoDeLaSRieSpinBox->text();
        if(titre==""||num=="")
        {
            QMessageBox::critical(this,"Champs non remplis","Veulliez remplir le champ titre");
            return;
        }
        QSqlQuery req;
        req.prepare("INSERT INTO Serie (num,titre) VALUES (:n,:t)");
        req.bindValue(":t",titre);
        req.bindValue(":n",num);
        req.exec();
        req.prepare("SELECT id FROM Serie WHERE num=:n");
        req.bindValue(":n",num);
        req.exec();
        if(!req.next())
        {
            QMessageBox::critical(this,trUtf8("Impossible d'enregister la leçon"),trUtf8("Vérifiez l'unicité du numéro de la leçon"));
            return;
        }
        int id=req.value(0).toInt();
        for(int i=0;i<model.rowCount();i++)
        {
            QSqlQuery req2;
            req2.prepare("INSERT INTO Mot (fr,nl,serie_id,updated) VALUES (:f,:n,:s,0)");
            req2.bindValue(":f",model.index(i,0).data().toString());
            req2.bindValue(":n",model.index(i,1).data().toString());
            req2.bindValue(":s",id);
            req2.exec();
        }
    }
    else
    {
        QSqlQuery q;
        q.prepare("UPDATE Serie SET titre=:t WHERE id=:i");
        q.bindValue(":t",ui->titreDeLaSRieLineEdit->text());
        q.bindValue(":s",id);
        q.exec();
        q.prepare("UPDATE Mot SET updated=1 WHERE serie_id = :s");
        q.bindValue(":s",id);
        q.exec();
        for(int i=0;i<model.rowCount();i++)
        {
            if(model.index(i,2).data().toInt()==0)
            {
                QSqlQuery req2;
                req2.prepare("INSERT INTO Mot (fr,nl,serie_id,updated) VALUES (:f,:n,:s,0)");
                req2.bindValue(":f",model.index(i,0).data().toString());
                req2.bindValue(":n",model.index(i,1).data().toString());
                req2.bindValue(":s",id);
                req2.exec();
            }
            else
            {
                QSqlQuery req2;
                req2.prepare("UPDATE Mot SET fr = :f , nl = :nl ,updated=0 WHERE id=:i");
                req2.bindValue(":f",model.index(i,0).data().toString());
                req2.bindValue(":n",model.index(i,1).data().toString());
                req2.bindValue(":i",model.index(i,2).data().toInt());
                req2.exec();
            }
        }
        q.exec("DELETE FROM Mot WHERE updated=1");

    }
    MainWindow::Instance()->setDefault();
}



IntroTableDelegate::IntroTableDelegate(QObject* parent):QStyledItemDelegate(parent)
{

}
QWidget *IntroTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    switch(index.column())
    {
    case 0:
    case 1:
    case 2:
    {
        QLineEdit *editor=new QLineEdit(parent);
        return editor;
    }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}
void IntroTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch(index.column())
    {
    case 0:
    case 1:
    case 2:
    {
        QLineEdit *slider = qobject_cast<QLineEdit *>(editor);
        slider->setText(index.model()->data(index).toString());
        break;
    }
    default:
        QStyledItemDelegate::setEditorData(editor, index);
        break;
    }
}
void IntroTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch(index.column())
    {
    case 0:
    case 1:
    case 2:
    {
        QLineEdit *slider = qobject_cast<QLineEdit *>(editor);
        model->setData(index,slider->text());
        if((model->data(model->index(index.row(),0)).toString()=="")&&(model->data(model->index(index.row(),1)).toString()==""))
        {
            model->removeRow(index.row());
        }

        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor,model, index);
        break;
    }
}
void IntroTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

IntroTableModel::IntroTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}
bool IntroTableModel::insertRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(row);
    beginInsertRows(QModelIndex(),0,0);
    mElements.push_front(QVector<QVariant>(3,""));
    mElements[0][2]=0;
    endInsertRows();
    return true;
}
bool IntroTableModel::removeRows(int row,int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(count);
    beginRemoveRows(QModelIndex(),row,row);
    mElements.removeAt(row);
    endRemoveRows();
    return true;
}

Qt::ItemFlags IntroTableModel::flags(const QModelIndex &index) const
{
    if (index.column() < Count)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return QAbstractTableModel::flags(index);
}
int IntroTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Count;
}
int IntroTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mElements.count();
}
QVariant IntroTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mElements.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (index.column() == Fr)
        {
            return mElements[index.row()][0];
        }
        else if (index.column() == Nl)
        {
            return mElements[index.row()][1];
        }
        else if (index.column() == id)
        {
            return mElements[index.row()][2];
        }
        break;
    }

    return QVariant();
}
bool IntroTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mElements.count())
    {
        return false;
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (index.column() == Fr)
        {
            mElements[index.row()][0] = value.toString();
        }
        else if (index.column() == Nl)
        {
            mElements[index.row()][1] = value.toString();
        }
        else if (index.column() == id)
        {
            mElements[index.row()][2] = value.toString();
        }
        emit dataChanged(index, index);
        return true;
        break;
    }

    return false;
}
QVariant IntroTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Fr:
            return trUtf8("Français");
            break;
        case Nl:
            return trUtf8("Néerlandais");
            break;
        case id:
            return trUtf8("id");
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
void Introwindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Delete)
    {
        model.removeRow(ui->tableView->currentIndex().row());
    }
}
