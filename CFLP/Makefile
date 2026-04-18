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

# Target: cflpCplex
CPLEX_SRCS = src/cflpCplex.cpp
CPLEX_OBJS = $(patsubst src/%.cpp,build/%.o,$(CPLEX_SRCS))
EXEC_CPLEX = build/cflpCplex

# Target: lagrangeCFLP
LAGRANGE_SRCS = src/main.cpp src/lagrangeCFLP.cpp
LAGRANGE_OBJS = $(patsubst src/%.cpp,build/%.o,$(LAGRANGE_SRCS))
EXEC_LAGRANGE = build/lagrangeCFLP

# --- Rules ---
all: cflpCplex lagrangeCFLP

# Rule to build the exact CPLEX baseline
cflpCplex: $(CORE_OBJS) $(CPLEX_OBJS)
	@mkdir -p build
	$(CXX) $(CORE_OBJS) $(CPLEX_OBJS) -o $(EXEC_CPLEX) $(LDFLAGS_CPLEX)

# Rule to build the lagrange heuristic baseline (Standard build, no CPLEX linkage)
lagrangeCFLP: $(CORE_OBJS) $(LAGRANGE_OBJS)
	@mkdir -p build
	$(CXX) $(CORE_OBJS) $(LAGRANGE_OBJS) -o $(EXEC_LAGRANGE)

# Pattern rules for object compilation
# Compiling CPLEX objects requires the heavy include headers
build/cflpCplex.o: src/cflpCplex.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS_CPLEX) -c $< -o $@

# Standard compilation for everything else
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXX_STANDARD) -c $< -o $@

# --- Cleanup ---
clean:
	rm -f build/*.o $(EXEC_CPLEX) $(EXEC_LAGRANGE)
