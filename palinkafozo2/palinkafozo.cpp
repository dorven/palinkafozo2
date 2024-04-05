#include "palinkafozo.h"
#include "ui_palinkafozo.h"
#include "parapin.h"
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QTime>
#include <iostream>
#include <QApplication>
#include <QSound>

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    rpm=0;
    enabled=0;
    move_time=0;
    sleep_time=0;
    counter=0;
    direction=0;
    emergency=0;
    left_stuck=0;
    right_stuck=0;
    both_stucked=0;

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ShowData()));
    timer->start(500);
    ShowData();

}

void Palinkafozo::ShowData(){
    rpm=getrpm();
    ui->lcdNumber->display(int(rpm));
    ui->lcdNumber_2->display(int(counter));
    if(enabled==1){
        counter+=0.5;
        if(counter<=move_time) {clear_pin(LP_PIN[2]); set_pin(LP_PIN[1]); }
        if(counter>move_time) clear_pin(LP_PIN[1]);
        if(counter>=(move_time+sleep_time)) counter=0;
        if(counter<=move_time && counter>=1 && rpm==0 && enabled==1){ enabled=0; counter=0; left_stuck=1;}
    }
    if((left_stuck==1 || right_stuck==1)&& both_stucked==0) solveproblem();
    if(both_stucked==1){
        QSound::play("WarningSiren.wav");

    }

}

void Palinkafozo::solveproblem(){
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    if(counter>=1.5 && rpm==0){ clear_pin(LP_PIN[2]);  clear_pin(LP_PIN[1]); both_stucked=1;}
    if(counter>=1.5 && rpm>0){
        counter=0;
        clear_pin(LP_PIN[2]);
        enabled=1;
        left_stuck=0;
    }
    counter+=0.5;

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
    while(myTimer.elapsed()<400){
       if(pin_is_set(LP_PIN[13])<1 && prev==1){
           rev++;
           prev=0;
       }
       if(pin_is_set(LP_PIN[13])>=1 && prev==0){
           rev++;
           prev=1;
       }
    }
    return rev*2.5*60;
}





Palinkafozo::~Palinkafozo()
{
    delete ui;
}

void Palinkafozo::on_pushButton_2_clicked()
{
    clear_pin(LP_PIN[2]);
    set_pin(LP_PIN[1]);
    enabled=0;
    both_stucked=0;
    left_stuck=0;
}

void Palinkafozo::on_pushButton_3_clicked()
{
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
    enabled=0;
    both_stucked=0;
    left_stuck=0;
}

void Palinkafozo::on_pushButton_4_clicked()
{
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    both_stucked=0;
    left_stuck=0;
    enabled=0;
}

void Palinkafozo::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    QString text2=ui->lineEdit_2->text();
    move_time=text.toDouble();
    sleep_time=text2.toDouble();

    enabled=1;
    both_stucked=0;
    left_stuck=0;
}
