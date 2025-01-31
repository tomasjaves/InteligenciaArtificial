#pragma once

#include "grafo.h"
#include "nodo.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <set>
#include <stack>
#include <algorithm>
#include <limits>
#include <queue>

/**
 * @brief Clase que implementa las búsquedas en el grafo.
 */
class Busqueda {
 public:
  // Función para realizar búsqueda en amplitud (BFS)
  static std::vector<Nodo> BusquedaEnAmplitud(const Grafo&, const int&, const int&, int&, std::ofstream&);

  // Función para realizar búsqueda en profundidad (DFS)
  static std::vector<Nodo> BusquedaEnProfundidad(const Grafo&, const int&, const int&, int&, std::ofstream&);

 private:
  private:
  // Función recursiva “interna” que hace la DFS
  void static DFSRecursivo(const Grafo& grafo,
                    Nodo* nodo_actual,
                    int id_destino,
                    std::unordered_map<int, bool>& visitado,
                    std::vector<Nodo>& generados,
                    std::vector<Nodo>& inspeccionados,
                    std::vector<Nodo>& camino_actual,
                    int& costo_total,
                    std::ofstream& archivo_salida,
                    int& iteracion,
                    bool& encontrado);
};