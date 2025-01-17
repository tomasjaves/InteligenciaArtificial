#pragma once

#include "cell.h"
#include <cstdlib>
#include <ctime>
#include <random>

// Define un vector de celdas como CellVector
typedef std::vector<Cell> CellVector;

// Representa una solución de la búsqueda
struct Instance {
  CellVector path;          // Camino de la solución
  CellVector generated;     // Nodos generados durante la búsqueda
  CellVector visited;       // Nodos visitados durante la búsqueda
  std::string iteraciones;  // String con las iteraciones de la instancia
  std::string dibujo;       // Dibujo del laberinto resuelto
};

// Clase Labyrinth 
class Labyrinth {
 public:
  // Constructores
  Labyrinth(std::ifstream& input_file);
  
  // Imprime el laberinto
  std::string PrintLabyrinth(CellVector open_nodes = {}, CellVector closed_nodes = {}, 
                      Cell current_node = Cell(), CellVector path = {}) const;

  // Cambia el nodo inicial o final para poder modificarlo manualmente
  void ChangeNode(bool final);
 
  // Getters
  Cell GetStartNode() const { return start_node_; }
  Cell GetEndNode() const { return end_node_; }
  int GetRows() const { return rows_; }
  int GetColumns() const { return columns_; }
  int GetChosenHeuristic() const { return chosen_heuristic_; }
  std::vector<CellVector> GetLabyrinth() const { return labyrinth_; } // Devuelve el laberinto
  
  // Obtiene los vecinos de un nodo
  CellVector GetNeighbors(Cell node) const;

  // Calcula los valores de un nodo
  void CalculateValues(Cell& node, Cell& current_node) const;

  // Accede a un nodo del laberinto por su posición
  Cell& Node(std::pair<int, int> pos) { return labyrinth_[pos.first][pos.second]; }

  // Permite cambiar la eurística
  void ChangeHeuristic();

  // Búsqueda A*
  Instance AStarSearch() const;

  // MODIFICACION

  Instance AStarSearchModified() const;
 
 private:
  // Atributos
  std::vector<CellVector> labyrinth_;
  int rows_, columns_;
  Cell start_node_, end_node_;
  int chosen_heuristic_{1};

};
