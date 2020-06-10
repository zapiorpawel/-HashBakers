#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "threads.h"
#include "hash.hpp"
#include "tools.hpp"
#include <FL/Fl_Draw.H>
#include <FL/Fl_BMP_Image.H>

Fl_Thread prime_thread;
Fl_Output *md5_output, *sha1_output, *sha256_output, *sha384_output, *sha512_output;
Fl_Check_Button *md5_switch, *sha1_switch, *sha256_switch, *sha384_switch, *sha512_switch;
std::string filepath = "";

class Rysunek : public Fl_Widget
{
void draw(void);
    Fl_BMP_Image *bitmapa;
    public:
    Rysunek(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H)
        {
    bitmapa = new Fl_BMP_Image("logo.bmp");
        }
    ~Rysunek()
        {
        delete bitmapa;
        }
};

class DragNDrop : public Fl_Box
{
public:
    DragNDrop(int x,int y,int w,int h,const char *l=0):Fl_Box(x,y,w,h,l){};
    int handle (int e)
    {
        if(e == FL_PASTE)
        {
            cout << "dropped!" <<endl;
            filepath = Fl::event_text();
            label(filepath.c_str());
        }
    }
};



void switch_callback(Fl_Widget *z, void *k)
{
    Fl_Check_Button *btn = (Fl_Check_Button*)z;
    Fl_Output *otpt = (Fl_Output*)k;
    if(!btn->value())
        otpt->deactivate();
    else
        otpt->activate();
    return;
}

int checkoven(Fl_Check_Button *button, HashBaker &oven, Fl_Output *out, HashType hashtype)
{
    if(button->value())
    {
        if(oven.IsCooked())
        {
            out->value(oven.TakeOut().c_str());
            return 1;
        }
        else
        {
            Fl::lock();
            out->value(GenerateRandomString(hashtype).c_str());
            Fl::unlock();
            return 0;
        }
    }
    return 0;
}

void* controlthread(void *p)
{
    int ccount = 0;
       Fl_Button *button = (Fl_Button*) p;
       Fl::lock();


       button->deactivate();

        Fl::unlock();
        HashBaker md5(HashType::MD5,filepath);
        HashBaker sha1(HashType::SHA1,filepath);
        HashBaker sha256(HashType::SHA256,filepath);
        HashBaker sha384(HashType::SHA384,filepath);
        HashBaker sha512(HashType::SHA512,filepath);

        if(md5_switch->value())
            {md5.Bake(); ccount++;md5_switch->deactivate();}
        if(sha1_switch->value())
            {sha1.Bake(); ccount++;sha1_switch->deactivate();}
        if(sha256_switch->value())
            {sha256.Bake(); ccount++;sha256_switch->deactivate();}
        if(sha384_switch->value())
            {sha384.Bake(); ccount++;sha384_switch->deactivate();}
        if(sha512_switch->value())
            {sha512.Bake(); ccount++;sha512_switch->deactivate();}

        while(true)
        {
            int fcount = 0;
            std::this_thread::sleep_for(chrono::milliseconds(250));
            cout << "loop" << endl;
            fcount += checkoven(md5_switch,md5,md5_output,HashType::MD5);
            fcount += checkoven(sha1_switch,sha1,sha1_output,HashType::SHA1);
            fcount += checkoven(sha256_switch,sha256,sha256_output,HashType::SHA256);
            fcount += checkoven(sha384_switch,sha384,sha384_output,HashType::SHA384);
            fcount += checkoven(sha512_switch,sha512,sha512_output,HashType::SHA512);
            if(fcount == ccount)
                break;
        }
        Fl::lock();
        button->activate();

        sha512_switch->activate();
        sha384_switch->activate();
        sha256_switch->activate();
        sha1_switch->activate();
        md5_switch->activate();

        Fl::unlock();


        return NULL;
}

void dostuff(Fl_Widget *z, void *k) {
    std::cout << "pressed" << std::endl;
    if(filepath == "")
    {
        fl_message_title("Warning!");
        fl_alert("No file selected!");
        return;
    }
    fl_create_thread(prime_thread, controlthread, z);
    return;
}

int main(int argc, char ** argv) {
    Fl::scheme("gtk+");
    Fl_Window *window = new Fl_Window(700,500, "Hasherino");
    Fl_Button *button = new Fl_Button(500, 180, 180, 100, "Start");

//    Rysunek *rysunek(0, 0, 243, 247);

    Fl_Button *button2 = new Fl_Button(500, 115, 180, 50, "Info");
    DragNDrop *box = new DragNDrop(20,20,350,300,"drop your files here");
    Fl_Box info(18, 295, 350, 30, "@undo  Please, decide which types of hashes you want to get:");
    md5_output = new Fl_Output(90,340,600,25);
    md5_switch = new Fl_Check_Button(10,340,70,25,"MD5");
    sha1_output = new Fl_Output(90,370,600,25);
    sha1_switch = new Fl_Check_Button(10,370,70,25,"SHA1");
    sha256_output = new Fl_Output(90,400,600,25);
    sha256_switch = new Fl_Check_Button(10,400,70,25,"SHA256");
    sha384_output = new Fl_Output(90,430,600,25);
    sha384_switch = new Fl_Check_Button(10,430,70,25,"SHA384");
    sha512_output = new Fl_Output(90,460,600,25);
    sha512_switch = new Fl_Check_Button(10,460,70,25,"SHA512");
    box->align(FL_ALIGN_WRAP);
    md5_switch->set();
    sha1_switch->set();
    sha256_switch->set();
    sha384_switch->set();
    sha512_switch->set();
    md5_switch->callback(switch_callback,md5_output);
    sha1_switch->callback(switch_callback,sha1_output);
    sha256_switch->callback(switch_callback,sha256_output);
    sha384_switch->callback(switch_callback,sha384_output);
    sha512_switch->callback(switch_callback,sha512_output);
    button->callback(dostuff,0);
    window->end();
    window->show(argc,argv);
    Fl::lock();
    return Fl::run();
}
