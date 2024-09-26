#include "../include/busqueda_profundidad.h"

/**
 * @brief Función auxiliar para realizar la búsqueda en profundidad (DFS) de forma recursiva.
 * 
 * @param grafo 
 * @param nodo_actual 
 * @param destino 
 * @param visitado 
 * @param camino 
 * @param costo_actual 
 * @param costo_total 
 * @param distancia 
 * @param padre 
 * @param nodos_generados 
 * @param nodos_inspeccionados 
 * @param iteracion 
 * @return true 
 * @return false 
 */
bool DFSRecursivo(const Grafo& grafo, int nodo_actual, int destino, std::unordered_map<int, bool>& visitado, 
                  std::vector<int>& camino, double& costo_actual, double& costo_total,
                  std::unordered_map<int, double>& distancia, std::unordered_map<int, int>& padre,
                  std::vector<int>& nodos_generados, std::vector<int>& nodos_inspeccionados, int& iteracion,
                  std::ofstream& archivo_salida) {
  // Marcamos el nodo actual como visitado.
  visitado[nodo_actual] = true;
  camino.push_back(nodo_actual);

  // Mostramos información de la iteración en el archivo de salida.
  ++iteracion;
  archivo_salida << "Iteración " << iteracion << std::endl;
  archivo_salida << "Nodos Generados: ";
  for (int nodo : nodos_generados) {
    archivo_salida << nodo + 1;  // Para mostrar como nodos 1-indexados.
    if (nodo != nodos_generados.back()) {
      archivo_salida << ", ";
    }
  }
  archivo_salida << "\nNodos Inspeccionados: ";
  if (nodos_inspeccionados.size() >= 1) {
    for (unsigned i = 0; i < nodos_inspeccionados.size(); ++i) {
      archivo_salida << nodos_inspeccionados[i] + 1;  // Para mostrar como nodos 1-indexados.
      if (i < nodos_inspeccionados.size() - 1) {
        archivo_salida << ", ";
      }
    }
  } else {
    archivo_salida << "-";
  }
  archivo_salida << std::endl;
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;

  // Añadimos el nodo actual a los nodos inspeccionados.
  nodos_inspeccionados.push_back(nodo_actual);

  // Si llegamos al destino, retornamos verdadero.
  if (nodo_actual == destino) {
    costo_total = costo_actual;
    return true;
  }

  // Obtenemos los vecinos del nodo actual.
  const auto& vecinos = grafo.ObtenerMatrizCoste()[nodo_actual];
  for (unsigned i = 0; i < vecinos.size(); ++i) {
    double costo = vecinos[i];
    // Si hay conexión y no ha sido visitado.
    if (costo != -1 && !visitado[i]) {
      costo_actual += costo;
      padre[i] = nodo_actual;
      nodos_generados.push_back(i);  // Registramos nodo generado.
      if (DFSRecursivo(grafo, i, destino, visitado, camino, costo_actual, costo_total, distancia, padre, nodos_generados, nodos_inspeccionados, iteracion, archivo_salida)) {
        return true;  // Si encontramos el camino, retornamos verdadero.
      }
      costo_actual -= costo;  // Retrocedemos y deshacemos el costo.
    }
  }

  // Si no encontramos el destino, eliminamos el nodo actual del camino y de los inspeccionados.
  camino.pop_back();
  nodos_inspeccionados.pop_back();
  return false;
}

/**
 * @brief Función que implementa la búsqueda en profundidad (DFS).
 * 
 * @param grafo 
 * @param origen 
 * @param destino 
 * @param costo_total 
 * @return std::vector<int> 
 */
std::vector<int> BusquedaEnProfundidad(const Grafo& grafo, int origen, int destino, double& costo_total, std::ofstream& archivo_salida) {
  std::vector<int> camino;
  std::unordered_map<int, bool> visitado;
  std::unordered_map<int, double> distancia;  // Para mantener la distancia acumulada.
  std::unordered_map<int, int> padre;  // Para rastrear el camino.
  double costo_actual = 0;
  
  costo_total = std::numeric_limits<double>::infinity();  // Inicializar el costo total con infinito.

  std::vector<int> nodos_generados;  // Para almacenar los nodos generados en cada iteración.
  std::vector<int> nodos_inspeccionados;  // Para almacenar los nodos inspeccionados en cada iteración.
  int iteracion = 0;  // Contador de iteraciones.

  // El nodo origen se agrega a los nodos generados.
  nodos_generados.push_back(origen);

  // Llamar a la función auxiliar para iniciar el DFS desde el nodo origen.
  if (DFSRecursivo(grafo, origen, destino, visitado, camino, costo_actual, costo_total, distancia, padre, nodos_generados, nodos_inspeccionados, iteracion, archivo_salida)) {
    return camino;  // Si encontramos el camino, retornamos el camino.
  } else {
    return {};  // Si no encontramos el camino, retornamos un vector vacío.
  }
}
