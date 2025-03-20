#include "PID.h"

PID::PID(double kp, double ki, double kd)
    : wzmocnienieProporcjonalne(kp), wzmocnienieCalkujace(ki), wzmocnienieRowniczkujace(kd),
    odchylenieLiniowe(0),  resetujOdchylenie(false),poprzedniUchyb(0) {
}
void PID::resetPamieci() {
    odchylenieLiniowe = 0.0;
    poprzedniUchyb = 0;
    Ti=0;
}

void PID::ustawWzmocnienieProporcjonalne(double wartosc) {
    wzmocnienieProporcjonalne = wartosc;
}

void PID::ustawWzmocnienieCalkujace(double wartosc) {
    wzmocnienieCalkujace = wartosc;
}

void PID::ustawWzmocnienieRowniczkujace(double wartosc) {
    wzmocnienieRowniczkujace = wartosc;
}
double PID::oblicz(double uchyb) {
    double proporcjonalna = wzmocnienieProporcjonalne * uchyb;
    Up=proporcjonalna;
    double calkujaca = 0;
    if (wzmocnienieCalkujace != 0) {
        Ti += uchyb;
        calkujaca = (1 / wzmocnienieCalkujace) * Ti;

    }
    Ui=calkujaca;

    double rozniczkujaca = wzmocnienieRowniczkujace * (uchyb - poprzedniUchyb);

    poprzedniUchyb = uchyb;
    Ud=rozniczkujaca;
    return proporcjonalna + calkujaca + rozniczkujaca;

}

double PID::sumator(double wartoscZadana, double wartoscMierzona) {
    return wartoscZadana - wartoscMierzona;
}

double PID::obliczSprzezenie(double wartoscZadana, double wartoscMierzona) {

    uchyb = sumator(wartoscZadana, wartoscMierzona);
   // qDebug()<<uchyb;
    double sygnalSterujacy = this->oblicz(uchyb);

    return sygnalSterujacy;
}
