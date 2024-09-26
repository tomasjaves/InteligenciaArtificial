CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17

SRC = src/busqueda_amplitud.cpp src/busqueda_profundidad.cpp src/grafo.cpp src/main.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
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
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@echo "${CYAN}${BOLD}Compilando${RESET} ${GRAY}${1}${RESET} ${CYAN}${BOLD}($(CURRENT_FILE) DE $(TOTAL_FILES))${RESET}...${GRAY}"
	@$(CXX) $(CXXFLAGS) -c -o $(2) $(1)
endef

all: $(EXEC)
	@echo "$(PURPLE)$(BOLD)Compilación completada$(RESET)"

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LBLIBS)

build/%.o: src/%.cpp
	$(call compile,$<,$@)

clean:
	@echo "${RED}$(BOLD)Limpieza en progreso: eliminando archivos de compilación y ejecutables...$(RESET)${GRAY}"
	rm -rf $(OBJ) $(EXEC)
