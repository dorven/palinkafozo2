#include "palinkafozo.h"
#include <QApplication>
#include "parapin.h"

int main(int argc, char *argv[])
{
    if (pin_init_user(LPT1) < 0) exit(0);
    pin_output_mode(LP_PIN[1]);
    pin_output_mode(LP_PIN[2]);
    pin_input_mode(LP_PIN[13]);
    clear_pin(LP_PIN[1]);
    clear_pin(LP_PIN[2]);
    QApplication a(argc, argv);
    Palinkafozo w;
    w.show();
    return a.exec();
}
