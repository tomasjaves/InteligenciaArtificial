#include "../include/labyrinth.h"

/**
 * @brief Constructor de la clase Labyrinth
 * 
 * @param input_file 
 */
Labyrinth::Labyrinth(std::ifstream& input_file) {
  std::string line;
  std::getline(input_file, line);                      // La primera línea contiene el número de filas
  rows_ = std::stoi(line);
  std::getline(input_file, line);                      // La segunda línea contiene el número de columnas
  columns_ = std::stoi(line);
  for (int i = 0; i < rows_; i++) {   
    CellVector row;
    std::getline(input_file, line);
    for (int j = 0, k = 0; j < columns_; j++, k++) {      // Se lee el laberinto
      if (line[k] == '3') {start_node_ = Cell(i, j, 3);}  // El nodo inicial se representa con un 3
      if (line[k] == '4') end_node_ = Cell(i, j, 4);      // El nodo final se representa con un 4
      row.push_back(Cell(i, j, line[k++] - '0'));         // El resto de nodos se representan con un 0 o un 1
    }
    labyrinth_.push_back(row);                            // Se añade la fila al laberinto
  }
}

/**
 * @brief Comprueba si un nodo es un nodo cerrado
 * 
 * @param node 
 * @param closed_nodes 
 * @return true 
 * @return false 
 */
std::string Labyrinth::PrintLabyrinth(CellVector open_nodes, CellVector closed_nodes,
                               Cell current_node, CellVector path) const {
  std::stringstream laberinto_str;
  for (std::vector<Cell> row : labyrinth_) {
    for (Cell cell : row) {
      if (cell.GetKind() == 1) laberinto_str << "█";   // Muro = Cuadrado negro
      if (cell.GetKind() == 3) laberinto_str << "I";   
      if (cell.GetKind() == 4) laberinto_str << "F";  
      if (cell.GetKind() == 0) { 
        if (IsClosedNode(cell, closed_nodes)){
          if (cell == current_node || IsInPath(cell, path)) 
            laberinto_str << "*";      // Nodo parte del camino final
          else laberinto_str << "·";                    // Nodo cerrado
        } else {
          laberinto_str << "·";                         // Nodo abierto
        }
      }
    }
    laberinto_str << "\n";
  }
  std::string salida = laberinto_str.str();
  return salida;
}

/**
 * @brief Función que cambia el nodo inicial/final a otra posición del laberinto
 * 
 * @param bool
 */
void Labyrinth::ChangeNode(bool final) {
  int i_pos, j_pos;
  std::cout << "Introduce la fila: ";
  std::cin >> i_pos;
  std::cout << "Introduce la columna: ";
  std::cin >> j_pos;
  // Se comprueba que el nodo esté dentro del laberinto
  if (i_pos < 0 || i_pos > rows_ - 1 || j_pos < 0 || j_pos > columns_ - 1) {  
    std::cerr << "Coordenadas fuera de rango. Cambios no aplicados.\n";
    return;
  }
  if (final) {
    std::pair<int, int> end_pos = end_node_.GetPos();   // El nodo final se convierte en un muro
    Node(end_pos).SetKind(1);
    labyrinth_[i_pos][j_pos] = Cell(i_pos, j_pos, 4);
    end_node_ = Cell(i_pos, j_pos, 4);                  // Se actualiza el nodo final
  } else {
    std::pair<int, int> start_pos = start_node_.GetPos();
    Node(start_pos).SetKind(1);                         // El nodo inicial se convierte en un muro
    labyrinth_[i_pos][j_pos] = Cell(i_pos, j_pos, 3);
    start_node_ = Cell(i_pos, j_pos, 3);                // Se actualiza el nodo inicial
  }
  std::cout << "The " << (final ? "final" : "initial") << " node has been set to"
            << " (" << i_pos << ", " << j_pos << ")\n"; // Se imprime el nodo modificado
}

/**
 * @brief Función que obtiene los vecinos de un nodo
 * 
 * @param node 
 * @return CellVector 
 */
CellVector Labyrinth::GetNeighbors(Cell node) const {
  CellVector neighbors;
  std::pair<int, int> pos = node.GetPos();
  Labyrinth labyrinth = *this;
  int i = pos.first, j = pos.second;
  for (int di = -1; di <= 1; di++) {                // Se comprueban los 8 vecinos del nodo
    for (int dj = -1; dj <= 1; dj++) {
      if (di == 0 && dj == 0) continue;
      if (IsNodeValid(i + di, j + dj, labyrinth)) { // Si el nodo es válido, se añade al vector de vecinos
        neighbors.push_back(labyrinth_[i + di][j + dj]);
      } else {
        neighbors.push_back(Cell(-1, -1, 1));       // Si el nodo no es válido, se añade un nodo inválido (-1, -1)
      }
    }
  }
  return neighbors;
}

/**
 * @brief Función que permite cambiar la heurística
 * 
 */
void Labyrinth::ChangeHeuristic() {
  std::cout << "Choose a heuristic:\n";
  std::cout << "1. Manhattan Distance\n";
  std::cout << "2. Euclidian Distance\n";
  char option;
  std::cin >> option;
  switch (option) {
    case '1': // Distancia Manhattan
      chosen_heuristic_ = 1;
      break;
    case '2': // Distancia Euclidiana
      chosen_heuristic_ = 2;
      break;
    default: // Opción inválida
      std::cout << "\nInvalid option\n\n";
      break;
  }
}

/**
 * @brief Función que calcula los valores de un nodo.
 * Calcula el valor g(n), h(n) y f(n) de un nodo.
 * 
 * @param node 
 * @param open_nodes 
 * @return true 
 * @return false 
 */
void Labyrinth::CalculateValues(Cell& node, Cell& current_node) const {
  if (current_node.IsDiagonal(node, labyrinth_))                // Actualiza el valor g(n) del nodo:
    node.SetGValue(current_node.GetGValue() + 7);               // Si el nodo es diagonal, el coste es 7
  else node.SetGValue(current_node.GetGValue() + 5);            // Si el nodo es adyacente, el coste es 5
  node.CalculateHeuristic(GetEndNode(), GetChosenHeuristic());  // Se calcula la heurística del nodo h(n) y su f(n)
  node.SetFValue(node.GetGValue() + node.GetHValue());          // Se establece el valor de F(n)
}

/**
 * @brief Función que ordena los nodos según el valor f(n)
 * 
 * @param node 
 * @param closed_nodes 
 * @return true 
 * @return false 
 */
CellVector SortByFValue(CellVector nodes) {
  // Se ordena el vector de nodos según el valor f(n)
  std::sort(nodes.begin(), nodes.end(), [](Cell a, Cell b) {
    return a.GetFValue() < b.GetFValue();
  });
  return nodes;
}

/**
 * @brief Función que ejecuta el algoritmo de la búsqueda A*
 * 
 * @param node 
 * @param open_nodes 
 * @return true 
 * @return false 
 */
Instance Labyrinth::AStarSearch() const {
  // Stringstream para guardar el detalle de cada iteración (nodos generados e inspeccionados)
  std::stringstream iteracion_str;  

  int iteracion = 0;  // Contador de iteraciones

  CellVector open_nodes{GetStartNode()};  // Lista de nodos abiertos (nodos por explorar)
  CellVector closed_nodes;                // Lista de nodos cerrados (nodos ya inspeccionados)
  CellVector generated{GetStartNode()};   // Lista de todos los nodos generados

  Cell current_node = GetStartNode();          // El nodo actual que estamos explorando
  std::vector<std::pair<Cell, Cell>> parents;  // Vector para guardar las relaciones padre-hijo (para construir el camino final)

  // Cálculo de la heurística para el nodo inicial (f(n) = g(n) + h(n))
  current_node.CalculateHeuristic(GetEndNode(), GetChosenHeuristic());
  current_node.SetFValue(current_node.GetGValue() + current_node.GetHValue());

  // Imprimir el nodo inicial generado (detalles de la primera iteración)
  iteracion_str << "Iteración " << iteracion++ << "\n";
  iteracion_str << "Nodos generados: (" << current_node.GetIPos() << ", " << current_node.GetJPos() << ")\n";
  iteracion_str << "Nodos inspeccionados: -\n\n"; 

  bool path_found = false;  // Indica si se ha encontrado el camino
  bool first = true;        // Indica si estamos en la primera iteración
  const int max_iter = 1000;  // Número máximo de iteraciones
  int iter = 0;  // Contador de iteraciones

  // Bucle principal de la búsqueda A*
  while (iter < max_iter) {
    iteracion_str << "Iteración " << iteracion++ << "\n";

    // Si no hay más nodos abiertos, significa que no existe un camino hacia el nodo final
    if (open_nodes.empty()) {
      return Instance{{}, generated, closed_nodes};  // Devolver una instancia vacía
    }

    // Seleccionar el nodo a explorar
    if (first) {
      // En la primera iteración simplemente se salta este paso
      first = false;
    } else {
      // A partir de las siguientes iteraciones, seleccionar el nodo con menor f(n)
      current_node = open_nodes[0];
      open_nodes.erase(open_nodes.begin());  // Eliminamos el nodo seleccionado de los nodos abiertos
    }

    // Añadir el nodo actual a los nodos cerrados (ya inspeccionado)
    closed_nodes.push_back(current_node);

    // Comprobar si hemos llegado al nodo final (se asume que el nodo final tiene Kind == 4)
    if (current_node.GetKind() == 4) {
      // Si encontramos el nodo final, imprimimos el estado de los nodos generados e inspeccionados
      iteracion_str << "Nodos generados: \n";
      for (const auto& node : generated) {
        iteracion_str << "(" << node.GetIPos() << "," << node.GetJPos() << ") ";
      }
      iteracion_str << "\n";

      iteracion_str << "Nodos inspeccionados: \n";
      for (const auto& node : closed_nodes) {
        iteracion_str << "(" << node.GetIPos() << "," << node.GetJPos() << ") ";
      }
      iteracion_str << "\n\n";
      break;  // Se ha encontrado el camino, salir del bucle
    }

    // Explorar todos los vecinos del nodo actual
    for (Cell neighbor : GetNeighbors(current_node)) {
      // Si el vecino es inválido (ya inspeccionado o no transitable), continuar con el siguiente
      if (InvalidNeighbor(neighbor, current_node, closed_nodes)) continue;

      // Si el vecino no está en los nodos abiertos, lo añadimos como nuevo nodo generado
      if (!IsOpenNode(neighbor, open_nodes)) {
        // Calculamos sus valores g(n), h(n) y f(n)
        CalculateValues(neighbor, current_node);

        // Añadimos el vecino a los nodos abiertos y a los nodos generados
        open_nodes.push_back(neighbor);
        generated.push_back(neighbor);

        // Guardamos la relación (padre-hijo) entre el nodo actual y el vecino
        parents.push_back(std::make_pair(neighbor, current_node));
      } else {
        // Si el vecino ya está en open_nodes, se actualizan los valores si encontramos un mejor camino
        UpdateIfBetter(neighbor, current_node, GetLabyrinth(), parents, open_nodes);
      }
    }

    // Imprimir todos los nodos generados hasta ahora
    iteracion_str << "Nodos generados: \n";
    for (const auto& node : generated) {
      iteracion_str << "(" << node.GetIPos() << "," << node.GetJPos() << ") ";
    }
    iteracion_str << "\n";

    // Imprimir todos los nodos inspeccionados (en closed_nodes) hasta ahora
    iteracion_str << "Nodos inspeccionados: \n";
    for (const auto& node : closed_nodes) {
      iteracion_str << "(" << node.GetIPos() << "," << node.GetJPos() << ") ";
    }
    iteracion_str << "\n\n";

    // Reordenar los nodos abiertos según el valor de f(n) para la siguiente iteración
    if (!open_nodes.empty()) {
      open_nodes = SortByFValue(open_nodes);
    }
    ++iter;
  }

  // Construir el camino desde el nodo inicial hasta el nodo final
  CellVector path = ConstructPath(current_node, GetStartNode(), parents);

  // Generar una representación final del laberinto con el camino encontrado
  std::string dibujo = PrintLabyrinth(open_nodes, closed_nodes, Cell(-1, -1, -1), path);

  // Devolver la instancia con el camino, nodos generados, nodos inspeccionados, las iteraciones y el dibujo de la solución
  std::string iteraciones = iteracion_str.str();
  return Instance{path, generated, closed_nodes, iteraciones, dibujo};
}