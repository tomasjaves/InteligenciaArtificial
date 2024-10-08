#pragma once

#include "grafo.h"
#include "nodo.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <set>
#include <stack>

/**
 * @brief Clase que implementa las búsquedas en el grafo.
 */
class Busqueda {
 public:
  // Función para realizar búsqueda en amplitud (BFS)
  static std::vector<Nodo> BusquedaEnAmplitud(const Grafo&, const int&, const int&, int&, std::ofstream&);

  // Función para realizar búsqueda en profundidad (DFS)
  static std::vector<Nodo> BusquedaEnProfundidad(const Grafo&, const int&, const int&, int&, std::ofstream&);
};
