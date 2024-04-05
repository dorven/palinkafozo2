#include <QTime>
#include <QTimer>
#include <QSound>

#include "palinkafozo.h"
#include "ui_palinkafozo.h"
#include "parapin.h"

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    rpm=0;
    enabled=0;
    mix_time=0;
    wait_time=0;
    elapsedSeconds=0;
    left_stuck=0;
    right_stuck=0;
    both_stuck=0;

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(show_data()));
    timer->start(500);
    show_data();

}

void Palinkafozo::show_data(){
    rpm=get_rpm();
    ui->lcdNumber->display(int(rpm));
    ui->lcdNumber_2->display(int(elapsedSeconds));
    if(enabled==1){
        elapsedSeconds+=0.5;
        if(elapsedSeconds<=mix_time) {clear_pin(LP_PIN[2]); set_pin(LP_PIN[1]);}
        if(elapsedSeconds>mix_time) clear_pin(LP_PIN[1]);
        if(elapsedSeconds>=(mix_time+wait_time)) elapsedSeconds=0;
        if(elapsedSeconds<=mix_time && elapsedSeconds>=1 && rpm==0 && enabled==1){
            enabled=0; elapsedSeconds=0; left_stuck=1;
        }
    }
    if((left_stuck==1 || right_stuck==1)&& both_stuck==0) try_to_unstuck();
    if(both_stuck==1){
        QSound::play("WarningSiren.wav");

    }

}

void Palinkafozo::try_to_unstuck(){
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    if(elapsedSeconds>=1.5 && rpm==0){
        clear_pin(LP_PIN[2]);
        clear_pin(LP_PIN[1]);
        both_stuck=1;
    }
    if(elapsedSeconds>=1.5 && rpm>0){
        elapsedSeconds=0;
        clear_pin(LP_PIN[2]);
        enabled=1;
        left_stuck=0;
    }
    elapsedSeconds+=0.5;

}

double Palinkafozo::get_rpm(){
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

void Palinkafozo::on_pushButton_2_clicked()
{
    clear_pin(LP_PIN[2]);
    set_pin(LP_PIN[1]);
    enabled=0;
    both_stuck=0;
    left_stuck=0;
}

void Palinkafozo::on_pushButton_3_clicked()
{
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
    enabled=0;
    both_stuck=0;
    left_stuck=0;
}

void Palinkafozo::on_pushButton_4_clicked()
{
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    both_stuck=0;
    left_stuck=0;
    enabled=0;
}

void Palinkafozo::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    QString text2=ui->lineEdit_2->text();
    mix_time=text.toDouble();
    wait_time=text2.toDouble();

    enabled=1;
    both_stuck=0;
    left_stuck=0;
}

Palinkafozo::~Palinkafozo()
{
    delete ui;
}
