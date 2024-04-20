#ifndef PALINKAFOZO_H
#define PALINKAFOZO_H

#include <QMainWindow>
#include "parapin.h"

namespace Ui {
class Palinkafozo;
}

class Palinkafozo : public QMainWindow
{
    Q_OBJECT

public:
    const int REFRESH_TIME = 500;
    const double REVERSE_SPIN_TIME = 1.5;
    const double MIN_RPM = 200;
    const int FORWARD_PIN = LP_PIN[1];
    const int BACKWARD_PIN = LP_PIN[2];
    const int REV_COUNTER_SENSOR = LP_PIN[13];

    double rpm;
    bool is_enabled;
    double mix_time;
    double wait_time;
    double elapsedSeconds;
    bool is_forward_stuck;
    bool is_both_direction_stuck;
    bool is_safety_revolution_check_on;
    void init_port();
    void reset_stuck_variables();
    double get_rpm();
    void try_to_unstuck();
    explicit Palinkafozo(QWidget *parent = 0);
    ~Palinkafozo();

public slots:
    void show_data();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_clicked();

public:
    Ui::Palinkafozo *ui;
};

#endif // PALINKAFOZO_H
