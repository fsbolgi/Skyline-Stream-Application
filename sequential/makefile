CXX = g++
CXXFLAGS = -std=c++17 -O3
OBJECTS = main.o util.o
EXE = main
RM = rm -f

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(EXE)
	@echo "Done Compiling All"

# these lines are useless, can be removed
main.o: main.cpp util.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c main.cpp
	@echo "Done Compiling Main"

util.o: util.cpp util.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c util.cpp
	@echo "Done Compiling Util"
# up until here


clean:
	$(RM) *.o $(EXE)
	@echo "Done Cleaning"