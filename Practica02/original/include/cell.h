#pragma once

#include "aux_functions.h"

// Clase Cell
class Cell {
 public:
  // Constructores
  Cell(int i_pos, int j_pos, int kind);
  Cell() = default; // Constructor por defecto
  
  // Getters
  int GetIPos() const { return i_pos_; }
  int GetJPos() const { return j_pos_; }
  int GetKind() const { return kind_; }
  std::pair<int, int> GetPos() const { return std::make_pair(i_pos_, j_pos_); }
  std::string GetPosString() const;
  int GetFValue() const { return f_value_; }
  int GetGValue() const { return g_value_; }
  int GetHValue() const { return h_value_; }
  
  // Imprime los valores de la celda
  std::string PrintValues() const;

  // Setters
  void SetKind(int kind) { kind_ = kind; }
  // Obtenemos el valor g(n) de la celda
  void SetGValue(int g_value) { g_value_ = g_value; }
  // Obtenemos el valor f(n) de la celda
  void SetFValue(int f_value) { f_value_ = f_value; }
  // Obtenemos el valor h(n) de la celda
  void SetHValue(int h_value) { h_value_ = h_value; }
  
  // Calcula la heurística de la celda durante la búsqueda
  void CalculateHeuristic(Cell end_node, int chosen_heuristic = 1);

  // Comprueba si la celda es diagonal a otra
  bool IsDiagonal(Cell node, std::vector<std::vector<Cell>> labyrinth) const;
  
  // Sobrecarga de operadores
  bool operator==(const Cell& cell) const { return GetPos() == cell.GetPos(); }
  bool operator<(const Cell& cell) const { return GetFValue() < cell.GetFValue(); }
 
 private:
  // Atributos
  int i_pos_{0}, j_pos_{0}, kind_{0};
  // Coste acumulado g(n), heurística h(n) y total f(n)
  int f_value_{0}, g_value_{0}, h_value_{0}; 

  Cell* parent_;
};

// Define un vector de Cells como CellVector
typedef std::vector<Cell> CellVector;

// Comprueba si un nodo está en el vector de nodos abiertos
inline bool IsOpenNode(Cell node, CellVector open) {
  return std::find(open.begin(), open.end(), node) != open.end();
}

// Comprueba si un nodo está en el vector de nodos cerrados
inline bool IsClosedNode(Cell node, CellVector closed) {
  return std::find(closed.begin(), closed.end(), node) != closed.end();
}

// Comprueba si un nodo está en el vector del camino final
inline bool IsInPath(Cell node, CellVector path) {
  return std::find(path.begin(), path.end(), node) != path.end();
}