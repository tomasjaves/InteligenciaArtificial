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
	std::vector<Nodo> generados;			// Nodos generados
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
std::vector<Nodo> Busqueda::BusquedaEnProfundidad(const Grafo& grafo, const int& id_inicial, const int& id_final, int& costo_total, std::ofstream& archivo_salida) {
  // Estructura para saber si un nodo ha sido visitado
  std::unordered_map<int, bool> visitado;

  // Vectores para “traza” (similar a tu versión iterativa)
  std::vector<Nodo> generados;
  std::vector<Nodo> inspeccionados;

  // Vector para guardar el camino actual en la recursión
  // (lo iremos construyendo a medida que descendemos)
  std::vector<Nodo> camino_actual;

  // Creamos el nodo inicial (similar a tu versión iterativa)
  Nodo nodo_inicial;
  nodo_inicial.SetID(id_inicial);
  nodo_inicial.SetCoste(0);
  nodo_inicial.SetPadre(nullptr);

  // Marcamos como visitado
  visitado[id_inicial] = true;
  generados.push_back(nodo_inicial);

  // Iteración para la traza
  int iteracion = 1;

  // Escribimos la primera traza (antes de entrar en DFSRecursivo)
  archivo_salida << "Iteración " << iteracion++ << std::endl;
  InformacionNodosGenerados(generados, archivo_salida);
  InformacionNodosInspeccionados(inspeccionados, archivo_salida);

  // Variable booleana para indicar si se ha encontrado el destino
  bool encontrado = false;

  // Iniciamos la búsqueda recursiva
  DFSRecursivo(grafo, &nodo_inicial, id_final,
               visitado, generados, inspeccionados, camino_actual,
               costo_total, archivo_salida, iteracion, encontrado);

  // Si al terminar la recursión no se encontró, camino_actual estará vacío
  return camino_actual;  
}

void Busqueda::DFSRecursivo(const Grafo& grafo,
                            Nodo* nodo_actual,
                            int id_destino,
                            std::unordered_map<int, bool>& visitado,
                            std::vector<Nodo>& generados,
                            std::vector<Nodo>& inspeccionados,
                            std::vector<Nodo>& camino_actual,
                            int& costo_total,
                            std::ofstream& archivo_salida,
                            int& iteracion,
                            bool& encontrado) {
  // Marcamos el nodo actual como visitado
  visitado[nodo_actual->GetID()] = true;
  // Añadimos el nodo actual a “inspeccionados”
  inspeccionados.push_back(*nodo_actual);

  // Añadimos también el nodo al “camino actual”
  camino_actual.push_back(*nodo_actual);

  // Comprobamos si es el destino
  if (nodo_actual->GetID() == id_destino) {
    // Mostramos la traza final
    archivo_salida << "Iteración " << iteracion++ << std::endl;
    InformacionNodosGenerados(generados, archivo_salida);
    InformacionNodosInspeccionados(inspeccionados, archivo_salida);

    // Reconstruimos el coste total (sumando coste de cada nodo en el camino)
    costo_total = 0;
    for (auto& nd : camino_actual) {
      costo_total += nd.GetCoste();
    }

    // Marcamos que el destino fue encontrado
    encontrado = true;
    return;
  }

  // Exploramos sus vecinos
  const std::vector<int>& fila = grafo.GetMatrizCoste()[nodo_actual->GetID()];

  // Recorremos los vecinos
  for (int i = 0; i < static_cast<int>(fila.size()); ++i) {
    int costo = fila[i];
    // Si hay conexión y no está visitado
    if (costo != -1 && !visitado[i]) {
      // Creamos un nuevo nodo
      Nodo* nuevo_hijo = new Nodo();
      nuevo_hijo->SetID(i);
      nuevo_hijo->SetCoste(costo);
      nuevo_hijo->SetPadre(nodo_actual);

      generados.push_back(*nuevo_hijo);
      archivo_salida << "Iteración " << iteracion++ << std::endl;
      InformacionNodosGenerados(generados, archivo_salida);
      InformacionNodosInspeccionados(inspeccionados, archivo_salida);

      // Llamada recursiva
      DFSRecursivo(grafo, nuevo_hijo, id_destino,
                   visitado, generados, inspeccionados, camino_actual,
                   costo_total, archivo_salida, iteracion, encontrado);

      // Si el destino se encontró en la rama recursiva, no seguimos
      if (encontrado) {
        return;
      }
    }
  }

  // 5) Si llegamos aquí, significa que este nodo no ha llevado al destino:
  //    lo quitamos del “camino actual” (backtracking)
  camino_actual.pop_back();
}

// MODIFICACION

/**
 * @brief Función para realizar la búsqueda en amplitud modificada.
 * 
 * @param grafo 
 * @param id_inicial 
 * @param id_final 
 * @param costo_total 
 * @param fichero_salida
 * @return std::vector<int> El camino encontrado
 */
std::vector<Nodo> Busqueda::BusquedaEnAmplitudModificada(const Grafo& grafo,
                                              const int& id_inicial,
                                              const int& id_final,
                                              int& costo_total,
                                              std::ofstream& fichero_salida)
{
  // Semilla para aleatoriedad (puede ajustarse según se necesite,
  // o simplemente usar el random_device).
  static std::mt19937 generador(std::random_device{}());

  // 1) Creamos el nodo inicial
  Nodo nodo_inicial;
  nodo_inicial.SetID(id_inicial);
  nodo_inicial.SetPosPadre(-1);    // No tiene padre (es la raíz)
  nodo_inicial.SetCoste(0);        // Coste hasta sí mismo = 0
  nodo_inicial.SetPadre(nullptr);

  // Estructuras auxiliares para la traza
  std::vector<Nodo> generados;         // Nodos generados
  std::vector<Nodo> inspeccionados;    // Nodos que hemos “expandido”
  generados.push_back(nodo_inicial);

  // En la búsqueda tradicional, usaríamos una cola (FIFO).
  // En esta versión, usaremos un “contenedor de frontera” (vector)
  // que iremos filtrando por el 50% con menor coste acumulado.
  std::vector<Nodo> frontera;
  frontera.push_back(nodo_inicial);

  // Estructura donde iremos guardando todos los nodos para poder
  // reconstruir la ruta al final (igual que en BFS normal).
  std::vector<Nodo> recorrido;  // “recorrido” es la memoria global de nodos
  recorrido.push_back(nodo_inicial);

  // Variables para controlar la traza e iteraciones
  int iteracion = 1;
  bool encontrado = false;
  int posicion_destino = -1;

  fichero_salida << "Iteración " << iteracion++ << std::endl;
  InformacionNodosGenerados(generados, fichero_salida);
  InformacionNodosInspeccionados(inspeccionados, fichero_salida);

  // Bucle principal: mientras haya nodos en la frontera
  while (!frontera.empty()) {
    // Si sólo hay un nodo en frontera , lo inspeccionamos directamente.
    Nodo nodo_actual;
    if (frontera.size() == 1) {
      nodo_actual = frontera.front();
    } 
    else {
      // Si hay más de un nodo, elegimos el 50% con menor coste.

      // Ordenar la frontera por coste acumulado (menor a mayor)
      std::sort(frontera.begin(), frontera.end(), [](const Nodo& a, const Nodo& b) { // comparador “lambda” inline
        return a.GetCoste() < b.GetCoste();
      });

      // Calcular hasta dónde tomar el “mejor 50%”.
      size_t mitad = frontera.size() / 2; // división entera
      if (mitad == 0) mitad = 1;         // asegurar al menos 1

      // Generar un índice aleatorio entre [0, mitad - 1]
      std::uniform_int_distribution<size_t> dist(0, mitad - 1);
      size_t idx_aleatorio = dist(generador);

      // Elegir ese nodo
      nodo_actual = frontera[idx_aleatorio];
    }

    // Retirarlo de la frontera
    auto it = std::find_if(frontera.begin(), frontera.end(), [&](const Nodo& nd){
      return nd.GetID() == nodo_actual.GetID() && nd.GetPosPadre() == nodo_actual.GetPosPadre();
    });
    if (it != frontera.end()) frontera.erase(it);

    // Lo anotamos en inspeccionados
    inspeccionados.push_back(nodo_actual);

    // Comprobamos si es el destino
    if (nodo_actual.GetID() == id_final) {
      // Marcamos encontrado y reservamos su posición en “recorrido”
      encontrado = true;

      // Encontrar la posición en ‘recorrido’ de este nodo para luego
      // reconstruir el camino
      for (size_t i = 0; i < recorrido.size(); ++i) {
        if (recorrido[i].GetID() == nodo_actual.GetID() &&
            recorrido[i].GetPosPadre() == nodo_actual.GetPosPadre()) {
          posicion_destino = i;
          break;
        }
      }

      // Imprimimos la traza final y rompemos
      fichero_salida << "Iteración " << iteracion++ << std::endl;
      InformacionNodosGenerados(generados, fichero_salida);
      InformacionNodosInspeccionados(inspeccionados, fichero_salida);
      break;
    }

    // Expandir al nodo actual generando sus hijos
    const auto& vecinos = grafo.GetMatrizCoste()[nodo_actual.GetID()];
    for (int i = 0; i < (int)vecinos.size(); ++i) {
      if (vecinos[i] != -1 && i != nodo_actual.GetID()) {
        // Crear el nuevo nodo (hijo)
        Nodo nuevo_nodo;
        nuevo_nodo.SetID(i);

        // El coste acumulado es el coste del padre + el coste de la arista
        int nuevo_coste = nodo_actual.GetCoste() + vecinos[i];
        nuevo_nodo.SetCoste(nuevo_coste);

        // Guardamos la traza del padre
        int pos_padre = -1;
        for (size_t idx = 0; idx < recorrido.size(); ++idx) {
          if (recorrido[idx].GetID() == nodo_actual.GetID() &&
              recorrido[idx].GetPosPadre() == nodo_actual.GetPosPadre()) {
            pos_padre = (int)idx;
            break;
          }
        }
        nuevo_nodo.SetPosPadre(pos_padre);
        nuevo_nodo.SetPadre(nullptr); // para la reconstrucción

        // Antes de añadir a la frontera, comprobamos si YA estaba en 'recorrido'
        // con un coste menor o igual; si es así, no generamos duplicado.
        bool mejor_o_igual_en_recorrido = false;
        for (auto& r : recorrido) {
          if (r.GetID() == nuevo_nodo.GetID()) {
            // Si ya está en recorrido con coste menor o igual, no lo insertamos
            if (r.GetCoste() <= nuevo_nodo.GetCoste()) {
              mejor_o_igual_en_recorrido = true;
              break;
            }
          }
        }

        // Si no se ha encontrado uno mejor en recorrido, lo añadimos
        if (!mejor_o_igual_en_recorrido) {
          frontera.push_back(nuevo_nodo);
          generados.push_back(nuevo_nodo);
          recorrido.push_back(nuevo_nodo);
        }
      }
    }

    // Imprimimos la traza tras esta expansión
    fichero_salida << "Iteración " << iteracion++ << std::endl;
    InformacionNodosGenerados(generados, fichero_salida);
    InformacionNodosInspeccionados(inspeccionados, fichero_salida);
  } // fin while(!frontera.empty())

  // Si hemos salido del bucle y el destino fue encontrado, reconstruimos la ruta
  std::vector<Nodo> ruta;
  costo_total = 0;

  if (encontrado && posicion_destino != -1) {
    int pos_actual = posicion_destino;
    // Reconstruimos y acumulamos coste
    while (pos_actual != -1) {
      ruta.push_back(recorrido[pos_actual]);
      costo_total += recorrido[pos_actual].GetCoste();
      pos_actual = recorrido[pos_actual].GetPosPadre();
    }
    costo_total = ruta.front().GetCoste(); 
    std::reverse(ruta.begin(), ruta.end());
  }

  return ruta;
}
