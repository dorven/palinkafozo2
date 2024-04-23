#include <QTime>
#include <QTimer>
#include <QSound>
#include <QIntValidator>

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
    set_input_validators();
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

void Palinkafozo::set_input_validators(){
    QValidator *input_validator = new QIntValidator(1, 9999, this);
    ui->mixTimeInput->setValidator(input_validator);
    ui->waitTimeInput->setValidator(input_validator);
}

void Palinkafozo::show_data(){
    rpm=get_rpm();
    ui->rpmDisplay->display(int(rpm));
    ui->timeDisplay->display(int(elapsedSeconds));
    is_safety_revolution_check_on = ui->safetyRevolutionCheck->isChecked();
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
        display_rpm_error_on_lcd();
        QSound::play("WarningSiren.wav");
    }
}

void Palinkafozo::display_rpm_error_on_lcd(){
    QPalette palette=ui->rpmDisplay->palette();
    palette.setColor(palette.WindowText, QColor(255, 0, 0));
    palette.setColor(palette.Light, QColor(255, 0, 0));
    palette.setColor(palette.Dark, QColor(255, 0, 0));
    ui->rpmDisplay->setPalette(palette);
}

void Palinkafozo::remove_rpm_error_from_lcd(){
    QPalette palette=ui->rpmDisplay->palette();
    palette.setColor(palette.WindowText, QColor(0, 0, 0));
    palette.setColor(palette.Light, QColor(255, 255, 255));
    palette.setColor(palette.Dark, QColor(159, 159, 159));
    ui->rpmDisplay->setPalette(palette);
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

void Palinkafozo::on_forwardButton_clicked()
{
    clear_pin(BACKWARD_PIN);
    set_pin(FORWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
    remove_rpm_error_from_lcd();
}

void Palinkafozo::on_stopButton_clicked()
{
    clear_pin(FORWARD_PIN);
    clear_pin(BACKWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
}

void Palinkafozo::on_backwardsButton_clicked()
{
    clear_pin(FORWARD_PIN);
    set_pin(BACKWARD_PIN);
    is_enabled=false;
    reset_stuck_variables();
    remove_rpm_error_from_lcd();
}

void Palinkafozo::on_startButton_clicked()
{
    mix_time=ui->mixTimeInput->text().toDouble();
    wait_time=ui->waitTimeInput->text().toDouble();
    is_enabled=true;
    elapsedSeconds=0;
    reset_stuck_variables();
    remove_rpm_error_from_lcd();
}

Palinkafozo::~Palinkafozo()
{
    delete ui;
}
