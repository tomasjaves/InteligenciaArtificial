#include "../include/grafo.h"

#include <fstream>
#include <iostream>
#include <sstream>

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
void Grafo::CargarDesdeArchivo(const std::string& nombre_archivo) {
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
 */
void Grafo::MostrarInformacion(const int& origen, const int& destino, std::ofstream& archivo_salida) const {
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
  archivo_salida << "Número de nodos del grafo: " << num_vertices_ << std::endl;
  archivo_salida << "Número de aristas del grafo: " << num_aristas_ << std::endl;
  archivo_salida << "Vértice origen: " << origen << std::endl;
  archivo_salida << "Vértice destino: " << destino << std::endl;
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}