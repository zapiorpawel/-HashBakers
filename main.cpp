#include <iostream>
#include "hash.hpp"
#include "tools.hpp"

int main(int argc, char ** argv) {
    Fl::scheme("gtk+");
    init_window(argc,argv);
    Fl::lock();
    return Fl::run();
}
