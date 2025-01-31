#pragma once

/**
 * @brief Clase que representa un nodo en un árbol de búsqueda.
 */
class Nodo {
 public:
  // Constructores de la clase
  Nodo();
  Nodo(const int&);
  Nodo(const int&, const int&, const int&, Nodo*);

  // Métodos getter y setter para acceder a los atributos
  int GetID() const;
  int GetCoste() const;
  Nodo* GetPadre() const;
  int GetPosPadre() const;

  // Métodos setter para establecer los atributos
  void SetID(int);
  void SetCoste(int);
  void SetPadre(Nodo*);
  void SetPosPadre(int);

  // Sobrecargar operador == para comparar nodos por su ID
  bool operator==(const Nodo&) const;

 private:
  int id_;            // ID del nodo
  int pos_padre_;     // Posición del padre en la estructura de nodos
  int coste_;         // Coste de la arista hacia este nodo
  Nodo* padre_;       // ID del nodo padre
};