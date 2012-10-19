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
