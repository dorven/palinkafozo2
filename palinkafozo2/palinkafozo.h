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
    double rpm;
    int enabled;
    double mix_time;
    double wait_time;
    double elapsedSeconds;
    int left_stuck;
    int right_stuck;
    int both_stuck;
    void init_port();
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
