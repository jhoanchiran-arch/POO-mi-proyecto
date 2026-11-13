#ifndef EXCEPCIONES_SMTI_H
#define EXCEPCIONES_SMTI_H
#include <stdexcept>
#include <string>

// Falla de red
class ErrorRedSMTI : public std::runtime_error {
public:
    explicit ErrorRedSMTI(const std::string& msg)
        : std::runtime_error("[RED] " + msg) {}
};

// Trama mal formada: solo descarta esa muestra
class ErrorTramaSMTI : public std::runtime_error {
public:
    explicit ErrorTramaSMTI(const std::string& msg)
        : std::runtime_error("[TRAMA] " + msg) {}
};
#endif
