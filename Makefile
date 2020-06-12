all:
	g++ -lpthread -lfltk -lfltk_images fltk/include/threads.h main.cpp hash.cpp hash.hpp tools.cpp tools.hpp hashlibpp/src/*.cpp hashlibpp/src/*.h 
