#pragma once

#include "../include/grafo.h"
#include <stack>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>

// Función que implementa la búsqueda en profundidad (DFS).
std::vector<int> BusquedaEnProfundidad(const Grafo&, int, int, double&, std::ofstream&);
