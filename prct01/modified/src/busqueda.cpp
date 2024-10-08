#include "../include/busqueda.h"

/**
 * @brief Función para mostrar la información de los nodos generados.
 * 
 * @param nodos_generados 
 * @param archivo_salida 
 */
void InformacionNodosGenerados(const std::vector<Nodo>& nodos_generados, std::ofstream& archivo_salida) {
  archivo_salida << "Nodos Generados: ";
  for (size_t i = 0; i < nodos_generados.size(); ++i) {
    archivo_salida << nodos_generados[i].GetID() + 1;  // Mostrar nodos indexados a partir de 1.
    if (i != nodos_generados.size() - 1) {  // Compara el índice, no el valor del nodo.
      archivo_salida << ", ";
    }
  }
  archivo_salida << std::endl;
}

/**
 * @brief Función para mostrar la información de los nodos inspeccionados.
 * 
 * @param nodos_inspeccionados 
 * @param archivo_salida 
 */
void InformacionNodosInspeccionados(const std::vector<Nodo>& nodos_inspeccionados, std::ofstream& archivo_salida) {
  archivo_salida << "Nodos Inspeccionados: ";
  if (nodos_inspeccionados.size() >= 1) {
    for (unsigned i = 0; i < nodos_inspeccionados.size(); ++i) {
      archivo_salida << nodos_inspeccionados[i].GetID() + 1;  // Mostrar nodos indexados a partir de 1.
      if (i < nodos_inspeccionados.size() - 1) {
        archivo_salida << ", ";
      }
    }
  } else {
    archivo_salida << "-";
  }
  archivo_salida << std::endl;
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}

/**
 * @brief Función para comprobar si un nodo está en la rama.
 * Esto quiere decir que si un nodo ya ha sido visitado, no se vuelve a visitar.
 * 
 * @param recorrido 
 * @param id 
 * @param pos_actual 
 * @return true 
 * @return false 
 */
bool NodoEnLaRama(const std::vector<Nodo>& recorrido, const int& id, int pos_actual)
{
	while (pos_actual != -1)
	{
		if (recorrido[pos_actual].GetID() == id) { return true; }
		pos_actual = recorrido[pos_actual].GetPosPadre();
	}
	return false;
}

/**
 * @brief Función para realizar la búsqueda en amplitud.
 * 
 * @param grafo 
 * @param id_inicial 
 * @param id_final 
 * @param costo_total 
 * @return std::vector<int> El camino encontrado
 */
std::vector<Nodo> Busqueda::BusquedaEnAmplitud(const Grafo& grafo, const int& id_inicial, const int& id_final, int& costo_total, std::ofstream& fichero_salida)
{
	Nodo nodo_inicial;
	nodo_inicial.SetID(id_inicial);
	nodo_inicial.SetPosPadre(-1);		// No tiene padre al ser el nodo inicial.
	nodo_inicial.SetCoste(0);				// El coste para llegar hasta él mismo es 0.
	nodo_inicial.SetPadre(nullptr); // No tiene padre, es la raíz.

	std::vector<Nodo> recorrido;
	std::vector<Nodo> inspeccionados; // Nodos inspeccionados
	std::vector<Nodo> generados;			 // Nodos generados
	std::vector<Nodo> camino;         // El camino que se devolverá

	recorrido.push_back(nodo_inicial);
	generados.push_back(nodo_inicial);

	bool final = false, hijo_anadido = false;
	unsigned ultimo_anadido = 0, pos_id_final = 0, aux = ultimo_anadido;
	int iteracion = 1; // Contador de iteraciones

  // Mostramos la iteración
  fichero_salida << "Iteración " << iteracion++ << std::endl;
	InformacionNodosGenerados(generados, fichero_salida);
	InformacionNodosInspeccionados(inspeccionados, fichero_salida);

  // Mientras no se haya recorrido todo el grafo y no se haya encontrado el nodo final
	while (recorrido.size() != static_cast<unsigned>(grafo.GetNumeroVertices()) && !final)
	{
    // Variables auxiliares para actualizar el recorrido
		hijo_anadido = false;
		aux = ultimo_anadido;
		long unsigned kTamanyo{recorrido.size()};

		// Bucle para recorrer los nodos generados, a partir del último nodo añadido hasta el final
		for (unsigned i = aux; i < kTamanyo; ++i)
		{
      // Mostramos la iteración
			fichero_salida << "Iteración " << iteracion << std::endl;

			// En caso de que sea el nodo final, terminamos
			if (recorrido[i].GetID() == id_final)
			{
				final = true;
				pos_id_final = i;

        // Imprimir nodos generados
				InformacionNodosGenerados(generados, fichero_salida);
				inspeccionados.push_back(recorrido[i]);
        // Imprimir nodos inspeccionados
				InformacionNodosInspeccionados(inspeccionados, fichero_salida);
				break;
			}

			// Si no era el nodo final, generamos los hijos que no estén en la rama.
			const std::vector<int>& vecinos = grafo.GetMatrizCoste()[recorrido[i].GetID()];

      // Bucle para recorrer los vecinos del nodo actual
			for (unsigned j = 0; j < vecinos.size(); ++j)
			{
        // Si hay conexión
				if (vecinos[j] != -1)
				{
					Nodo nuevo_nodo;
					nuevo_nodo.SetID(j); // ID del nodo hijo
            
          // Si el nodo no está en la rama
					if (!NodoEnLaRama(recorrido, nuevo_nodo.GetID(), i))
					{
						nuevo_nodo.SetPosPadre(i);					// Almacenamos en qué posición está el padre.
						nuevo_nodo.SetCoste(vecinos[j]);		// Coste de la conexión
						nuevo_nodo.SetPadre(&recorrido[i]); // El padre es el nodo actual

						recorrido.push_back(nuevo_nodo);
						generados.push_back(nuevo_nodo); // Añadimos el nodo generado
						hijo_anadido = true;
					}
				}
			}

      // Después de generar los hijos, mostramos la información de los nodos generados e inspeccionados
			InformacionNodosGenerados(generados, fichero_salida);
			inspeccionados.push_back(recorrido[i]); // Añadimos el nodo inspeccionado
			++ultimo_anadido;
      // Imprimir nodos inspeccionados
			InformacionNodosInspeccionados(inspeccionados, fichero_salida);
			++iteracion; // Incrementamos el contador de iteraciones
		}

		if (final) { break; } // Si ya hemos encontrado el nodo final, salimos del bucle
		if (!hijo_anadido) { break; } // No hay solución, no existe camino
	}

  // Si se ha encontrado el nodo final, se recupera el camino
	if (final)
	{
    // La posición actual es la del nodo final
		int pos_actual = pos_id_final;
    // Mientras no se llegue al nodo inicial
		while (pos_actual != -1)
		{
      // Añadimos el nodo al camino
			camino.push_back(recorrido[pos_actual]);
      // Añadimos el coste del nodo al coste total
			costo_total += recorrido[pos_actual].GetCoste();
      // Actualizamos la posición actual al nodo padre
			pos_actual = recorrido[pos_actual].GetPosPadre();
		}
		// Invertimos el camino para que vaya desde el origen al destino
		std::reverse(camino.begin(), camino.end());
	}
	return camino;
}

/**
 * @brief Función para realizar la búsqueda en profundidad.
 * 
 * @param grafo 
 * @param nodo_origen 
 * @param nodo_destino 
 * @param costo_total 
 * @param file 
 * @return std::vector<Nodo> 
 */
std::vector<Nodo> Busqueda::BusquedaEnProfundidad(const Grafo& grafo, const int& id_origen, const int& id_destino, int& costo_total, std::ofstream& archivo_salida) {
  // Creamos una pila para gestionar la búsqueda
  std::stack<Nodo*> pila;

  // Mapa para saber si un nodo ya fue visitado
  std::unordered_map<int, bool> visitado; // Para saber si un nodo ya fue visitado

  // Vectores para mostrar los nodos inspeccionados y generados
  std::vector<Nodo> inspeccionados;
  std::vector<Nodo> generados;

  // Inicializamos el nodo inicial
  Nodo nodo_inicial;
  nodo_inicial.SetID(id_origen);   // ID del nodo
  nodo_inicial.SetCoste(0);        // Coste de la arista hacia este nodo
  nodo_inicial.SetPadre(nullptr);  // El nodo inicial no tiene padre
  visitado[id_origen] = true;

  // Insertamos el nodo inicial en la pila
  pila.push(&nodo_inicial);
  visitado[id_origen] = true;
  generados.push_back(nodo_inicial);

  // Mostramos la iteración actual
  int iteracion = 1;
  archivo_salida << "Iteración " << iteracion++ << std::endl;
  InformacionNodosGenerados(generados, archivo_salida);
  InformacionNodosInspeccionados(inspeccionados, archivo_salida);

  // Mientras la pila no esté vacía
  while (!pila.empty()) {
    // Obtenemos el nodo actual
    Nodo* nodo_actual = pila.top();
    pila.pop();
    inspeccionados.push_back(*nodo_actual);  // Añadimos el nodo a los inspeccionados

    // Si llegamos al nodo destino, terminamos la búsqueda
    if (nodo_actual->GetID() == id_destino) {
      // Creamos un vector para almacenar el camino
      std::vector<Nodo> camino;
      Nodo* nodo_camino = nodo_actual;

      // Reconstruimos el camino hacia atrás
      while (nodo_camino != nullptr) {
        camino.push_back(*nodo_camino);
        costo_total += nodo_camino->GetCoste();
        nodo_camino = nodo_camino->GetPadre();
      }

      // Mostramos la iteración actual
      archivo_salida << "Iteración " << iteracion++ << std::endl;
      InformacionNodosGenerados(generados, archivo_salida);
      InformacionNodosInspeccionados(inspeccionados, archivo_salida);

      // Invertimos el camino para que vaya desde el origen hasta el destino
      std::reverse(camino.begin(), camino.end());
      return camino;  // Devolvemos el camino encontrado
    }

    // En caso contrario, generamos los hijos del nodo actual
    const std::vector<int>& vecinos = grafo.GetMatrizCoste()[nodo_actual->GetID()];

    // Iteramos sobre los vecinos
    for (int i = vecinos.size() - 1; i >= 0; --i) {
      int costo = vecinos[i];  // Costo de la arista hacia el vecino
      // Si hay conexión y no hemos visitado el nodo
      if (costo != -1 && !visitado[i]) {
        // Creamos un nuevo nodo para el vecino
        Nodo* nuevo_nodo = new Nodo();
        nuevo_nodo->SetID(i);
        nuevo_nodo->SetCoste(costo);
        nuevo_nodo->SetPadre(nodo_actual);

        // Marcamos como visitado y lo añadimos a la pila
        visitado[i] = true;
        pila.push(nuevo_nodo);
        generados.push_back(*nuevo_nodo);
      }
    }
    // Mostramos la iteración actual
    archivo_salida << "Iteración " << iteracion++ << std::endl;
    InformacionNodosGenerados(generados, archivo_salida);
    InformacionNodosInspeccionados(inspeccionados, archivo_salida);
  }
  return {};  // Si no encontramos un camino, devolvemos un vector vacío
}