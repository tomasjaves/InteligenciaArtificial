#include "../include/grafo.h"

/**
 * @brief Constructor para inicializar el grafo con un número de vértices dado.
 * 
 * @param num_vertices 
 */
Grafo::Grafo(int num_vertices) : num_vertices_(num_vertices) {
  // Inicializamos la matriz de adyacencia con -1 (sin conexión).
  matriz_adyacencia_.resize(num_vertices_, std::vector<int>(num_vertices_, -1));
  // Por cada vértice, la distancia a sí mismo es 0.
  for (int i = 0; i < num_vertices_; ++i) {
    matriz_adyacencia_[i][i] = 0;
  }
}

/**
 * @brief Método para cargar el grafo desde un archivo con el formato especificado.
 * 
 * @param nombre_archivo 
 */
Grafo::Grafo(const std::string& nombre_archivo) {
  std::string ruta = "input/" + nombre_archivo;
  // Abrimos el archivo.
  std::ifstream archivo(ruta);
  // Si no se pudo abrir, mostramos un mensaje de error y retornamos.
  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return;
  }

  std::string linea;
  std::getline(archivo, linea);  // Leemos número de vértices.
  std::istringstream iss(linea);
  iss >> num_vertices_;

  // Inicializamos el grafo con el número de vértices leídos.
  *this = Grafo(num_vertices_);  // Usamos el constructor para inicializar el grafo.

  // Leemos las distancias de la matriz de adyacencia.
  int fila = 0, columna = 1, aristas = 0;  // Comenzamos en la primera fila, segunda columna.
  while (std::getline(archivo, linea)) {
    std::istringstream iss(linea);
    int costo;
    iss >> costo;  // Leemos el costo de la arista.

    // Agregamos la arista si hay conexión (costo != -1).
    if (costo != -1) {
      AgregarArista(fila, columna, costo);
      ++aristas;  // Incrementamos el número de aristas.
    }

    // Actualizamos índices para avanzar en la matriz triangular superior.
    ++columna;
    if (columna == num_vertices_) {
      ++fila;
      columna = fila + 1;  // La columna empieza siempre después de la fila.
    }
  }
  num_aristas_ = aristas;  // Actualizamos el número de aristas.
  // Cerramos el archivo después de leer todos los datos.  
  archivo.close();
}

/**
 * @brief Método para agregar una arista entre dos vértices con un costo específico.
 * 
 * @param origen 
 * @param destino 
 * @param costo 
 */
void Grafo::AgregarArista(int origen, int destino, double costo) {
  // Si el origen y destino son válidos, actualizamos la matriz de adyacencia.
  if (origen >= 0 && origen < num_vertices_ && destino >= 0 && destino < num_vertices_) {
    matriz_adyacencia_[origen][destino] = costo;
    matriz_adyacencia_[destino][origen] = costo;
  }
}

/**
 * @brief Función para obtener el número de aristas del grafo.
 * 
 * @return int 
 */
int Grafo::GetNumeroAristas() const {
  return num_aristas_;
}

/**
 * @brief Función para obtener el número de vértices del grafo.
 * 
 * @return int 
 */
int Grafo::GetNumeroVertices() const {
  return num_vertices_;
}

/**
 * @brief Función para obtener la matriz de adyacencia del grafo.
 * 
 * @return const std::vector<std::vector<double>>& 
 */
const std::vector<std::vector<int>>& Grafo::GetMatrizCoste() const {
  return matriz_adyacencia_;
}

/**
 * @brief Método para mostrar la matriz de adyacencia del grafo (para debugging).
 * 
 */
void Grafo::MostrarGrafo() const {
  for (const auto& fila : matriz_adyacencia_) {
    for (double costo : fila) {
      if (costo == -1) {
        // Si no hay conexión, mostramos el costo con un ancho de campo fijo.
        std::cout << std::setw(5) << std::left << "-" << " ";
      } else {
        // Mostrar el costo con 3 decimales y en un ancho de campo fijo.
        std::cout << std::setw(5) << std::left << costo << " ";
      }
    }
    std::cout << std::endl;
  }
}

/**
 * @brief Método para mostrar información del grafo.
 * 
 * @param origen
 * @param destino
 * @param archivo_salida archivo de salida para escribir la información.
 */
void Grafo::MostrarInformacion(const int& origen, const int& destino, std::ofstream& archivo_salida) const {
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
  archivo_salida << "Número de nodos del grafo: " << num_vertices_ << std::endl;
  archivo_salida << "Número de aristas del grafo: " << num_aristas_ << std::endl;
  archivo_salida << "Vértice origen: " << origen << std::endl;
  archivo_salida << "Vértice destino: " << destino << std::endl;
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}

/**
 * @brief Método para mostrar las conexiones de cada vértice en el grafo.
 * 
 * @param fichero_salida fichero de salida para escribir la información.
 */
void Grafo::InformacionConexiones(std::ofstream& fichero_salida) const {
  // 1) Imprimimos el número de vértices y aristas
  fichero_salida << "Número de nodos: " << num_vertices_ << "\n";
  fichero_salida << "Número de aristas: " << num_aristas_ << "\n\n";

  // 2) Listamos todas las aristas con su coste
  int contador_aristas = 1;
  for (int i = 0; i < num_vertices_; ++i) {
    for (int j = i + 1; j < num_vertices_; ++j) {
      // Si hay conexión y el coste no es -1
      if (matriz_adyacencia_[i][j] != -1) {
        fichero_salida << contador_aristas << ".- Conexión ("
                       << i + 1 << ", " << j + 1 << ") = "
                       << matriz_adyacencia_[i][j] << "\n";
        contador_aristas++;
      }
    }
  }
  fichero_salida << "\n";

  // 3) Para cada nodo, mostrar sus hijos (o vecinos)
  for (int i = 0; i < num_vertices_; ++i) {
    // Recogemos los vecinos, excluyendo al propio nodo (i == j)
    std::vector<int> hijos;
    for (int j = 0; j < num_vertices_; ++j) {
      // Verificamos que exista conexión y que j != i
      if (j != i && matriz_adyacencia_[i][j] != -1) {
        hijos.push_back(j + 1);  // +1 para imprimir con base 1
      }
    }

    // Imprimimos la información: "Nodo X tiene Y hijos: ..."
    fichero_salida << "Nodo " << i + 1 << " tiene " << hijos.size() << " hijos: ";
    if (!hijos.empty()) {
      for (size_t k = 0; k < hijos.size(); ++k) {
        fichero_salida << hijos[k];
        if (k < hijos.size() - 1) {
          fichero_salida << ", ";
        }
      }
    }
    fichero_salida << "\n";
  }
}