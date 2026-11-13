#ifndef SERVIDOR_SOCKETS_H
#define SERVIDOR_SOCKETS_H
#include <cstddef>
#include "ListaGen.h"
#include "Sensor.h"
#include "MuestraSensor.h"

class ServidorSMTI {
private:
    int server_fd;
    int puerto;
    ListaGen<Sensor*>& lista;

    bool recibirExacto(int fd, void* buf, std::size_t n);   // falso si el cliente cerro
    Sensor* crearSensor(const MuestraSensor& m);            // fabrica polimorfica
    // Devuelve true si el cliente pidio terminar el servidor
    bool atenderCliente(int client_fd);

public:
    ServidorSMTI(int p, ListaGen<Sensor*>& l);
    ~ServidorSMTI();

    void iniciar();          // socket
    void atenderClientes();  // en bucle hasta recibir fin
    void cerrar();
};
#endif
