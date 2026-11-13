#ifndef MUESTRA_SENSOR_H
#define MUESTRA_SENSOR_H
// Trama industrial binaria que viaja por TCP (cliente y servidor la comparten).
struct MuestraSensor {
    int    idSensor;
    double valor;
    char   tipo[12];   // "TEMPERATURA", "PRESION", "CAUDAL" o "FIN"
};
#endif
