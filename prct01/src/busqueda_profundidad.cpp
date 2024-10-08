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
bool DFSRecursivo(const Grafo& grafo, const int& nodo_actual, const int& destino,
                  std::unordered_map<int, bool>& visitado, std::vector<int>& camino, int& costo_actual,
                  int& costo_total, std::unordered_map<int, int>& distancia,
                  std::unordered_map<int, int>& padre, std::vector<int>& nodos_generados, 
                  std::vector<int>& nodos_inspeccionados, int& iteracion, std::ofstream& archivo_salida) {
  // Marcamos el nodo actual como visitado.
  visitado[nodo_actual] = true;
  camino.push_back(nodo_actual);

  // Mostramos información de la iteración en el archivo de salida.
  archivo_salida << "Iteración " << iteracion++ << std::endl;
  InformacionNodosGenerados(nodos_generados, archivo_salida);
  InformacionNodosInspeccionados(nodos_inspeccionados, archivo_salida);

  // Añadimos el nodo actual a los nodos inspeccionados.
  nodos_inspeccionados.push_back(nodo_actual);

  // Si llegamos al destino, retornamos verdadero.
  if (nodo_actual == destino) {
    costo_total = costo_actual;
    return true;
  }

  // Obtenemos los vecinos del nodo actual.
  const std::vector<int>& vecinos = grafo.GetMatrizCoste()[nodo_actual];
  // Iteramos sobre los vecinos.
  for (unsigned i = 0; i < vecinos.size(); ++i) {
    // Obtenemos el costo de ir al vecino.
    int costo = vecinos[i];
    // Si hay conexión y no ha sido visitado.
    if (costo != -1 && !visitado[i]) {
      costo_actual += costo;
      padre[i] = nodo_actual;
      InsertarMenorAMayor(nodos_generados, i);  // Registramos nodo generado.
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
std::vector<int> BusquedaEnProfundidad(const Grafo& grafo, int origen, int destino, int& costo_total, std::ofstream& archivo_salida) {
  // Creamos un vector para almacenar el camino.
  std::vector<int> camino;

  // Si el origen y el destino son iguales, devolvemos el origen.
  if (origen == destino) {
    camino.push_back(origen);
    costo_total = 0;
    return camino;
  }

  // Estrucutras para manejar la búsqueda.
  std::unordered_map<int, bool> visitado; // Para saber si un nodo ya fue visitado. Clave: nodo, Valor: visitado.
  std::unordered_map<int, int> distancia;  // Para mantener la distancia acumulada. Clave: nodo, Valor: distancia.
  std::unordered_map<int, int> padre;  // Para rastrear el camino. Clave: nodo, Valor: padre.
  int costo_actual = 0; // Costo actual del camino.

  visitado[origen] = true;
  distancia[origen] = 0;
  padre[origen] = -1;  // El nodo inicial no tiene padre.

  std::vector<int> nodos_generados;  // Para almacenar los nodos generados en cada iteración.
  std::vector<int> nodos_inspeccionados;  // Para almacenar los nodos inspeccionados en cada iteración.
  int iteracion = 0;  // Contador de iteraciones.

  // El nodo origen se agrega a los nodos generados.
  nodos_generados.push_back(origen);

  // Llamar a la función auxiliar para iniciar el DFS desde el nodo origen.
  if (DFSRecursivo(grafo, origen, destino, visitado, camino, costo_actual, costo_total, distancia, padre, nodos_generados, nodos_inspeccionados, iteracion, archivo_salida)) {
    return camino;  // Si encontramos el camino, retornamos el camino.
  } else {
    costo_total = std::numeric_limits<int>::infinity();
    return {};  // Si no encontramos el camino, retornamos un vector vacío.
  }
}
