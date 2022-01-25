#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose,false);
}

Help::~Help()
{
    delete ui;
}
