#include <iostream>
#include "hash.hpp"
#include "tools.hpp"

int main(int argc, char ** argv) {
    Fl::scheme("gtk+");             //skin for window: gtk+
    init_window(argc,argv);         //main window initialization
    Fl::lock();
    return Fl::run();
}
