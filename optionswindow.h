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
