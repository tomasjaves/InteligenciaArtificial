#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>
#include <sstream>
#include <iomanip>

#include <filesystem>

// Definición de colores para la salida en consola.
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define PINK "\033[95m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GRAY "\033[90m"
#define TURQUOISE "\033[36m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

class Cell;
class Labyrinth;

// Define un vector de celdas como CellVector
typedef std::vector<Cell> CellVector;

// Función de uso para comprobar parámetros y mostrar uso
void Uso(int argc, char *argv[]);

// Función que imprime el menú
void ImprimirMenu();

// Convierte un vector de celdas a un string
std::string VectorToString(CellVector vector);

// Comprueba si un nodo está disponible para transitar
bool IsNodeValid(int row, int col, Labyrinth& labyrinth);

// Realiza el flujo de salida que contiene la solución de la búsqueda
std::ofstream StoreSearch(Labyrinth& labyrinth, std::string& instance_name);

// Construye el camino de la solución a partir de los padres
CellVector ConstructPath(Cell current_node, Cell start_node,
                         std::vector<std::pair<Cell,Cell>> parents);

// Actualiza el padre de un nodo si es mejor que el actual
void UpdateIfBetter(Cell& node, Cell& current_node, 
                          std::vector<CellVector> labyrinth,
                          std::vector<std::pair<Cell,Cell>>& parents, 
                          CellVector& open_nodes);

// Comprueba si un nodo vecino es inválido para transitar
bool InvalidNeighbor(Cell& neighbor, Cell& current_node, 
                                     CellVector& closed_nodes);

// MODIFICACION
std::ofstream StoreSearchModified(Labyrinth& labyrinth, std::string& instance_name);