all:
	g++ -lpthread main.cpp hash.cpp hash.hpp tools.cpp tools.hpp hashlibpp/src/*.cpp hashlibpp/src/*.h 
