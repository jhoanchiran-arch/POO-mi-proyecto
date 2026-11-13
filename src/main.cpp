#include <iostream>
#include <stdexcept>
#include "Sensor.h"
#include "ListaGen.h"
#include "ServidorSockets.h"

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << " SMTI: SERVIDOR DE TELEMETRIA DISTRIBUIDA (UNIDAD 9)" << std::endl;
    std::cout << "==================================================" << std::endl;

    ListaGen<Sensor*> listaPlanta;

    try {
        ServidorSMTI servidor(8080, listaPlanta);
        servidor.iniciar();
        servidor.atenderClientes();

        std::cout << "\n--- PROCESAMIENTO POLIMORFICO DE TELEMETRIA ("
                  << listaPlanta.tamano() << " muestras) ---" << std::endl;
        listaPlanta.procesarTodo();

        std::cout << "\n--- BUSQUEDA POR ID (101) ---" << std::endl;
        Sensor* s = listaPlanta.buscarPorId(101);
        if (s) std::cout << *s << std::endl;
        else   std::cout << "Sensor 101 no encontrado." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "EXCEPCION GRAVE SMTI " << e.what() << std::endl;
        return 1;
    }
    std::cout << "\n--- Cierre: ~ListaGen() libera el Heap ---" << std::endl;
    return 0; 
}
