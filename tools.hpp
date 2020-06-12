#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "threads.h"
#include "hash.hpp"
#include <iostream>
#include <chrono>

std::string GenerateRandomString(HashType Hash);

class DragNDrop : public Fl_Box
{
public:
    DragNDrop(int x,int y,int w,int h,const char *l=0):Fl_Box(x,y,w,h,l){};
    int handle (int e);
};
void init_ui();

#endif
