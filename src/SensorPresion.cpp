#include "SensorPresion.h"

SensorPresion::SensorPresion(int _id, const std::string& _ub, double _lect, double _umbral)
    : Sensor(_id, _ub, _lect), umbralMaximo(_umbral) {}

void SensorPresion::procesarLectura() {
    std::cout << "[PRESION POLIMORFICO] " << *this;
    if (lectura > umbralMaximo)
        std::cout << " --> ALERTA: SOBREPRESION!";
    std::cout << std::endl;
}
