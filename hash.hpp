#ifndef HASH_HPP
#define HASH_HPP

#include "hashlibpp/src/hashlibpp.h"
#include <future>
#include <thread>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
//#include <string>
using namespace std;

//zamień na połączenie e.g HashClass->HashMD5
enum HashType
{
	MD5,
	SHA1,
	SHA256,
	SHA384,
	SHA512
};

class HashBaker
{
private:
    hashwrapper *HashWrapper;
    string FilePath;
    string Hash = "";
	future<string> thread;
	bool Lock;
public:
    HashBaker(HashType Hash, string Path);
    void Bake();
    bool IsCooked();
    string TakeOut();
};

#endif
