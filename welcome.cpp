#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QString img, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap(img));
}

Welcome::~Welcome()
{
    delete ui;
}
