CLIBS= -fsanitize=address -g
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g
LIBP=-L.
OBJECTS=src/extra_func/dialogueFunc.o src/main.o src/tiles/atributes.o src/tiles/tiles.o src/gameMap.o src/extra_func/loadFileFunc.o src/tiles/Maptiles/empty_space.o src/tiles/Maptiles.o src/tiles/towers/Tower.o src/tiles/towers/TowerAOE.o src/tiles/towers/TowerSingleTG.o src/tiles/towers/TowerObstacle.o src/tiles/Unit.o src/tiles/attackers/attacker.o src/tiles/Maptiles/wall.o src/extra_func/readPropertyFunc.o
DOXY=./Doxyfile
BIN=./towerAttack

all: $(OBJECTS) 

	make compile

	make doc

compile: $(BIN)


$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN) $(CLIBS)

clean:
	rm -f src/*.o *~ towerAttack
	rm -f src/extra_func/*.o 
	rm -f src/tiles/*.o 
	rm -f src/tiles/towers/*.o 
	rm -f src/tiles/attackers/*.o 
	rm -f src/tiles/Maptiles/*.o 
	rm -r doc

run:
	./towerAttack

doc: $(DOXY) 
	doxygen $(DOXY)  2> /dev/null > /dev/null


%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

