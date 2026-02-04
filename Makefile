
#  Makefile für Dinic vs. Edmonds-Karp Benchmark
#  Autor: Wesam Al-Hayani
#  Universität: TU Dortmund


CXX       := g++
CXXFLAGS  := -std=c++17 -O3 -Wall -Wextra -pedantic
TARGET    := max_flow_benchmark

SRC_DIR   := src
SRCS      := $(SRC_DIR)/max_flow.cpp $(SRC_DIR)/dinic.cpp $(SRC_DIR)/benchmark.cpp
OBJS      := $(SRCS:.cpp=.o)

#Plot settings 
PLOTS_DIR := plots
CSV       := results.csv
PLOT_SCRIPT := $(SRC_DIR)/plot_results.py

# --------------------------------------------------------------
PYTHON    := /opt/anaconda3/bin/python

# --------------------------------------------------------------
.PHONY: all clean run plot rebuild

# --------------------------------------------------------------
all: $(TARGET) run plot
	@echo "Build + Benchmark + Plots fertig!"

$(TARGET): $(OBJS)
	@echo "🔧 Linking $@ ..."
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Build erfolgreich!"

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "🛠️  Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "Running Benchmark..."
	./$(TARGET)

plot:
	@echo "Generating plots..."
	@mkdir -p $(PLOTS_DIR)
	$(PYTHON) $(PLOT_SCRIPT) $(CSV)
	@mv -f bar_*.png line_*.png box_*.png $(PLOTS_DIR) 2>/dev/null || true
	@echo "Plots gespeichert in: $(PLOTS_DIR)/"

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) $(OBJS)
	rm -rf $(PLOTS_DIR)
	rm -f bar_*.png line_*.png box_*.png

rebuild: clean all
