#include "ServidorSockets.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "SensorCaudal.h"
#include "ExcepcionesSMTI.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

ServidorSMTI::ServidorSMTI(int p, ListaGen<Sensor*>& l)
    : server_fd(-1), puerto(p), lista(l) {}

ServidorSMTI::~ServidorSMTI() { cerrar(); }

void ServidorSMTI::cerrar() {
    if (server_fd >= 0) { close(server_fd); server_fd = -1; }
}

void ServidorSMTI::iniciar() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw ErrorRedSMTI("No se pudo instanciar el socket.");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerrar();
        throw ErrorRedSMTI("Enlace fallido en puerto " + std::to_string(puerto) +
                           " (puerto ocupado o sin permisos).");
    }
    if (listen(server_fd, 5) < 0) {
        cerrar();
        throw ErrorRedSMTI("Fallo la operacion listen().");
    }
    std::cout << "SERVIDOR SMTI Escuchando telemetria en puerto " << puerto << "..." << std::endl;
}


bool ServidorSMTI::recibirExacto(int fd, void* buf, std::size_t n) {
    char* p = static_cast<char*>(buf);
    std::size_t total = 0;
    while (total < n) {
        ssize_t r = recv(fd, p + total, n - total, 0);
        if (r == 0) {                       
            if (total == 0) return false;   
            throw ErrorTramaSMTI("Trama incompleta: el cliente cerro a mitad de envio.");
        }
        if (r < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                throw ErrorTramaSMTI("se acabo el tiempo esperando datos del cliente.");
            throw ErrorRedSMTI("Fallo recv(): " + std::string(std::strerror(errno)));
        }
        total += static_cast<std::size_t>(r);
    }
    return true;
}

Sensor* ServidorSMTI::crearSensor(const MuestraSensor& m) {
    if (m.idSensor <= 0)
        throw ErrorTramaSMTI("ID de sensor invalido: " + std::to_string(m.idSensor));
    if (!std::isfinite(m.valor))
        throw ErrorTramaSMTI("Valor no numerico en sensor " + std::to_string(m.idSensor));

    std::string tipo(m.tipo, strnlen(m.tipo, sizeof(m.tipo)));   
    std::string ub = "Nodo-" + std::to_string(m.idSensor);

    if (tipo == "TEMPERATURA") return new SensorTemperatura(m.idSensor, ub, m.valor);
    if (tipo == "PRESION")     return new SensorPresion(m.idSensor, ub, m.valor);
    if (tipo == "CAUDAL")      return new SensorCaudal(m.idSensor, ub, m.valor);
    throw ErrorTramaSMTI("Tipo de sensor desconocido: '" + tipo + "'");
}

bool ServidorSMTI::atenderCliente(int client_fd) {
    
    timeval tv; tv.tv_sec = 5; tv.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (true) {
        MuestraSensor m;
        std::memset(&m, 0, sizeof(m));
        try {
            if (!recibirExacto(client_fd, &m, sizeof(m))) return false;  

            if (std::strncmp(m.tipo, "FIN", sizeof(m.tipo)) == 0) {
                std::cout << "SERVIDOR SMTI Orden FIN recibida." << std::endl;
                return true;
            }
            Sensor* s = crearSensor(m);   
            lista.insertar(s);            
            std::cout << "[SERVIDOR SMTI] Muestra recibida -> " << *s << std::endl;
        } catch (const ErrorTramaSMTI& e) {
            std::cerr << "[AVISO] Trama descartada: " << e.what() << std::endl;
            if (std::string(e.what()).find("incompleta") != std::string::npos ||
                std::string(e.what()).find("se acabo el tiempo") != std::string::npos)
                return false;            
        }
    }
}

void ServidorSMTI::atenderClientes() {
    bool terminar = false;
    while (!terminar) {
        sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int client_fd = accept(server_fd, (struct sockaddr*)&cli, &len);
        if (client_fd < 0) {
            if (errno == EINTR) continue;
            throw ErrorRedSMTI("No se pudo aceptar la conexion del cliente.");
        }
        std::cout << "SERVIDOR SMTI Cliente conectado." << std::endl;
        try {
            terminar = atenderCliente(client_fd);
        } catch (...) {
            close(client_fd);  
            throw;
        }
        close(client_fd);
        std::cout << "SERVIDOR SMTI Cliente desconectado." << std::endl;
    }
}
