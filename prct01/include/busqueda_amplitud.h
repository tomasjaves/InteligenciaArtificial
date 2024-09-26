#pragma once

#include "grafo.h"
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <fstream>

// Función que implementa la búsqueda en amplitud (BFS).
std::vector<int> BusquedaEnAmplitud(const Grafo&, int, int, double&, std::ofstream&);