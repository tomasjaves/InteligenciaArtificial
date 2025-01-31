#include "../include/aux_functions.h"
#include "../include/cell.h"
#include "../include/labyrinth.h"

/**
 * @brief Uso del programa
 * 
 * @param argc 
 * @param argv 
 */
void Uso(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << RED << BOLD << "Error: Parámetros inválidos.\n" << RESET << GRAY << ITALIC << "Prueba " << RESET << GRAY << BOLD << "'./program input.txt'\n";
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Función que imprime el menú
 * 
 */
void ImprimirMenu() {
  std::cout << GRAY << BOLD << "Menú de opciones:\n" << RESET;
  std::cout << BOLD << PINK << "1. " << RESET << "Búsqueda A*\n";
  std::cout << BOLD << PINK << "2. " << RESET << "Cambiar casilla inicial\n";
  std::cout << BOLD << PINK << "3. " << RESET << "Cambiar casilla final\n";
  std::cout << BOLD << PINK << "4. " << RESET << "Cambiar heurística\n";
  std::cout << BOLD << PINK << "5. " << RESET << "Imprimir laberinto\n";
  std::cout << BOLD << PINK << "6. " << RESET << "Cambiar fichero de entrada\n";
  std::cout << BOLD << PINK << "7. " << RESET << "Salir\n";
}

/**
 * @brief Función que convierte un vector de celdas a un string
 * 
 * @param vector 
 * @return std::string 
 */
std::string VectorToString(CellVector vector) {
  std::string result;
  if (vector.size() == 0) return "No path";  // Si no hay camino
  for (int i = 0; i < vector.size(); i++) {
    result += vector[i].GetPosString();  // Añade la posición del nodo de la forma (x,y)
    if (i != vector.size() - 1) result += " - "; // Añade un guión entre nodos
  }
  return result;
}

/**
 * @brief Función que centra un texto en una cadena
 * 
 * @param text 
 * @param width 
 * @return std::string 
 */
std::string CenterText(const std::string& text, int width) {
  int padding = width - text.size();
  int padding_left = padding / 2;
  int padding_right = padding - padding_left;
  return std::string(padding_left, ' ') + text + std::string(padding_right, ' ');
}

/**
 * @brief Función que almacena la búsqueda en un archivo
 * 
 * @param labyrinth 
 * @param instance_name 
 * @return std::ofstream 
 */
std::ofstream StoreSearch(Labyrinth& labyrinth, std::string& instance_name) {
  std::ofstream output_file;
  if (!std::filesystem::exists("output")) {
    std::filesystem::create_directory("output");
  }
  output_file.open("output/instancias.md", std::ios_base::app);  // Abre el fichero en modo append

  std::string id = instance_name.substr(7, 2);  // Obtiene el identificador de la instancia

  // Realizamos la búsqueda
  Instance table = labyrinth.AStarSearchModified();

  // Extraemos la información
  std::string path = VectorToString(table.path);  // Convierte el camino a string
  int generados = table.generated.size();
  int visitados = table.visited.size();
  int cost = 0;
  std::string dibujo = table.dibujo;

  // Si hay camino, obtenemos el coste
  if (path != "No path") cost = table.path.back().GetGValue();

  // Ancho fijo para cada columna
  const int col_widths[] = {10, 5, 5, 7, 20, 22};

  // Cabecera de la tabla
  output_file << "\n";
  output_file << "| " << CenterText("Instancia", col_widths[0])
              << " | " << CenterText("n", col_widths[1])
              << " | " << CenterText("m", col_widths[2])
              << " | " << CenterText("Coste", col_widths[3])
              << " | " << CenterText("Nodos Generados", col_widths[4])
              << " | " << CenterText("Nodos inspeccionados", col_widths[5]) << " |\n";

  output_file << "|-" << std::string(col_widths[0], '-') << "-|-" 
              << std::string(col_widths[1], '-') << "-|-" 
              << std::string(col_widths[2], '-') << "-|-" 
              << std::string(col_widths[3], '-') << "-|-" 
              << std::string(col_widths[4], '-') << "-|-" 
              << std::string(col_widths[5], '-') << "-|\n";

  // Contenido de la tabla
  output_file << "| " << CenterText(id, col_widths[0])
              << " | " << CenterText(std::to_string(labyrinth.GetRows()), col_widths[1])
              << " | " << CenterText(std::to_string(labyrinth.GetColumns()), col_widths[2])
              << " | " << CenterText(std::to_string(cost), col_widths[3])
              << " | " << CenterText(std::to_string(generados), col_widths[4])
              << " | " << CenterText(std::to_string(visitados), col_widths[5]) << " |\n\n";

  output_file << "S: " << labyrinth.GetStartNode().GetPosString() << "\n";
  output_file << "E: " << labyrinth.GetEndNode().GetPosString() << "\n";
  output_file << "Camino: " << path << "\n";

  // Añadimos otros datos adicionales
  output_file << "\n" << dibujo << "\n\n";
  output_file << "Iteraciones: \n\n" << table.iteraciones << "\n\n";

  // Cerramos el archivo
  output_file.close();
  return output_file;
}

/**
 * @brief Comprueba si un nodo está disponible para transitar
 * 
 * @param row
 * @param col
 * @param labyrinth
 * @return true
 * @return false
 */
bool IsNodeValid(int row, int col, Labyrinth& labyrinth) {
  if (row < 0 || col < 0) return false; // Si el nodo está fuera del laberinto
  if (row >= labyrinth.GetRows() || col >= labyrinth.GetColumns()) return false; 
  if (labyrinth.Node(std::make_pair(row, col)).GetKind() == 1) return false; // Si el nodo es un muro
  return true;
}

/**
 * @brief Construye el camino de la solución a partir de los padres
 * 
 * @param current_node
 * @param start_node
 * @param parents
 * @return CellVector
 */
CellVector ConstructPath(Cell current_node, Cell start_node,
                         std::vector<std::pair<Cell,Cell>> parents) {
  CellVector path;
  while (current_node.GetKind() != 3) {         // Mientras no se llegue al nodo inicial
    path.push_back(current_node); 
    for (int i = 0; i < parents.size(); i++) {
      if (parents[i].first == current_node) {
        current_node = parents[i].second;      // Se añade el padre del nodo actual al camino
        break;
      }
    }
  }
  path.push_back(start_node);
  std::reverse(path.begin(), path.end());      // Se invierte el camino para mostrarlo de la forma correcta
  return path;
}

/**
 * @brief Actualiza el padre de un nodo si es mejor que el actual
 * 
 * @param node
 * @param current_node
 * @param labyrinth
 * @param parents
 * @param open_nodes
 */
void UpdateIfBetter(Cell& node, Cell& current_node, 
                          std::vector<CellVector> labyrinth,
                          std::vector<std::pair<Cell,Cell>>& parents,
                          CellVector& open_nodes) {
  int g_value = current_node.GetGValue();
  if (current_node.IsDiagonal(node, labyrinth)) g_value += 7; // Si el nodo es diagonal el coste es 7
  else g_value += 5;                                          // Si el nodo es adyacente el coste es 5
  for (int i = 0; i < parents.size(); i++) {  
    if (parents[i].first == node) {
      if (g_value < parents[i].first.GetGValue()) {                           // Si el padre actual es mejor que el anterior
        parents[i].second = current_node;                                     // Se actualiza el padre
        parents[i].first.SetGValue(g_value);                                  // Se actualiza el coste g
        parents[i].first.SetFValue(g_value + parents[i].first.GetHValue());   // Se actualiza el coste f
        for (int j = 0; j < open_nodes.size(); j++) {                         // Se actualiza el nodo en la lista de nodos abiertos
          if (open_nodes[j] == parents[i].first) {
            open_nodes[j] = parents[i].first;
            break;
          }
        }
      }
      break;
    }
  }
}

/**
 * @brief Comprueba si un nodo vecino es inválido para transitar
 * 
 * @param neighbor
 * @param current_node
 * @param closed_nodes
 * @return true
 * @return false
 */
bool InvalidNeighbor(Cell& neighbor, Cell& current_node, 
                     CellVector& closed_nodes) {
  return neighbor.GetPos() == std::make_pair(-1, -1) ||  // Si el nodo no existe en el laberinto
  IsClosedNode(neighbor, closed_nodes);                  // Si el nodo ya está cerrado
}

// MODIFICACION

std::ofstream StoreSearchModified(Labyrinth& labyrinth, std::string& instance_name) {
  std::ofstream output_file;

  // Crear el directorio "output" si no existe
  if (!std::filesystem::exists("output")) {
    std::filesystem::create_directory("output");
  }
  output_file.open("output/instancias.md", std::ios_base::app);  // Abre el fichero en modo append

  std::string id = instance_name.substr(7, 2);  // Obtiene el identificador de la instancia

  // Cabecera inicial
  output_file << "Resultados para la instancia " << id << ":\n";
  output_file << std::string(50, '-') << "\n\n";

  // Ejecutar el algoritmo modificado 10 veces
  for (int iteracion_global = 1; iteracion_global <= 10; ++iteracion_global) {
    // Realizar la búsqueda usando el algoritmo A* modificado
    Instance table = labyrinth.AStarSearchModified();

    // Extraer información de la instancia
    std::string path = VectorToString(table.path);  // Convierte el camino a string
    int generados = table.generated.size();
    int visitados = table.visited.size();
    int cost = 0;
    std::string dibujo = table.dibujo;

    // Si hay camino, obtenemos el coste
    if (path != "No path") cost = table.path.back().GetGValue();

    // Ancho fijo para cada columna
    const int col_widths[] = {10, 5, 5, 7, 20, 22};

    // Cabecera de la tabla
    if (iteracion_global == 1) {
      output_file << "| " << CenterText("Iteración", col_widths[0])
                  << " | " << CenterText("n", col_widths[1])
                  << " | " << CenterText("m", col_widths[2])
                  << " | " << CenterText("Coste", col_widths[3])
                  << " | " << CenterText("Nodos Generados", col_widths[4])
                  << " | " << CenterText("Nodos inspeccionados", col_widths[5]) << " |\n";

      output_file << "|-" << std::string(col_widths[0], '-') << "-|-" 
                  << std::string(col_widths[1], '-') << "-|-" 
                  << std::string(col_widths[2], '-') << "-|-" 
                  << std::string(col_widths[3], '-') << "-|-" 
                  << std::string(col_widths[4], '-') << "-|-" 
                  << std::string(col_widths[5], '-') << "-|\n";
    }

    // Contenido de la tabla por iteración
    output_file << "| " << CenterText(std::to_string(iteracion_global), col_widths[0])
                << " | " << CenterText(std::to_string(labyrinth.GetRows()), col_widths[1])
                << " | " << CenterText(std::to_string(labyrinth.GetColumns()), col_widths[2])
                << " | " << CenterText(std::to_string(cost), col_widths[3])
                << " | " << CenterText(std::to_string(generados), col_widths[4])
                << " | " << CenterText(std::to_string(visitados), col_widths[5]) << " |\n";

    // Detalles adicionales para cada iteración
    output_file << "Camino: " << path << "\n";
    output_file << "S: " << labyrinth.GetStartNode().GetPosString() << "\n";
    output_file << "E: " << labyrinth.GetEndNode().GetPosString() << "\n";

    // Nodos inspeccionados
    output_file << "Nodos inspeccionados: ";
    for (const auto& cell : table.visited) {
      output_file << "(" << cell.GetIPos() << "," << cell.GetJPos() << ") ";
    }
    output_file << "\n";

    // Dibujo del laberinto
    output_file << "\n" << dibujo << "\n\n";

    // Iteraciones de la ejecución
    output_file << "Iteraciones: \n\n" << table.iteraciones << "\n\n";

    // Separador entre iteraciones
    output_file << std::string(50, '-') << "\n\n";
  }

  // Cerrar el archivo
  output_file.close();
  return output_file;
}