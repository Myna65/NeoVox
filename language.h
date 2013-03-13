#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QDialog>

namespace Ui {
class Language;
}

class Language : public QDialog
{
    Q_OBJECT
    
public:
    explicit Language(QWidget *parent = 0);
    ~Language();
    std::string getCh();
    
private:
    Ui::Language *ui;
};

#endif // LANGUAGE_H
