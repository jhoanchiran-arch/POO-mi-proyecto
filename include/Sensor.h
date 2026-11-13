#ifndef SENSOR_H
#define SENSOR_H
#include <iostream>
#include <string>

class Sensor {
protected:
    int id;
    std::string ubicacion;
    double lectura;
public:
    Sensor(int _id, const std::string& _ub, double _lect);
    virtual ~Sensor();                       // virtual: evita fugas al borrar via Sensor

    int getId() const { return id; }
    double getLectura() const { return lectura; }

    virtual void procesarLectura() = 0;      // clase abstracta

    friend std::ostream& operator<<(std::ostream& os, const Sensor& s);
};
#endif
