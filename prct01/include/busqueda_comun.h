#pragma once

#include <vector>
#include <fstream>

// Función para insertar un nodo en orden ascendente en un vector.
void InsertarMenorAMayor(std::vector<int>&, int);
// Función para mostrar información de la iteración actual.
void InformacionIteracion(int&, const std::vector<int>&, const std::vector<int>&, std::ofstream&);