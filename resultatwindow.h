#ifndef RESULTATWINDOW_H
#define RESULTATWINDOW_H

#include <QWidget>

namespace Ui {
class ResultatWindow;
}

class ResultatWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultatWindow(QWidget *parent = 0);
    ~ResultatWindow();
    
private:
    Ui::ResultatWindow *ui;
};

#endif // RESULTATWINDOW_H
