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
