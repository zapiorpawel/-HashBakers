#include <iostream>
#include "hash.hpp"
#include "tools.hpp"

int main(int argc, char ** argv) {
    Fl::scheme("gtk+");
    Fl_Window *window = new Fl_Window(700,500, "Hasherino");
    init_ui();
    window->end();
    window->show(argc,argv);
    Fl::lock();
    return Fl::run();
}
