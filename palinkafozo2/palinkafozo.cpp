#include <QTime>
#include <QTimer>
#include <QSound>

#include "palinkafozo.h"
#include "ui_palinkafozo.h"

Palinkafozo::Palinkafozo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Palinkafozo)
{
    rpm=0;
    is_enabled=false;
    mix_time=0;
    wait_time=0;
    elapsedSeconds=0;
    is_safety_revolution_check_on=true;
    reset_stuck_variables();

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(show_data()));
    timer->start(REFRESH_TIME);
    show_data();
}

void Palinkafozo::init_port(){
    if (pin_init_user(LPT1) < 0) exit(0);
    pin_output_mode(FORWARD_PIN);
    pin_output_mode(BACKWARD_PIN);
    pin_input_mode(REV_COUNTER_SENSOR);
    clear_pin(FORWARD_PIN);
    clear_pin(BACKWARD_PIN);
}

void Palinkafozo::reset_stuck_variables(){
    is_forward_stuck=false;
    is_both_direction_stuck=false;
}

void Palinkafozo::show_data(){
    rpm=get_rpm();
    ui->lcdNumber->display(int(rpm));
    ui->lcdNumber_2->display(int(elapsedSeconds));
    if(is_enabled){
        elapsedSeconds+=0.5;
        if(elapsedSeconds<=mix_time) {clear_pin(BACKWARD_PIN); set_pin(FORWARD_PIN);}
        if(elapsedSeconds>mix_time) clear_pin(FORWARD_PIN);
        if(elapsedSeconds>=(mix_time+wait_time)) elapsedSeconds=0;
        if(elapsedSeconds<=mix_time && elapsedSeconds>=1 && rpm<MIN_RPM && is_enabled && is_safety_revolution_check_on){
            is_enabled=false; elapsedSeconds=0; is_forward_stuck=true;
        }
    }
    if(is_forward_stuck && !is_both_direction_stuck) try_to_unstuck();
    if(is_both_direction_stuck){
        QSound::play("WarningSiren.wav");
    }
}

void Palinkafozo::try_to_unstuck(){
    clear_pin(FORWARD_PIN);
    set_pin(BACKWARD_PIN);
    if(elapsedSeconds>=REVERSE_SPIN_TIME && rpm<MIN_RPM){
        clear_pin(BACKWARD_PIN);
        is_both_direction_stuck=true;
    }
    if(elapsedSeconds>=REVERSE_SPIN_TIME && rpm>=MIN_RPM){
        elapsedSeconds=0;
        clear_pin(BACKWARD_PIN);
        is_enabled=true;
        is_forward_stuck=false;
    }
    elapsedSeconds+=0.5;
}

double Palinkafozo::get_rpm(){
    QTime myTimer;
    int prev=0;
    int rev=0;
    myTimer.start();

    if(pin_is_set(REV_COUNTER_SENSOR)<1){
        prev=0;
    }
    if(pin_is_set(REV_COUNTER_SENSOR)>=1){
        prev=1;
    }
    while(myTimer.elapsed()<400){
       if(pin_is_set(REV_COUNTER_SENSOR)<1 && prev==1){
           rev++;
           prev=0;
       }
       if(pin_is_set(REV_COUNTER_SENSOR)>=1 && prev==0){
           rev++;
           prev=1;
       }
    }
    return rev*2.5*60;
}

void Palinkafozo::on_pushButton_2_clicked()
{
    clear_pin(BACKWARD_PIN);
    set_pin(FORWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_3_clicked()
{
    clear_pin(FORWARD_PIN);
    clear_pin(BACKWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_4_clicked()
{
    clear_pin(FORWARD_PIN);
    set_pin(BACKWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    QString text2=ui->lineEdit_2->text();
    mix_time=text.toDouble();
    wait_time=text2.toDouble();
    is_safety_revolution_check_on = ui->safetyRevolutionCheck->isChecked();

    is_enabled=true;
    reset_stuck_variables();
    elapsedSeconds=0;
}

Palinkafozo::~Palinkafozo()
{
    delete ui;
}
