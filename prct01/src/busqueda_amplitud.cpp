#include "../include/busqueda_amplitud.h"

/**
 * @brief Función que implementa la búsqueda en amplitud (BFS) con salida a archivo.
 * 
 * @param grafo 
 * @param origen 
 * @param destino 
 * @param costo_total 
 * @param archivo_salida 
 * @return std::vector<int> 
 */
std::vector<int> BusquedaEnAmplitud(const Grafo& grafo, const int& origen, const int& destino, int& costo_total, std::ofstream& archivo_salida) {
  // Creamos un vector para almacenar el camino.
  std::vector<int> camino;

  // Si el origen y el destino son iguales, devolvemos el origen.
  if (origen == destino) {
    camino.push_back(origen);
    costo_total = 0;
    return camino;
  }

  // Estructuras para manejar la búsqueda.
  std::queue<int> cola;  // Cola para manejar BFS.
  std::unordered_map<int, bool> visitado;  // Para saber si un nodo ya fue visitado. Clave: nodo, Valor: visitado.
  std::unordered_map<int, int> distancia;  // Para mantener la distancia mínima. Clave: nodo, Valor: distancia.
  std::unordered_map<int, int> padre;  // Para rastrear el camino. Clave: nodo, Valor: padre.

  // Inicializamos las estructuras.
  cola.push(origen);
  visitado[origen] = true;
  distancia[origen] = 0;
  padre[origen] = -1;  // El nodo inicial no tiene padre.

  // Estructuras para almacenar nodos generados e inspeccionados en cada iteración.
  std::vector<int> nodos_generados;  // Nodos generados.
  std::vector<int> nodos_inspeccionados;  // Nodos inspeccionados.
  int iteracion = 0;  // Contador de iteraciones.

  // El nodo origen es generado al principio.
  nodos_generados.push_back(origen);

  // Realizamos la búsqueda en amplitud.
  while (!cola.empty()) {
    int nodo_actual = cola.front();
    cola.pop();

    // Mostrar información de la iteración.
    InformacionIteracion(iteracion, nodos_generados, nodos_inspeccionados, archivo_salida);

    // Agregamos el nodo actual a los nodos inspeccionados.
    nodos_inspeccionados.push_back(nodo_actual);

    // Obtenemos los vecinos del nodo actual.
    const std::vector<int>& vecinos = grafo.ObtenerMatrizCoste()[nodo_actual];
    
    // Iteramos sobre los vecinos.
    for (unsigned i = 0; i < vecinos.size(); ++i) {
      int costo = vecinos[i];  // Almacenamos el costo de ir del nodo actual al vecino i.

      // Si hay conexión y no ha sido visitado.
      if (costo != -1 && !visitado[i]) {
        // Agregamos el vecino a la cola y actualizamos las estructuras.
        cola.push(i);
        visitado[i] = true;
        padre[i] = nodo_actual;
        distancia[i] = distancia[nodo_actual] + costo;

        // Agregamos el vecino a los nodos generados.
        InsertarMenorAMayor(nodos_generados, i);

        // Si hemos encontrado el destino.
        if (i == unsigned(destino)) {
          // Mostramos información adicional antes de terminar.
          InformacionIteracion(iteracion, nodos_generados, nodos_inspeccionados, archivo_salida);

          // El costo total es la distancia acumulada al nodo destino.
          costo_total = distancia[i];

          // Reconstruimos el camino desde el destino al origen.
          int nodo = destino;
          // Mientras no lleguemos al origen (nodo raíz)
          while (nodo != -1) {
            // Agregamos el nodo al camino y actualizamos el nodo actual al padre y repetimos.
            camino.push_back(nodo);
            nodo = padre[nodo];
          }
          // Invertimos el camino para que vaya desde el origen al destino.
          std::reverse(camino.begin(), camino.end());
          return camino;
        }
      }
    }
  }

  // Si no encontramos un camino, devolvemos un vector vacío y un costo de infinito.
  costo_total = std::numeric_limits<int>::infinity();
  return {};
}