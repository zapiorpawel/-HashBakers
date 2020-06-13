#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#ifdef WIN32
#include "threads.h"
#else
#include "fltk/include/threads.h"
#endif
#include "hash.hpp"
#include <iostream>
#include <chrono>

class DragNDrop : public Fl_Box                                                 //dragNdrop class based on Fl_Box
{
public:
    DragNDrop(int x,int y,int w,int h,const char *l=0):Fl_Box(x,y,w,h,l){};     //attribiutes definition
    int handle (int e);
};
void init_window(int argc, char ** argv);

#endif
