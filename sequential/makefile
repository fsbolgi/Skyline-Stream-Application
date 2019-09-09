CXX = g++
CXXFLAGS = -std=c++17 -O3
OBJECTS = main.o util.o stages.o linked_list.o
EXE = main
RM = rm -f

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(EXE)
	@echo "Done Compiling All"

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c main.cpp
	@echo "Done Compiling Main"

stages.o: stages.cpp stages.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c stages.cpp
	@echo "Done Compiling Stages"

linked_list.o: linked_list.cpp linked_list.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c linked_list.cpp
	@echo "Done Compiling Stages"

util.o: ../util.cpp ../util.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c ../util.cpp
	@echo "Done Compiling Util"

clean:
	$(RM) *.o $(EXE)
	@echo "Done Cleaning"