#pragma once

class Nodo {
 public:
  // Constructor por defecto
  Nodo();
  Nodo(int, int, int, Nodo*, int);

  // Métodos getter y setter para acceder a los atributos
  int GetID() const;
  int GetCoste() const;
  Nodo* GetPadre() const;
  int GetPosPadre() const;
  int GetProfundidad() const;

  void SetID(int);
  void SetCoste(int);
  void SetPadre(Nodo*);
  void SetPosPadre(int);
  void SetProfundidad(int);

  // Sobrecargar operador == para comparar nodos por su ID
  bool operator==(const Nodo&) const;

 private:
  int id_;           // ID del nodo
  int pos_padre_;     // Posición del padre en la estructura de nodos
  int coste_;         // Coste de la arista hacia este nodo
  Nodo* padre_;         // ID del nodo padre
  int profundidad_;   // Profundidad del nodo en el árbol
};
