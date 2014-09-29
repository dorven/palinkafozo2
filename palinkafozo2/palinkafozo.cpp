#include "palinkafozo.h"
#include "ui_palinkafozo.h"
#include "parapin.h"
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QTime>

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    rpm=0;
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ShowData()));
    timer->start(600);
    ShowData();
}

void Palinkafozo::ShowData(){
    double rpm=getrpm();
    ui->lcdNumber->display(int(rpm));
}

double Palinkafozo::getrpm(){
    QTime myTimer;
    int prev=0;
    int rev=0;
    myTimer.start();

    if(pin_is_set(LP_PIN[13])<1){
        prev=0;
    }
    if(pin_is_set(LP_PIN[13])>=1){
        prev=1;
    }
    while(myTimer.elapsed()<500){
       if(pin_is_set(LP_PIN[13])<1 && prev==1){
           rev++;
           prev=0;
       }
       if(pin_is_set(LP_PIN[13])>=1 && prev==0){
           rev++;
           prev=1;
       }
    }
    return rev*2;
}




Palinkafozo::~Palinkafozo()
{
    delete ui;
}

void Palinkafozo::on_pushButton_2_clicked()
{
    clear_pin(LP_PIN[2]);
    set_pin(LP_PIN[1]);
}

void Palinkafozo::on_pushButton_3_clicked()
{
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
}

void Palinkafozo::on_pushButton_4_clicked()
{
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
}
