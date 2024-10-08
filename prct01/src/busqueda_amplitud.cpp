#include "../include/busqueda_amplitud.h"

// Función auxiliar que verifica si un nodo ya está en la rama de inspección
bool NodoEnLaRama(const std::vector<Nodo>& recorrido, int id, int pos_actual) {
	while (pos_actual != -1) {
	  if (recorrido[pos_actual].GetID() == id) {
	    return true;
	  }
	  pos_actual = recorrido[pos_actual].GetPosPadre();
	}
	return false;
}

// Función para mostrar el recorrido en amplitud entre dos nodos
void BusquedaEnAmplitud(const Grafo& grafo, const int& id_inicial, const int& id_final, int& costo_total, std::ofstream& fichero_salida) {
    Nodo nodo_inicial;
    nodo_inicial.SetID(id_inicial);
    nodo_inicial.SetPosPadre(-1);  // No tiene padre al ser el nodo inicial.
    nodo_inicial.SetCoste(0);      // El coste para llegar hasta él mismo es 0.
    nodo_inicial.SetPadre(nullptr);  // No tiene padre, es la raíz.

    std::vector<Nodo> recorrido;
    std::vector<int> inspeccionados;  // Nodos inspeccionados
    std::vector<int> generados;       // Nodos generados

    recorrido.push_back(nodo_inicial);
    generados.push_back(nodo_inicial.GetID());

    bool final{false};
    unsigned ultimo_anadido{0};
    unsigned pos_id_final{0};
    unsigned aux{ultimo_anadido};
    bool hijo_anadido{false};
    int iteracion = 1;  // Contador de iteraciones

    fichero_salida << "Iteración " << iteracion++ << std::endl;
		InformacionNodosGenerados(generados, fichero_salida);
		InformacionNodosInspeccionados(inspeccionados, fichero_salida);

    while (recorrido.size() != static_cast<long unsigned int>(grafo.GetNumeroVertices()) && !final) {
        hijo_anadido = false;
        aux = ultimo_anadido;
        long unsigned int kTamanyo{recorrido.size()};

        // Añadimos los hijos del último nivel inspeccionado
        for (unsigned i{aux}; i < kTamanyo; ++i) {
            // Mostramos la iteración
            fichero_salida << "Iteración " << iteracion << std::endl;

            // Si el nodo actual no es el final, generamos los hijos. En caso de que sea el nodo final, terminamos
            if (recorrido[i].GetID() == id_final) {
                final = true;
                pos_id_final = i;

                // Imprimir nodos generados
                InformacionNodosGenerados(generados, fichero_salida);

                inspeccionados.push_back(recorrido[i].GetID());

                // Imprimir nodos inspeccionados
                InformacionNodosInspeccionados(inspeccionados, fichero_salida);

                break;
            }

            // Si no era el nodo final, generamos los hijos que no estén en la rama.
            const std::vector<int>& vecinos = grafo.GetMatrizCoste()[recorrido[i].GetID()];
            for (unsigned j{0}; j < vecinos.size(); ++j) {
                if (vecinos[j] != -1) {  // Hay conexión
                    Nodo nuevo_nodo;
                    nuevo_nodo.SetID(j);  // ID del nodo hijo

                    if (!NodoEnLaRama(recorrido, nuevo_nodo.GetID(), i)) {
                        nuevo_nodo.SetPosPadre(i);  // Almacenamos en qué posición está el padre.
                        nuevo_nodo.SetCoste(vecinos[j]);  // Coste de la conexión
                        nuevo_nodo.SetPadre(&recorrido[i]);  // El padre es el nodo actual
                        nuevo_nodo.SetProfundidad(recorrido[i].GetProfundidad() + 1);

                        recorrido.push_back(nuevo_nodo);
                        generados.push_back(nuevo_nodo.GetID());  // Añadimos el nodo generado
                        hijo_anadido = true;
                    }
                }
            }

            // Imprimir nodos generados
            InformacionNodosGenerados(generados, fichero_salida);

            if (final) {
                break;
            }

            inspeccionados.push_back(recorrido[i].GetID());  // Añadimos el nodo inspeccionado
            ++ultimo_anadido;

            // Imprimir nodos inspeccionados
            InformacionNodosInspeccionados(inspeccionados, fichero_salida);
            ++iteracion;  // Incrementamos el contador de iteraciones
        }

        if (final) {
            break;
        }
        if (!hijo_anadido) {
            break;  // No hay solución, no existe camino
        }
    }

    if (final) {
        fichero_salida << "Camino: ";
        int pos_actual = pos_id_final;
        std::vector<int> camino;
        while (pos_actual != -1) {
            camino.push_back(recorrido[pos_actual].GetID());
						costo_total += recorrido[pos_actual].GetCoste();
            pos_actual = recorrido[pos_actual].GetPosPadre();
        }

        for (int i = camino.size() - 1; i >= 0; --i) {
            fichero_salida << camino[i] + 1 << " - ";
        }
        fichero_salida << std::endl;
				fichero_salida << "Costo: " << costo_total << std::endl;
    } else {
        fichero_salida << "No se ha encontrado un camino" << std::endl;
    }
}
