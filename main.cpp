#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include "threads.h"
#include "hash.hpp"
#include "tools.hpp"

Fl_Thread prime_thread;
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

void* tempcalcstuff(void *p)
{
       Fl_Button *button = (Fl_Button*) p;
       Fl::lock();
       button->deactivate();
       output->deactivate();
        Fl::unlock();
        HashBaker oven(HashType::MD5,filepath);
        oven.Bake();
        while(true)
        {
            std::this_thread::sleep_for(chrono::milliseconds(250));
            cout << "while" << endl;
            if(oven.IsCooked())
                break;
            else
            {
               Fl::lock();
                output->value(GenerateRandomString(HashType::MD5).c_str());
                Fl::unlock();
            }
        }
        Fl::lock();
        output->value(oven.TakeOut().c_str());
        button->activate();
        output->activate();
        Fl::unlock();
        return NULL;
}

void dostuff(Fl_Widget *z, void *k) {
    std::cout << "pressed" << std::endl;
    fl_create_thread(prime_thread, tempcalcstuff, z);
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
    Fl::lock();
    return Fl::run();
}
