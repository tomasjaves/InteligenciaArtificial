#include "../include/nodo.h"

/**
 * @brief Constructor por defecto de la clase Nodo.
 * 
 */
Nodo::Nodo() : id_(-1), pos_padre_(-1), coste_(0), padre_(nullptr) {}

/**
 * @brief Constructor de la clase Nodo.
 * 
 * @param id ID del nodo.
 */
Nodo::Nodo(const int& id) : id_(id), pos_padre_(-1), coste_(0), padre_(nullptr) {}

/**
 * @brief Constructor de la clase Nodo.
 * 
 * @param id ID del nodo.
 * @param pos_padre Posición del padre en la estructura de nodos.
 * @param coste Coste de la arista hacia este nodo.
 * @param padre ID del nodo padre.
 * @param profundidad Profundidad del nodo en el árbol.
 */
Nodo::Nodo(const int& id, const int& pos_padre, const int& coste, Nodo* padre) 
  : id_(id), pos_padre_(pos_padre), coste_(coste), padre_(padre) {}

/**
 * @brief Método getter para obtener el ID del nodo.
 * 
 * @return int 
 */
int Nodo::GetID() const { return id_; }

/**
 * @brief Método getter para obtener el coste de la arista hacia este nodo.
 * 
 * @return int 
 */
int Nodo::GetCoste() const { return coste_; }

/**
 * @brief Método getter para obtener el ID del nodo padre.
 * 
 * @return int 
 */
Nodo* Nodo::GetPadre() const { return padre_; }

/**
 * @brief Método getter para obtener la posición del padre en la estructura de nodos.
 * 
 * @return int 
 */
int Nodo::GetPosPadre() const { return pos_padre_; }

/**
 * @brief Método setter para establecer el ID del nodo.
 * 
 * @param id ID del nodo.
 */
void Nodo::SetID(int id) { id_ = id; }

/**
 * @brief Método setter para establecer el coste de la arista hacia este nodo.
 * 
 * @param coste Coste de la arista hacia este nodo.
 */
void Nodo::SetCoste(int coste) { coste_ = coste; }

/**
 * @brief Método setter para establecer el ID del nodo padre.
 * 
 * @param padre ID del nodo padre.
 */
void Nodo::SetPadre(Nodo* padre) { padre_ = padre; }

/**
 * @brief Método setter para establecer la posición del padre en la estructura de nodos.
 * 
 * @param pos_padre Posición del padre en la estructura de nodos.
 */
void Nodo::SetPosPadre(int pos_padre) { pos_padre_ = pos_padre; }

/**
 * @brief Sobrecarga del operador == para comparar nodos por su ID.
 * 
 * @param otro Nodo a comparar.
 * @return true Si los nodos son iguales.
 * @return false Si los nodos son diferentes.
 */
bool Nodo::operator==(const Nodo& otro) const { return id_ == otro.id_; }