CXX = g++
CXXFLAGS = -pedantic -std=c++17

SRC = src/cell.cc src/main.cc src/labyrinth.cc src/aux_functions.cc
OBJ = $(SRC:src/%.cc=build/%.o)
EXEC = program

# Colores
RED=\033[31m
PURPLE=\033[35m
CYAN=\033[36m
GRAY=\033[90m
BOLD=\033[1m
RESET=\033[0m

# Contador para el progreso
TOTAL_FILES := $(words $(SRC))
CURRENT_FILE = 0

define compile
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1)) ))
	@echo "${CYAN}${BOLD}Compilando${RESET} ${GRAY}${1}${RESET} ${CYAN}${BOLD}($(CURRENT_FILE) DE $(TOTAL_FILES))${RESET}...${GRAY}"
	@$(CXX) $(CXXFLAGS) -c -o $(2) $(1)
endef

# Regla principal
all: build $(EXEC)
	@echo "${PURPLE}${BOLD}Compilación completada: Ejecutable 'program' creado correctamente.${RESET}"

# Crear el ejecutable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilar archivos fuente a objetos en el directorio build
build/%.o: src/%.cc | build
	$(call compile,$<,$@)

# Crear el directorio build si no existe
build:
	@mkdir -p build

# Limpiar archivos generados
clean:
	@echo "${RED}${BOLD}Limpieza en progreso: eliminando archivos de compilación y ejecutables...${RESET}${GRAY}"
	rm -rf build $(EXEC) output
	
reset:
	@echo "${RED}${BOLD}Reiniciando: eliminando fichero de salida 'instancias.md'...${RESET}${GRAY}"
	rm -rf output/instancias.md