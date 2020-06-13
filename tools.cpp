#include "tools.hpp"
#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_BMP_Image.H>
#include <Fl/Fl_Scroll.H>
#include <Fl/Fl_Multiline_Output.H>

std::string filepath = "";
Fl_Output *md5_output, *sha1_output, *sha256_output, *sha384_output, *sha512_output;            //Pointers for string output hashes
Fl_Check_Button *md5_switch, *sha1_switch, *sha256_switch, *sha384_switch, *sha512_switch;      //Pointers for checkig if hash type should be counted
Fl_Double_Window *window_info;                                                                  //Pointer for info window (click info button)
DragNDrop *b;
Fl_Thread prime_thread;

const char *licenses =                                                                          //licence informations. Display in 'about' window.
R"(hasherino is licensed under the terms
of the GNU General Public License version 3

Available online under:
http://www.gnu.org/licenses/gpl-3.0.html

hashlib++ - a simple hash library for C++
Copyright (c) 2007-2011 Benjamin Gr¸delbach
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1)     Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
2)     Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in
        the documentation and/or other materials provided with the
        distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.)

FLTK 1.x.x by Bill Spitzak and contributors
Licensed under modified GPLv2, which is available for download from
https://www.fltk.org/COPYING.php)";


int DragNDrop::handle (int e)                                                                   //DragNDrop function
{
    if(e == FL_PASTE)
    {
        cout << "dropped!" <<endl;
        filepath = Fl::event_text();
        if(filepath.find('\n',0) != std::string::npos)                                          //protection against wrong dragNdrop usage
        {
            fl_message_title("Warning!");
            fl_alert("Multiple files dropped!");                                                //Only one file can be hashed per cycle
            filepath = "";
            label("drop your file here");                                                       //instruction how to use dragNdrop
            b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
            return 0;
        }
        b->align(FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        md5_output->value("");                                                                  //supposed values for hash outputs
        sha1_output->value("");
        sha256_output->value("");
        sha384_output->value("");
        sha512_output->value("");
        label(filepath.c_str());
        return 0;
    }
}

int check_oven(Fl_Check_Button *button, HashBaker &oven, Fl_Output *out)                       //protection: using for blocking check buttons in hashes counting time
{
    if(button->value())
    {
        if(oven.IsCooked())
        {
            out->value(oven.TakeOut().c_str());
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

void* control_thread(void *p)                                                                  //control threading function
{
    int ccount = 0;
    Fl_Button *button = (Fl_Button*) p;
    Fl::lock();
    button->deactivate();                                                                      //Button start desactivation
    sha512_switch->deactivate();                                                               //Switches desactivation to protect against changes during algoritm's work
    sha384_switch->deactivate();
    sha256_switch->deactivate();
    sha1_switch->deactivate();
    md5_switch->deactivate();
    Fl::unlock();
    HashBaker md5(HashType::MD5,filepath);                                                      //
    HashBaker sha1(HashType::SHA1,filepath);
    HashBaker sha256(HashType::SHA256,filepath);
    HashBaker sha384(HashType::SHA384,filepath);
    HashBaker sha512(HashType::SHA512,filepath);

    if(md5_switch->value())
        {md5.Bake(); ccount++; md5_output->value("Calculating...");}                            //check if hash algoritm finished work. If no print "Calculating.."
    if(sha1_switch->value())
        {sha1.Bake(); ccount++; sha1_output->value("Calculating...");}                          //-...-
    if(sha256_switch->value())
        {sha256.Bake(); ccount++; sha256_output->value("Calculating...");}
    if(sha384_switch->value())
        {sha384.Bake(); ccount++; sha384_output->value("Calculating...");}
    if(sha512_switch->value())
        {sha512.Bake(); ccount++; sha512_output->value("Calculating...");}

    while(true)
    {
        int fcount = 0;
        std::this_thread::sleep_for(chrono::milliseconds(250));
        cout << "loop" << endl;
        fcount += check_oven(md5_switch,md5,md5_output);
        fcount += check_oven(sha1_switch,sha1,sha1_output);
        fcount += check_oven(sha256_switch,sha256,sha256_output);
        fcount += check_oven(sha384_switch,sha384,sha384_output);
        fcount += check_oven(sha512_switch,sha512,sha512_output);
        if(fcount == ccount)
            break;
    }

    Fl::lock();
    button->activate();
    sha512_switch->activate();                                                               //Switches activation to enable hash next file
    sha384_switch->activate();
    sha256_switch->activate();
    sha1_switch->activate();
    md5_switch->activate();
    Fl::unlock();

    return NULL;
}

void start_callback(Fl_Widget *z, void *k) {
    std::cout << "pressed" << std::endl;                                                       //console debug informations
    if(filepath == "")
    {
        fl_message_title("Warning!");                                                           //alerts if no file has been selected
        fl_alert("No file selected!");
        return;
    }
    fl_create_thread(prime_thread, control_thread, z);
    return;
}

void informations(Fl_Widget *z, void *k) {                                                      //function show info window
    window_info->show();
    return;
}


void switch_callback(Fl_Widget *z, void *k)                                                     //function attribute actions to button "start"
{
    Fl_Check_Button *button = (Fl_Check_Button*)z;
    Fl_Output *output = (Fl_Output*)k;
    if(!button->value())
        output->deactivate();                                                                   //
    else
        output->activate();
    return;
}

void init_ui()
{
    Fl_Button *button = new Fl_Button(480, 200, 200, 100, "Start");                              //button start object iniclalization
    Fl_Button *button2 = new Fl_Button(480, 140, 200, 50, "About");                              //button About object iniclalization
    b = new DragNDrop(20,20,450,280,"drop your file here");                                      //dragNdrop widget
    Fl_Box *instuct = new Fl_Box (20, 305, 350, 30, "@undo  Please, decide which types of hashes you want to get:");      //box with instruction text how to use program
    Fl_BMP_Image *image = new Fl_BMP_Image("logo.bmp");                                          //logo graphic file pointer
    Fl_Box *img = new Fl_Box (480,0,200,150);                                                    //logo graphic file box
    img->image(image);
    b->box(FL_DOWN_BOX);                                                                         //frame arround box with dragNdrop widget
    md5_output = new Fl_Output(90,340,600,25);                                                   //uneditable for user text field with hash output
    md5_switch = new Fl_Check_Button(10,340,70,25,"MD5");                                        //check switch (mark to get hash this type)
    sha1_output = new Fl_Output(90,370,600,25);
    sha1_switch = new Fl_Check_Button(10,370,70,25,"SHA1");
    sha256_output = new Fl_Output(90,400,600,25);
    sha256_switch = new Fl_Check_Button(10,400,70,25,"SHA256");
    sha384_output = new Fl_Output(90,430,600,25);
    sha384_switch = new Fl_Check_Button(10,430,70,25,"SHA384");
    sha512_output = new Fl_Output(90,460,600,25);
    sha512_switch = new Fl_Check_Button(10,460,70,25,"SHA512");                                  // for next hash types analogus text fields and switches
    b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);                                       //text wrapping in dragNdrop box
    b->color(FL_WHITE);                                                                          //dragNdrop box colour
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
    button->callback(start_callback,0);                                                           //actions fot 'start' button
    button2->callback(informations,0);                                                            //actions for 'about' button
    return;
}

void init_window(int argc, char **argv)
{
    Fl_Window *window = new Fl_Window(700,500, "Hasherino");                                      //Main window inicjaliization
    init_ui();                                                                                    //show main window UI
    window->end();
    window->show(argc,argv);
    window_info = new Fl_Double_Window(500,500, "About");                                          //About info window inicjalization
    Fl_BMP_Image *image = new Fl_BMP_Image("logo.bmp");                                            //logo object inicjalization
    Fl_Box *img2 = new Fl_Box (150,0,200,150);                                                     //box with logo.bmp placed in info window
    img2->image(image);                                                                            //show logo in about window
    Fl_Box *about = new Fl_Box(110, 100, 280, 180, "File hashing software developed on Programming II course. \n \n Faculty of Applied Mathematics \n Silesian University of Technology \n \n  Authors: Semir Sionek, Paweł Zapiór");  //show informations in about window
    Fl_Box *info = new Fl_Box(10,270,100,25,"Licenses:");                                          //show box with informations about licence
    info->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);                                                    //text wrapping in info window
    Fl_Scroll *scroll = new Fl_Scroll(10,300,480,180);                                             //scroll controlers for information about licences in info window
    Fl_Multiline_Output *license_show = new Fl_Multiline_Output(0,0,650,500);                      //text field with licence information inicjalization
    license_show->value(licenses);                                                                 //show lincence information in text field
    scroll->end();
    scroll->scroll_to(-10,-300);
    about->align(FL_ALIGN_WRAP);                                                                    //text wrapping for licence informations
    window_info->end();
}
