#include "palinkafozo.h"
#include "ui_palinkafozo.h"

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    ui->setupUi(this);
}

Palinkafozo::~Palinkafozo()
{
    delete ui;
}
