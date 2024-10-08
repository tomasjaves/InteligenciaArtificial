#pragma once

#include "grafo.h"
#include "busqueda_comun.h"
#include "nodo.h"
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <fstream>
#include <unordered_set>

// Función que implementa la búsqueda en amplitud (BFS).
void BusquedaEnAmplitud(const Grafo&, const int&, const int&, int&, std::ofstream&);