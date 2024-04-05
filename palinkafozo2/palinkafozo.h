#ifndef PALINKAFOZO_H
#define PALINKAFOZO_H

#include <QMainWindow>

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

    double rpm;
    bool enabled;
    double mix_time;
    double wait_time;
    double elapsedSeconds;
    bool left_stuck;
    bool both_stuck;
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
