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
    explicit Palinkafozo(QWidget *parent = 0);
    ~Palinkafozo();

private:
    Ui::Palinkafozo *ui;
};

#endif // PALINKAFOZO_H
