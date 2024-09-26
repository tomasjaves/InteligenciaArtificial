#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

/**
 * @brief Clase que representa un grafo no dirigido mediante una matriz de
 * adyacencia.
 */
class Grafo {
 public:
  Grafo(int);
  void CargarDesdeArchivo(const std::string&);

  void AgregarArista(int, int, double);

  int ObtenerNumeroAristas() const;
  int ObtenerNumeroVertices() const;
  const std::vector<std::vector<double>>& ObtenerMatrizCoste() const;
    
  void MostrarGrafo() const;
  void MostrarInformacion(const int&, const int&, std::ofstream&) const;

 private:
  int num_vertices_;  // Número de vértices en el grafo.
  int num_aristas_;  // Número de aristas en el grafo.
  std::vector<std::vector<double>> matriz_adyacencia_;  // Matriz de adyacencia.
};
