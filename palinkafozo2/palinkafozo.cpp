#include <QTime>
#include <QTimer>
#include <QSound>

#include "parapin.h"

#include "palinkafozo.h"
#include "ui_palinkafozo.h"

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    rpm=0;
    enabled=false;
    mix_time=0;
    wait_time=0;
    elapsedSeconds=0;
    reset_stuck_variables();

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(show_data()));
    timer->start(REFRESH_TIME);
    show_data();
}

void Palinkafozo::init_port(){
    if (pin_init_user(LPT1) < 0) exit(0);
    pin_output_mode(LP_PIN[1]);
    pin_output_mode(LP_PIN[2]);
    pin_input_mode(LP_PIN[13]);
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
}

void Palinkafozo::reset_stuck_variables(){
    left_stuck=false;
    both_stuck=false;
}

void Palinkafozo::show_data(){
    rpm=get_rpm();
    ui->lcdNumber->display(int(rpm));
    ui->lcdNumber_2->display(int(elapsedSeconds));
    if(enabled){
        elapsedSeconds+=0.5;
        if(elapsedSeconds<=mix_time) {clear_pin(LP_PIN[2]); set_pin(LP_PIN[1]);}
        if(elapsedSeconds>mix_time) clear_pin(LP_PIN[1]);
        if(elapsedSeconds>=(mix_time+wait_time)) elapsedSeconds=0;
        if(elapsedSeconds<=mix_time && elapsedSeconds>=1 && rpm<MIN_RPM && enabled){
            enabled=false; elapsedSeconds=0; left_stuck=true;
        }
    }
    if(left_stuck && !both_stuck) try_to_unstuck();
    if(both_stuck){
        QSound::play("WarningSiren.wav");
    }
}

void Palinkafozo::try_to_unstuck(){
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    if(elapsedSeconds>=REVERSE_SPIN_TIME && rpm<MIN_RPM){
        clear_pin(LP_PIN[2]);
        clear_pin(LP_PIN[1]);
        both_stuck=true;
    }
    if(elapsedSeconds>=REVERSE_SPIN_TIME && rpm>=MIN_RPM){
        elapsedSeconds=0;
        clear_pin(LP_PIN[2]);
        enabled=true;
        left_stuck=false;
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
    enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_3_clicked()
{
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
    enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_4_clicked()
{
    clear_pin(LP_PIN[1]);
    set_pin(LP_PIN[2]);
    enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    QString text2=ui->lineEdit_2->text();
    mix_time=text.toDouble();
    wait_time=text2.toDouble();

    enabled=true;
    reset_stuck_variables();
}

Palinkafozo::~Palinkafozo()
{
    delete ui;
}
