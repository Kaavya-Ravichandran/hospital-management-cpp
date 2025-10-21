# Hospital Management System Makefile
# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -O2 -pthread -Wall -Wextra

# Target executable
TARGET = server

# Source files
SOURCES = server.cpp

# Header files
HEADERS = httplib.h json.hpp

# Default target
all: $(TARGET)

# Build the server
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "🏥 Building Hospital Management System..."
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)
	@echo "✅ Build successful! Run with: ./$(TARGET)"

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -f $(TARGET) hospital_data.txt
	@echo "✅ Clean complete!"

# Run the server
run: $(TARGET)
	@echo "🚀 Starting Hospital Management System server..."
	./$(TARGET)

# Install dependencies (download headers)
deps:
	@echo "📦 Downloading dependencies..."
	@if [ ! -f httplib.h ]; then \
		echo "Downloading httplib.h..."; \
		curl -s -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h; \
	fi
	@if [ ! -f json.hpp ]; then \
		echo "Downloading json.hpp..."; \
		curl -s -o json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp; \
	fi
	@echo "✅ Dependencies ready!"

# Open the web interface
web:
	@echo "🌐 Opening web interface..."
	@echo "Make sure the server is running first!"
	open index.html

# Full setup: download deps, build, and run
setup: deps all run

# Help target
help:
	@echo "🏥 Hospital Management System - Available targets:"
	@echo "  make deps     - Download required dependencies"
	@echo "  make all      - Build the server (default)"
	@echo "  make run      - Build and run the server"
	@echo "  make clean    - Remove build artifacts and data"
	@echo "  make web      - Open the web interface"
	@echo "  make setup    - Full setup: deps + build + run"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "📋 Quick start:"
	@echo "  1. make setup    # Download deps, build, and run"
	@echo "  2. make web      # Open web interface in browser"
	@echo "  3. Ctrl+C        # Stop server when done"

# Phony targets
.PHONY: all clean run deps web setup help
