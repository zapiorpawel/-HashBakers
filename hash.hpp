#ifndef HASH_HPP
#define HASH_HPP

#include "hashlibpp/src/hashlibpp.h"
#include <future>
#include <thread>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
using namespace std;

enum HashType
{
	MD5,
	SHA1,
	SHA256,
	SHA384,
	SHA512
};

class HashBaker                                            //class for hash sting output making
{
private:
    hashwrapper *HashWrapper;
    string FilePath;
    string Hash = "";                                      //supposed value
	future<string> thread;
	bool Lock;
public:
    HashType Hashtype;
    HashBaker(HashType Hash, string Path);
    ~HashBaker();                                          //class dectructor
    void Bake();
    bool IsCooked();                                       //bool var - if hash is ready?
    string TakeOut();                                      //string for ready hash output
};

#endif
