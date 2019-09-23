CXX = g++
CXXFLAGS = -std=c++17 -O3 
LDFLAGS = -pthread
FFFLAGS = -I . -finline-functions -DNDEBUG
OBJECTS = main.o util.o stages.o linked_list.o
EXE = main
RM = rm -f

$(EXE): $(OBJECTS)
	$(CXX) $(FFFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS) $(CXXFLAGS)
	@echo "Done Compiling All"

main.o: main.cpp
	$(CXX) $(FFFLAGS) -c main.cpp $(LDFLAGS) $(CXXFLAGS) 
	@echo "Done Compiling Main"

stages.o: stages.cpp stages.hpp
	$(CXX) -c stages.cpp $(LDFLAGS) $(CXXFLAGS)  
	@echo "Done Compiling Stages"

linked_list.o: ../linked_list.cpp ../linked_list.hpp
	$(CXX) -c ../linked_list.cpp $(LDFLAGS) $(CXXFLAGS)
	@echo "Done Compiling Stages"

util.o: ../util.cpp ../util.hpp
	$(CXX) -c ../util.cpp $(LDFLAGS) $(CXXFLAGS) 
	@echo "Done Compiling Util"

clean:
	$(RM) *.o $(EXE)
	@echo "Done Cleaning"
