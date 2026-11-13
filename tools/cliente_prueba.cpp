#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "MuestraSensor.h"

static MuestraSensor crear(int id, double v, const char* t) {
    MuestraSensor m; std::memset(&m, 0, sizeof(m));
    m.idSensor = id; m.valor = v; std::strncpy(m.tipo, t, sizeof(m.tipo) - 1);
    return m;
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a; std::memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET; a.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (connect(fd, (struct sockaddr*)&a, sizeof(a)) < 0) { std::cerr << "No conecta\n"; return 1; }

    MuestraSensor datos[] = {
        crear(101, 92.5, "TEMPERATURA"),
        crear(102, 65.0, "TEMPERATURA"),
        crear(201, 7.2,  "PRESION"),
        crear(301, 2.1,  "CAUDAL"),
        crear(999, 1.0,  "HUMEDAD"),  
        crear(0,   0.0,  "FIN")
    };
    for (auto& m : datos) send(fd, &m, sizeof(m), 0);
    close(fd);
    return 0;
}
