#include "tools.hpp"
#include <random>

std::string filepath = "";
Fl_Output *md5_output, *sha1_output, *sha256_output, *sha384_output, *sha512_output;
Fl_Check_Button *md5_switch, *sha1_switch, *sha256_switch, *sha384_switch, *sha512_switch;
Fl_Thread prime_thread;

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
            label("drop your files here");
            return 0;
        }
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
        {
            //usunąć później
            Fl::lock();
            out->value(GenerateRandomString(oven.Hashtype).c_str());
            Fl::unlock();
            return 0;
        }
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
        {md5.Bake(); ccount++;}
    if(sha1_switch->value())
        {sha1.Bake(); ccount++;}
    if(sha256_switch->value())
        {sha256.Bake(); ccount++;}
    if(sha384_switch->value())
        {sha384.Bake(); ccount++;}
    if(sha512_switch->value())
        {sha512.Bake(); ccount++;}

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
    Fl_Button *button = new Fl_Button(500, 180, 180, 100, "Start");
    Fl_Button *button2 = new Fl_Button(500, 115, 180, 50, "Info");
    DragNDrop *b = new DragNDrop(20,20,450,280,"drop your files here");
    Fl_Box *instuct = new Fl_Box (20, 305, 350, 30, "@undo  Please, decide which types of hashes you want to get:");
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
    b->align(FL_ALIGN_WRAP);
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
    return;
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
