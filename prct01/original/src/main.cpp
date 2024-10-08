#include "../include/grafo.h"
#include "../include/busqueda.h"
#include <iostream>
#include <thread>  // Para usar std::this_thread::sleep_for
#include <chrono>  // Para usar std::chrono::milliseconds
#include <cstdlib>  // Para usar system()
#include <filesystem> // Para usar std::filesystem
#include <vector>
#include <string>

// Definición de colores para la salida en consola.
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define PINK "\033[95m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GRAY "\033[90m"
#define TURQUOISE "\033[36m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

/**
 * @brief Función para mostrar el camino encontrado y el costo total.
 * 
 * @param camino 
 * @param costo_total 
 * @param origen 
 * @param destino 
 * @param archivo_salida 
 */
void MostrarCamino(const std::vector<Nodo>& camino, double costo_total, 
                   int origen, int destino, std::ofstream& archivo_salida) {
  // Mostramos el camino encontrado.
  if (camino.empty()) {
    archivo_salida << "No se encontró un camino entre los vértices " << origen << " y " << destino;
  } else {
    archivo_salida << "Camino: ";
    for (long unsigned i = 0; i < camino.size(); ++i) {
      archivo_salida << camino[i].GetID()+1;
      if (i < camino.size()-1) {
        archivo_salida << " - ";
      }
    }
  }
  archivo_salida << "\n──────────────────────────────────────────────────" << std::endl;
  archivo_salida << "Costo: " << costo_total;
  archivo_salida << "\n──────────────────────────────────────────────────";
}

/**
 * @brief Función para mostrar el menú de opciones.
 * 
 */
void MostrarMenu() {
  std::cout << ITALIC << "Menú de opciones:" << RESET << std::endl;
  std::cout << PINK << BOLD << "1." << RESET << " Buscar camino mediante Amplitud." << std::endl;
  std::cout << PINK << BOLD << "2." << RESET << " Buscar camino mediante Profundidad." << std::endl;
  std::cout << PINK << BOLD << "3." << RESET << " Salir." << std::endl;
  std::cout << UNDERLINE << "Opción:" << RESET << " ";
}

/**
 * @brief Función para mostrar un texto con efecto de escritura.
 * 
 * @param texto 
 * @param delay 
 */
void MostrarConEfecto(const std::string& texto, const int& delay = 10) {
  for (char c : texto) {
    std::cout << c << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}

/**
 * @brief Función para limpiar la pantalla.
 * 
 */
void LimpiarPantalla() {
  #ifdef _WIN32
    system("cls");  // Comando para limpiar pantalla en Windows
  #else
    system("clear");  // Comando para limpiar pantalla en Linux/Unix
  #endif
}

/**
 * @brief Función para listar los archivos en un directorio.
 * 
 * @param path 
 * @return std::vector<std::string> 
 */
std::vector<std::string> ListarArchivos(const std::string& path) {
  std::vector<std::string> archivos;
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      archivos.push_back(entry.path().filename().string());
    }
  }
  return archivos;
}


/**
 * @brief Función principal para la ejecución del programa.
 * 
 * @return int 
 */
int main() {
  LimpiarPantalla();
  std::cout << GRAY << BOLD << "Práctica 1: Búsquedas no informadas" << RESET << std::endl;
  
  // Mostramos el menú de opciones.
  int opcion;
  MostrarMenu();
  std::cin >> opcion;
  std::cout << std::endl;
  if (opcion == 3) {
    std::cout << RED << "Saliendo del programa." << RESET << std::endl;
    return 0;
  } else if (opcion != 1 && opcion != 2) {
    std::cerr << YELLOW << "Opción no válida." << RESET << std::endl;
    return 1;
  }

  // Listamos los archivos en el directorio 'input'.
  std::string directorio = "input";
  std::vector<std::string> archivos = ListarArchivos(directorio);
  // Si no hay archivos en el directorio, mostrar mensaje y salir
  if (archivos.empty()) {
    std::cerr << RED << "No se encontraron archivos en el directorio 'input'." << RESET << std::endl;
    return 1;
  }

  // Mostramos los archivos y permitir al usuario seleccionar uno
  std::cout << ITALIC << "Seleccione un archivo de entrada:" << RESET << std::endl;
  for (size_t i = 0; i < archivos.size(); ++i) {
    std::cout << PINK << BOLD << i + 1 << ". " << RESET << archivos[i] << std::endl;  // Mostrar archivos con número
  }
  // Solicitamos al usuario que ingrese el número del archivo
  int seleccion;
  std::cout << UNDERLINE << "Ingrese el número del archivo a seleccionar:" << RESET << " ";
  std::cin >> seleccion;
  // Validamos la selección
  if (seleccion < 1 || seleccion > static_cast<int>(archivos.size())) {
    std::cerr << RED << "\nSelección inválida." << RESET << std::endl;
    return 1;
  }
  // Obtenemos el nombre del archivo seleccionado
  std::string archivo_seleccionado = archivos[seleccion - 1];
  std::cout << GREEN << "\nArchivo seleccionado: " << BOLD << archivo_seleccionado << RESET << std::endl << std::endl;

  // Inicializamos el grafo con 0 vértices. Posteriormente se carga desde el archivo.
  Grafo* grafo = new Grafo(archivo_seleccionado);

  // DEBUGGING:
  // Mostramos la matriz de adyacencia cargada desde el archivo.
  // std::cout << "Matriz inicial cargada desde el archivo:" << std::endl;
  // grafo->MostrarGrafo();

  // Preguntamos los vértices de origen y destino.
  int origen, destino;
  MostrarConEfecto("Ingrese el vértice de origen: ");
  std::cin >> origen;
  if (origen < 0) {
    std::cerr << RED << "Vértice de origen inválido (negativo)." << RESET << std::endl;
    delete grafo;
    return 1;
  }
  MostrarConEfecto("Ingrese el vértice de destino: ");
  std::cin >> destino;
  if (destino < 0) {
    std::cerr << RED << "Vértice de destino inválido (negativo)." << RESET << std::endl;
    delete grafo;
    return 1;
  }
  std::cout << std::endl;

  // Abrimos el archivo para la salida de las iteraciones.
  std::ofstream archivo_salida("output/resultados.txt");
  if (!archivo_salida) {
    std::cerr << "Error al abrir el archivo de salida." << std::endl;
    delete grafo;
    return 1;
  }

  std::vector<Nodo> camino;
  // Mostramos datos del grafo.
  grafo->MostrarInformacion(origen, destino, archivo_salida);

  // Inicializamos el coste total del camino.
  int costo_total = 0;

  // Realizamos la búsqueda según la opción seleccionada.
  if (opcion == 1) { // Búsqueda en amplitud.
    camino = Busqueda::BusquedaEnAmplitud(*grafo, origen-1, destino-1, costo_total, archivo_salida);
  } else if (opcion == 2) { // Búsqueda en profundidad.
    camino = Busqueda::BusquedaEnProfundidad(*grafo, origen-1, destino-1, costo_total, archivo_salida);
  }

  // Mostramos el camino y el costo total.
  MostrarCamino(camino, costo_total, origen, destino, archivo_salida);
  std::cout << GREEN << "Resultado depositado en archivo " << BOLD << "'output/resultados.txt'." << RESET << std::endl;
  
  // Cerrar el archivo de salida.
  archivo_salida.close();
  // Liberamos la memoria.
  delete grafo;
  return 0;
}