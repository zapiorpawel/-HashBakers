#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include "hash.hpp"

Fl_Output *output;
std::string filepath;

class DragNDrop : public Fl_Box
{
public:
    DragNDrop(int x,int y,int w,int h,const char *l=0):Fl_Box(x,y,w,h,l){};
    int handle (int e)
    {
        if( e == FL_PASTE)
        {
            cout << "dropped!" <<endl;
            filepath = Fl::event_text();
            label(filepath.c_str());
        }
    }
};

void dostuff(Fl_Widget *, void *) {
    std::cout << "pressed" << std::endl;
    HashBaker oven(HashType::MD5,filepath);
    oven.Bake();
    while(true)
    {
        if(oven.IsCooked())
            break;
    }
    output->value(oven.TakeOut().c_str());
    return;
}

int main(int argc, char ** argv) {
    Fl::scheme("gtk+");
    Fl_Window *window = new Fl_Window(320,120);
    Fl_Button *button = new Fl_Button(220, 20, 80, 25, "Start");
    DragNDrop *box = new DragNDrop(20,20,180,40,"drop here");
    output = new Fl_Output(50,80,150,25,"MD5:");
    box->align(FL_ALIGN_WRAP);
    button->callback(dostuff,0);
    window->end();
    window->show(argc,argv);
    return Fl::run();
}
