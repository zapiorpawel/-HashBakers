#include "tools.hpp"
#include <random>
#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_BMP_Image.H>
#include <Fl/Fl_Scroll.H>
#include <Fl/Fl_Multiline_Output.H>

std::string filepath = "";
Fl_Output *md5_output, *sha1_output, *sha256_output, *sha384_output, *sha512_output;
Fl_Check_Button *md5_switch, *sha1_switch, *sha256_switch, *sha384_switch, *sha512_switch;
Fl_Double_Window *window_info;
DragNDrop *b;
Fl_Thread prime_thread;

const char *licenses =
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


int DragNDrop::handle (int e)
{
    if(e == FL_PASTE)
    {
        cout << "dropped!" <<endl;
        filepath = Fl::event_text();
        if(filepath.find('\n',0) != std::string::npos)
        {
            fl_message_title("Warning!");
            fl_alert("Multiple files dropped!");
            filepath = "";
            label("drop your file here");
            b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
            return 0;
        }
        b->align(FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        md5_output->value("");
        sha1_output->value("");
        sha256_output->value("");
        sha384_output->value("");
        sha512_output->value("");
        label(filepath.c_str());
        return 0;
    }
}

int check_oven(Fl_Check_Button *button, HashBaker &oven, Fl_Output *out)
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

void* control_thread(void *p)
{
    int ccount = 0;
    Fl_Button *button = (Fl_Button*) p;
    Fl::lock();
    button->deactivate();
    sha512_switch->deactivate();
    sha384_switch->deactivate();
    sha256_switch->deactivate();
    sha1_switch->deactivate();
    md5_switch->deactivate();
    Fl::unlock();
    HashBaker md5(HashType::MD5,filepath);
    HashBaker sha1(HashType::SHA1,filepath);
    HashBaker sha256(HashType::SHA256,filepath);
    HashBaker sha384(HashType::SHA384,filepath);
    HashBaker sha512(HashType::SHA512,filepath);

    if(md5_switch->value())
        {md5.Bake(); ccount++; md5_output->value("Calculating...");}
    if(sha1_switch->value())
        {sha1.Bake(); ccount++; sha1_output->value("Calculating...");}
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
    sha512_switch->activate();
    sha384_switch->activate();
    sha256_switch->activate();
    sha1_switch->activate();
    md5_switch->activate();
    Fl::unlock();

    return NULL;
}

void start_callback(Fl_Widget *z, void *k) {
    std::cout << "pressed" << std::endl;
    if(filepath == "")
    {
        fl_message_title("Warning!");
        fl_alert("No file selected!");
        return;
    }
    fl_create_thread(prime_thread, control_thread, z);
    return;
}

void informations(Fl_Widget *z, void *k) {
    window_info->show();
    return;
}


void switch_callback(Fl_Widget *z, void *k)
{
    Fl_Check_Button *button = (Fl_Check_Button*)z;
    Fl_Output *output = (Fl_Output*)k;
    if(!button->value())
        output->deactivate();
    else
        output->activate();
    return;
}

void init_ui()
{
    Fl_Button *button = new Fl_Button(480, 200, 200, 100, "Start");
    Fl_Button *button2 = new Fl_Button(480, 140, 200, 50, "Info");
    b = new DragNDrop(20,20,450,280,"drop your file here");
    Fl_Box *instuct = new Fl_Box (20, 305, 350, 30, "@undo  Please, decide which types of hashes you want to get:");
    Fl_BMP_Image *image = new Fl_BMP_Image("logo.bmp");
    Fl_Box *img = new Fl_Box (480,0,200,150);
    img->image(image);
    b->box(FL_DOWN_BOX);
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
    b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    b->color(FL_WHITE);
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
    button->callback(start_callback,0);
    button2->callback(informations,0);
    return;
}

void init_window(int argc, char **argv)
{
    Fl_Window *window = new Fl_Window(700,500, "Hasherino");
    init_ui();
    window->end();
    window->show(argc,argv);
    window_info = new Fl_Double_Window(500,500, "About");
    Fl_BMP_Image *image = new Fl_BMP_Image("logo.bmp");
    Fl_Box *img2 = new Fl_Box (150,0,200,150);
    img2->image(image);
    Fl_Box *about = new Fl_Box(110, 100, 280, 180, "File hashing software developed on Programming II course at the \n Faculty of Applied Mathematics of the Silesian University of Technology. \n \n  Authors: Semir Sionek, Paweł Zapiór");
    Fl_Box *info = new Fl_Box(10,270,100,25,"Licenses:");
    info->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    Fl_Scroll *scroll = new Fl_Scroll(10,300,480,180);
    Fl_Multiline_Output *license_show = new Fl_Multiline_Output(0,0,650,500);
    license_show->value(licenses);
    scroll->end();
    scroll->scroll_to(-10,-300);
    about->align(FL_ALIGN_WRAP);
    window_info->end();
}

string GenerateRandomString(HashType Hash)
{
	//może zmień na std::vector
	string Chars = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ1234567890";
	default_random_engine G{static_cast<long unsigned int>(chrono::high_resolution_clock::now().time_since_epoch().count())};
	int StringLength;
	string RandomString;

    uniform_int_distribution<int> Rand(0,Chars.length()-1);
	switch(Hash)
	{
		case MD5:
			StringLength = 32;
			break;
		case SHA1:
			StringLength = 40;
			break;
		case SHA256:
			StringLength = 64;
			break;
		case SHA384:
			StringLength = 96;
			break;
		case SHA512:
			StringLength = 128;
			break;
	}
	for (int i = 0; i < StringLength; i++)
		RandomString += Chars[Rand(G)];
	return RandomString;
}
