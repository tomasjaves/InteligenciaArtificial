#pragma once

#include "nodo.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief Clase que representa un grafo no dirigido mediante una matriz de
 * adyacencia.
 */
class Grafo {
 public:
  // Constructores de la clase.
  Grafo(int);
  Grafo(const std::string&);

  // Métodos para añadir aristas al grafo.
  void AgregarArista(int, int, double);

  // Métodos getter para acceder a los atributos.
  int GetNumeroAristas() const;
  int GetNumeroVertices() const;
  const std::vector<std::vector<int>>& GetMatrizCoste() const;

  // Métodos para mostrar información del grafo.
  void MostrarGrafo() const;
  void MostrarInformacion(const int&, const int&, std::ofstream&) const;

 private:
  int num_vertices_;  // Número de vértices en el grafo.
  int num_aristas_;  // Número de aristas en el grafo.
  std::vector<std::vector<int>> matriz_adyacencia_;  // Matriz de adyacencia.
};