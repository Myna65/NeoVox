#include "choixtestwindow.h"
#include "ui_choixtestwindow.h"
#include "mainwindow.h"
#include "testwindow.h"
#include <QtSql/QSqlQuery>
#include <QtGui/qmessagebox.h>
#include <QtGui/qprinter.h>
#include <QtGui/qprintdialog.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtexttable.h>
#include "introwindow.h"

ChoixTestWindow::ChoixTestWindow(QWidget *parent, int modi) :
    QWidget(parent),
    ui(new Ui::ChoixTestWindow)
{
    modif=modi;
    ui->setupUi(this);
    ui->listWidget->setDragEnabled(false);
    QSqlQuery q("SELECT num,titre,id FROM Serie ORDER BY num DESC");
    q.exec();
    while(q.next())
    {
        ui->listWidget->insertItem(0,q.value(0).toString()+" - "+q.value(1).toString());
        ids.push_front(q.value(2).toInt());
    }
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(ouvrirTest()));
    if(modif==1)
    {
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->label->setText(trUtf8("Choisissez la leçon à modifier"));
        ui->listWidget->setToolTip("");
        ui->pushButton->setText(trUtf8("Modifier la leçon sélectionnée"));
    }
    if(modif==2)
    {
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->label->setText(trUtf8("Renumérotez les leçons"));
        ui->listWidget->setToolTip("");
        ui->pushButton->setText(trUtf8("Valider la renumérotation"));
        ui->listWidget->setDragEnabled(true);
    }
    if(modif==3)
    {
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->label->setText(trUtf8("Sélectionnez la leçon à supprimer"));
        ui->listWidget->setToolTip("");
        ui->pushButton->setText(trUtf8("Supprimer la leçon sélectionnée"));
    }
    if(modif==4)
    {
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->label->setText(trUtf8("Sélectionnez la leçon à imprimer"));
        ui->listWidget->setToolTip("");
        ui->pushButton->setText(trUtf8("Imprimer la leçon sélectionnée"));
    }
}

ChoixTestWindow::~ChoixTestWindow()
{
    delete ui;
}
void ChoixTestWindow::ouvrirTest()
{
    if(modif==0)
    {
        QVector<int> sel;
        QModelIndexList list = ui->listWidget->selectionModel()->selectedIndexes();
        if(list.size()==0)
        {
            QMessageBox::critical(MainWindow::Instance(),trUtf8("Impossible de démarrer le test"),trUtf8("Aucun élément sélectionné"));
            return;
        }
        for(int i=0;i<list.size();i++)
        {
            sel.push_back(ids[list[i].row()]);
        }
        TestWindow* tw = new TestWindow(MainWindow::Instance(),sel);
        if(tw->ok)
            MainWindow::Instance()->setCentralWidget(tw);
        else
        {
            QMessageBox::information(MainWindow::Instance(),trUtf8("Impossible de démarrer le test"),trUtf8("Tous les éléments de la leçon ont été validés justes lors des cinq dernières demandes"));
            MainWindow::Instance()->setDefault();
        }
    }
    else if(modif==1)
    {
        MainWindow::Instance()->setCentralWidget(new Introwindow(MainWindow::Instance(),true,ids[ui->listWidget->currentIndex().row()]));
    }
    else if(modif==2)
    {
        QSqlQuery q;
        q.prepare("UPDATE Serie SET num=:n WHERE id=:i");
        for(int i=0;i<ui->listWidget->count();i++)
        {
            q.bindValue(":n",i+1);
            q.bindValue(":i",ids[ui->listWidget->model()->index(i,0).data().toString().section("-",0,0).toInt()-1]);
            q.exec();
        }
        MainWindow::Instance()->setDefault();
    }
    else if(modif==3)
    {
        if(QMessageBox::warning(MainWindow::Instance(),"Confirmation",trUtf8("Êtes-vous sûr de vouloir supprimer la leçon sélectionnée"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        {
            int sel = ui->listWidget->currentIndex().data().toString().section('-',0,0).toInt();
            QSqlQuery q;
            q.prepare("SELECT id FROM Serie WHERE num=:i");
            q.bindValue(":i",sel);
            q.exec();
            q.next();
            int id=q.value(0).toInt();
            q.prepare("DELETE FROM Serie WHERE id=:i");
            q.bindValue(":i",id);
            q.exec();
            q.prepare("DELETE FROM Result WHERE mot_id IN (SELECT id FROM Mot WHERE serie_id=:i)");
            q.bindValue(":i",id);
            q.exec();
            q.prepare("DELETE FROM Mot WHERE serie_id=:i");
            q.bindValue(":i",id);
            q.exec();
            q.prepare("DELETE FROM Jeu WHERE serie_id=:i");
            q.bindValue(":i",id);
            q.exec();
            q.prepare("UPDATE Serie SET num=num-1 WHERE num>:i");
            q.bindValue(":i",sel);
            q.exec();
            MainWindow::Instance()->setDefault();
        }
    }
    else if(modif==4)
    {
        int sel = ui->listWidget->currentIndex().data().toString().section('-',0,0).toInt();
        QSqlQuery q;
        q.prepare("SELECT id FROM Serie WHERE num=:n");
        q.bindValue(":n",sel);
        q.exec();
        q.next();
        int id=q.value(0).toInt();
        QPrinter printer;
        QPrintDialog dia(&printer,MainWindow::Instance());
        if(dia.exec()!=QPrintDialog::Accepted)
        {
            return;
        }

        QTextCharFormat h1;
        h1.setFont(QFont("Arial",20));
        h1.setUnderlineStyle(QTextCharFormat::SingleUnderline);

        int sc[]={50,50};
        QVector<QTextLength> col;
        for(int i=0;i<2;i++)
        {
            col.push_back(QTextLength(QTextLength::PercentageLength,sc[i]));
        }
        QTextTableFormat tableformat;
        tableformat.setCellPadding(5);
        tableformat.setCellSpacing(0);
        tableformat.setHeaderRowCount(1);
        tableformat.setColumns(2);
        tableformat.setColumnWidthConstraints(col);

        QTextTableCellFormat cellformatheader,cellformat;
        QFont f("Arial",10),f2("Arial",10);
        cellformat.setFont(f2);
        f.setBold(true);
        cellformatheader.setFont(f);

        int count=0;
        QTextEdit doc;
        doc.setCurrentCharFormat(h1);
        doc.setAlignment(Qt::AlignCenter);
        q.prepare("SELECT Titre FROM Serie WHERE id=:i");
        q.bindValue(":i",id);
        q.exec();
        q.next();
        doc.append("Vocabulaire : "+q.value(0).toString());
        QTextTable* table = doc.textCursor().insertTable(1,2);
        table->setFormat(tableformat);
        QString titre[]={trUtf8("Français"),trUtf8("Néerlandais")};
        for(int i=0;i<2;i++)
        {
            table->cellAt(0,i).setFormat(cellformatheader);
            table->cellAt(0,i).firstCursorPosition().insertText(titre[i]);
        }
        q.prepare("SELECT fr,nl FROM Mot WHERE serie_id=:s");
        q.bindValue(":s",id);
        q.exec();
        while(q.next())
        {
            table->appendRows(1);
            count++;
            table->cellAt(count,0).setFormat(cellformat);
            table->cellAt(count,0).firstCursorPosition().insertText(q.value(0).toString());
            table->cellAt(count,1).setFormat(cellformat);
            table->cellAt(count,1).firstCursorPosition().insertText(q.value(1).toString());
        }
        doc.print(&printer);


        MainWindow::Instance()->setDefault();
    }
}