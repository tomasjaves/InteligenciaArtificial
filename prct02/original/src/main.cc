#include "../include/aux_functions.h"
#include "../include/cell.h"
#include "../include/labyrinth.h"
#include <cstdlib>

/**
 * @brief Función principal
 * 
 * @param argc 
 * @param argv 
 */
int main(int argc, char* argv[]) {
  
  Uso(argc, argv); // Comprueba que los parámetros sean los correctos y sino muestra mensaje de uso
  
  std::string nombre_fichero{argv[1]}; // Nombre del fichero de entrada
  nombre_fichero = "inputs/" + nombre_fichero; // Dirección del fichero de entrada
  std::ifstream fichero_entrada{nombre_fichero, std::ios_base::in}; // Abrimos el fichero como lectura

  if (fichero_entrada.fail()) { // Error al abrir el fichero
    std::cerr << RED << BOLD << "Fallo al abrir el fichero de entrada.\n" << RESET;
    return 1;                     
  }

  Labyrinth labyrinth(fichero_entrada); // Creamos el laberinto
  bool exit = false;

  while (exit != true) {
    system("clear");
    std::cout << GRAY << BOLD << "Laberinto Seleccionado\n\n";
    std::cout << labyrinth.PrintLabyrinth() << "\n" << RESET;
    ImprimirMenu();
    char opcion;
    std::cout << "\n";
    std::cout << UNDERLINE << "Introduce una opción:" << RESET << " ";
    std::cin >> opcion;
    switch (opcion) {
      case '1': // Realizar búsqueda A*
        StoreSearch(labyrinth, nombre_fichero);
        break;
      case '2': // Cambiar nodo inicial
        std::cout << GREEN << BOLD << "\nCambiar nodo inicial\n\n" << RESET;
        labyrinth.ChangeNode(0);
        break;
      case '3': // Cambiar nodo final
        std::cout << GREEN << BOLD << "\nCambiar nodo final\n\n" << RESET;
        labyrinth.ChangeNode(1);
        break;
      case '4': // Cambiar heurística
        std::cout << GREEN << BOLD << "\nCambiar heurística\n\n" << RESET;
        labyrinth.ChangeHeuristic();
        break;
      case '5': // Imprimir laberinto
        std::cout << GREEN << BOLD "\nImprimir laberinto\n\n" << RESET;
        std::cout << labyrinth.PrintLabyrinth();
        break;
      case '6': // Cambiar fichero de entrada
        std::cout << GREEN << BOLD << "\nCambiar fichero de entrada\n\n" << RESET;
        std::cout << UNDERLINE << "Introduce el nuevo fichero de entrada (sin la ruta):" << RESET << " ";
        std::cin >> nombre_fichero;  // Recibimos el nombre sin la ruta
        fichero_entrada.close();
        
        nombre_fichero = "inputs/" + nombre_fichero;  // Actualizamos el nombre completo con la nueva ruta
        fichero_entrada.open(nombre_fichero);  // Intentamos abrir el nuevo fichero

        if (fichero_entrada.fail()) {
          std::cerr << YELLOW << BOLD << "Fallo al abrir el nuevo fichero. Se seguirá usando " << RESET << argv[1] << BOLD << YELLOW << " por defecto.\n\n";
          nombre_fichero = "inputs/" + std::string(argv[1]);  // Restauramos el fichero original
          fichero_entrada.open(nombre_fichero);  // Reabrimos el fichero original
        } else {
          labyrinth = Labyrinth(fichero_entrada);  // Cargamos el nuevo laberinto
        }
        break;
      case '7': // Salir
        std::cout << GREEN << BOLD << "\nSaliendo...\n\n";
        exit = true;
        break; // Señal para salir del programa
      default: // Opción inválida
        std::cout << RED << BOLD << "\nOpción inválida\n\n";
        break;
    }
  }
  return 0; // Fin del programa con éxito
}