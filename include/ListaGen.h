#ifndef LISTAGEN_H
#define LISTAGEN_H
#include <iostream>
#include <stdexcept>

template <typename T>
struct NodoGen {
    T dato;
    NodoGen<T>* siguiente;
    NodoGen(T _d) : dato(_d), siguiente(nullptr) {}
};

// Lista enlazada generica. Pensada para T = puntero a objeto en el Heap
// el destructor hace delete del objeto apuntado y del nodo.
template <typename T>
class ListaGen {
private:
    NodoGen<T>* head;
    int cantidad;

    // Evita copias: dos listas con los mismos punteros que harian doble delete
    ListaGen(const ListaGen&) = delete;
    ListaGen& operator=(const ListaGen&) = delete;

public:
    ListaGen() : head(nullptr), cantidad(0) {}

    ~ListaGen() {
        NodoGen<T>* actual = head;
        while (actual != nullptr) {
            NodoGen<T>* aux = actual->siguiente;
            delete actual->dato;   // libera el objeto polimorfico
            delete actual;         // libera el nodo
            actual = aux;
        }
        head = nullptr;
    }

    // Insercion al inicio
    void insertar(T nuevoDato) {
        NodoGen<T>* nuevo = new NodoGen<T>(nuevoDato);
        nuevo->siguiente = head;
        head = nuevo;
        ++cantidad;
    }

    // Recorrido polimorfico
    void procesarTodo() {
        for (NodoGen<T>* a = head; a != nullptr; a = a->siguiente)
            a->dato->procesarLectura();
    }

    // Busqueda por ID
    T buscarPorId(int id) const {
        for (NodoGen<T>* a = head; a != nullptr; a = a->siguiente)
            if (a->dato->getId() == id) return a->dato;
        return nullptr;
    }

    int tamano() const { return cantidad; }
};
#endif
