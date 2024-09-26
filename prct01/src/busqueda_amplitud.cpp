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
std::vector<int> BusquedaEnAmplitud(const Grafo& grafo, int origen, int destino, double& costo_total, std::ofstream& archivo_salida) {
  // Creamos un vector para almacenar el camino.
  std::vector<int> camino;

  // Si el origen y el destino son iguales, devolvemos el origen.
  if (origen == destino) {
    camino.push_back(origen);
    costo_total = 0;
    return camino;
  }

  // Estructuras para manejar la búsqueda.
  std::queue<int> cola;  // Cola para manejar la BFS.
  std::unordered_map<int, int> padre;  // Para rastrear el camino.
  std::unordered_map<int, double> distancia;  // Para mantener la distancia mínima.
  std::unordered_map<int, bool> visitado;  // Para saber si un nodo ya fue visitado.

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

  // Realizar la búsqueda en amplitud.
  while (!cola.empty()) {
    int nodo_actual = cola.front();
    cola.pop();

    // Mostrar información de la iteración.
    ++iteracion;
    archivo_salida << "Iteración " << iteracion << std::endl;
    archivo_salida << "Nodos Generados: ";
    for (int nodo : nodos_generados) {
      archivo_salida << nodo + 1;  // Mostrar nodos indexados a partir de 1.
      if (nodo != nodos_generados.back()) {
        archivo_salida << ", ";
      }
    }
    archivo_salida << "\nNodos Inspeccionados: ";
    if (nodos_inspeccionados.size() >= 1) {
      for (unsigned i = 0; i < nodos_inspeccionados.size(); ++i) {
        archivo_salida << nodos_inspeccionados[i] + 1;  // Mostrar nodos indexados a partir de 1.
        if (i < nodos_inspeccionados.size() - 1) {
          archivo_salida << ", ";
        }
      }
    } else {
      archivo_salida << "-";
    }
    archivo_salida << std::endl;
    archivo_salida << "──────────────────────────────────────────────────" << std::endl;

    // Agregamos el nodo actual a los nodos inspeccionados.
    nodos_inspeccionados.push_back(nodo_actual);

    // Obtenemos los vecinos del nodo actual.
    const std::vector<double>& vecinos = grafo.ObtenerMatrizCoste()[nodo_actual];
    
    // Iteramos sobre los vecinos.
    for (unsigned i = 0; i < vecinos.size(); ++i) {
      double costo = vecinos[i];  // Almacenamos el costo de ir del nodo actual al vecino i.

      // Si hay conexión y no ha sido visitado.
      if (costo != -1 && !visitado[i]) {
        // Agregamos el vecino a la cola y actualizamos las estructuras.
        cola.push(i);
        visitado[i] = true;
        padre[i] = nodo_actual;
        distancia[i] = distancia[nodo_actual] + costo;

        // Agregamos el vecino a los nodos generados.
        nodos_generados.push_back(i);

        // Si hemos encontrado el destino.
        if (i == unsigned(destino)) {
          // Mostrar información adicional antes de terminar.
          ++iteracion;
          archivo_salida << "Iteración " << iteracion << std::endl;
          archivo_salida << "Nodos Generados: ";
          for (int nodo : nodos_generados) {
            archivo_salida << nodo + 1;  // Mostrar nodos indexados a partir de 1.
            if (nodo != nodos_generados.back()) {
              archivo_salida << ", ";
            }
          }
          archivo_salida << "\nNodos Inspeccionados: ";
          for (int nodo : nodos_inspeccionados) {
            archivo_salida << nodo + 1;  // Mostrar nodos indexados a partir de 1.
            if (nodo != nodos_inspeccionados.back()) {
              archivo_salida << ", ";
            }
          }
          archivo_salida << std::endl;
          archivo_salida << "──────────────────────────────────────────────────" << std::endl;

          // El costo total es la distancia acumulada al nodo destino.
          costo_total = distancia[i];
          int nodo = destino;

          // Reconstruimos el camino desde el destino al origen.
          while (nodo != -1) {
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
  costo_total = std::numeric_limits<double>::infinity();
  return {};
}