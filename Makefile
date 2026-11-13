CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -g
SRC = $(wildcard src/*.cpp)
all: smti cliente
smti: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o smti
cliente: tools/cliente_prueba.cpp
	$(CXX) $(CXXFLAGS) tools/cliente_prueba.cpp -o cliente
clean:
	rm -f smti cliente
