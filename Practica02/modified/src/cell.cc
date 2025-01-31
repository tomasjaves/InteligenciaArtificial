#include "../include/cell.h"

/**
 * @brief Constructor de la celda
 * 
 * @param i_pos 
 * @param j_pos 
 * @param kind 
 */
Cell::Cell(int i_pos, int j_pos, int kind) 
  : i_pos_(i_pos), j_pos_(j_pos), kind_(kind) {}

/**
 * @brief Función que devuelve la posición de la celda
 * 
 * @return std::string 
 */
std::string Cell::GetPosString() const {
  return "(" + std::to_string(i_pos_) +         // +1 para que empiece en 1
         ", " + std::to_string(j_pos_) + ")";   
}

/**
 * @brief Calcula la distancia euclídea entre dos puntos
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @return int 
 */
int euclideanDistance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

/**
 * @brief Calcula la heurística de la celda durante la búsqueda
 * 
 * @param end_node 
 * @param chosen_heuristic 
 */
void Cell::CalculateHeuristic(Cell end_node, int chosen_heuristic) {
  switch (chosen_heuristic) {
  default: // Por defecto, se utiliza la distancia Manhattan
  case 1:  // Distancia Manhattan
    h_value_ = (std::abs(i_pos_ - end_node.GetIPos()) +  
               std::abs(j_pos_ - end_node.GetJPos())) * 3;
    break;
  case 2: // Distancia euclídea
    h_value_ = euclideanDistance(i_pos_, j_pos_, 
        end_node.GetIPos(), end_node.GetJPos());
    break;
  }
}

/**
 * @brief Imprime los valores de la celda
 * 
 * @return std::string 
 */
std::string Cell::PrintValues() const {
  return "f: " + std::to_string(f_value_) + ", g: " + std::to_string(g_value_)
          + ", h: " + std::to_string(h_value_);
}

/**
 * @brief Comprueba si la celda es diagonal a otra
 * 
 * @param node 
 * @param labyrinth 
 * @return true 
 * @return false 
 */
bool Cell::IsDiagonal(Cell node, std::vector<std::vector<Cell>> labyrinth) const {
  std::pair<int, int> a_pos = GetPos();
  std::pair<int, int> b_pos = node.GetPos();
  int dx = std::abs(a_pos.first - b_pos.first);
  int dy = std::abs(a_pos.second - b_pos.second);
  return dx == 1 && dy == 1; // Si dx y dy son 1, los nodos están en diagonal
}