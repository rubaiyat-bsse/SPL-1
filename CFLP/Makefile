# --- Compiler Settings ---
CXX = g++
CXX_STANDARD = -O3 -std=c++17

# --- CPLEX Specific Settings ---
CPLEX_INCDIR = /opt/ibm/ILOG/CPLEX_Studio2212/cplex/include
CONCERT_INCDIR = /opt/ibm/ILOG/CPLEX_Studio2212/concert/include
CPLEX_LIBDIR = /opt/ibm/ILOG/CPLEX_Studio2212/cplex/lib/x86-64_linux/static_pic
CONCERT_LIBDIR = /opt/ibm/ILOG/CPLEX_Studio2212/concert/lib/x86-64_linux/static_pic

CXXFLAGS_CPLEX = $(CXX_STANDARD) -I$(CPLEX_INCDIR) -I$(CONCERT_INCDIR)
LDFLAGS_CPLEX = -L$(CPLEX_LIBDIR) -L$(CONCERT_LIBDIR) -lilocplex -lconcert -lcplex -lm -lpthread -ldl

# --- Sources and Objects ---
# Shared utility objects
CORE_SRCS = src/DataProcessor.cpp src/ParameterGenerator.cpp src/KMLGenerator.cpp
CORE_OBJS = $(patsubst src/%.cpp,build/%.o,$(CORE_SRCS))

CPLEX_SRCS = src/cflpCplex.cpp
CPLEX_OBJS = $(patsubst src/%.cpp,build/%.o,$(CPLEX_SRCS))

# Target: lagrange-cli (CLI)
CLI_SRCS = src/cli.cpp src/lagrangeCFLP.cpp
CLI_OBJS = $(patsubst src/%.cpp,build/%.o,$(CLI_SRCS))
EXEC_CLI = build/lagrange-cli

# --- Rules ---
all: lagrange-cli

# Rule to build the unified executable
lagrange-cli: $(CORE_OBJS) $(CLI_OBJS) $(CPLEX_OBJS)
	@mkdir -p build
	$(CXX) $(CORE_OBJS) $(CLI_OBJS) $(CPLEX_OBJS) -o $(EXEC_CLI) $(LDFLAGS_CPLEX)

# Pattern rules for object compilation
build/cflpCplex.o: src/cflpCplex.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS_CPLEX) -c $< -o $@

build/lagrangeCFLP.o: src/lagrangeCFLP.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS_CPLEX) -c $< -o $@

build/cli.o: src/cli.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS_CPLEX) -c $< -o $@

# Standard compilation for everything else
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXX_STANDARD) -c $< -o $@

# --- Cleanup ---
clean:
	rm -f build/*.o $(EXEC_CLI)
