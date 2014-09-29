#ifndef PALINKAFOZO_H
#define PALINKAFOZO_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class Palinkafozo;
}

class Palinkafozo : public QMainWindow
{
    Q_OBJECT

public:
    double rpm;
    int enabled;
    double move_time;
    double sleep_time;
    double counter;
    int direction;
    int emergency;
    int left_stuck;
    int right_stuck;
    int both_stucked;
    double getrpm();
    void solveproblem();
    explicit Palinkafozo(QWidget *parent = 0);
    ~Palinkafozo();

public slots:
    void ShowData();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_clicked();

public:
    Ui::Palinkafozo *ui;
};


/*class Sleeper : public QThread
{
public:
    Sleeper(){}
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};*/


#endif // PALINKAFOZO_H
