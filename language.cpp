#include "language.h"
#include "ui_language.h"

Language::Language(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Language)
{
    ui->setupUi(this);
}

Language::~Language()
{
    delete ui;
}
std::string Language::getCh()
{
    QString c=ui->comboBox->currentText();
    if(c==QString("Anglais"))
    {
        return "en";
    }
    if(c==QString(trUtf8("Néerlandais")))
    {
        return "nl";
    }
    if(c==QString("Allemand"))
    {
        return "de";
    }
    if(c==QString("Espagnol"))
    {
        return "es";
    }
    if(c==QString("Latin"))
    {
        return "la";
    }
    if(c==QString("Grec"))
    {
        return "el";
    }
    return "";
}
