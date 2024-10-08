#pragma once

#include <vector>
#include <fstream>

// Función para insertar un nodo en orden ascendente en un vector.
void InsertarMenorAMayor(std::vector<int>&, int);
// Función para mostrar la información de los nodos generados.
void InformacionNodosGenerados(const std::vector<int>&, std::ofstream&);
// Función para mostrar la información de los nodos inspeccionados.
void InformacionNodosInspeccionados(const std::vector<int>&, std::ofstream&);