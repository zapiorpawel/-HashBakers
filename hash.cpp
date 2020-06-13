#include "hash.hpp"
using namespace std;

HashBaker::HashBaker(HashType Hash, string Path)
{
	switch(Hash)                                                  //check which hash type is needed. Make appropriate hash wrappers
	{
		case MD5:
			HashWrapper = new md5wrapper();
			break;
		case SHA1:
			HashWrapper = new sha1wrapper();
			break;
		case SHA256:
			HashWrapper = new sha256wrapper();
			break;
		case SHA384:
			HashWrapper = new sha384wrapper();
			break;
		case SHA512:
			HashWrapper = new sha512wrapper();
			break;
	}
	Hashtype = Hash;
	FilePath = Path;
	Lock = false;
}

HashBaker::~HashBaker()
{
    delete HashWrapper;
}

void HashBaker::Bake()
{
	thread = async(std::launch::async, [this] {
		Lock = true;
		string result = HashWrapper->getHashFromFile(FilePath);
		Lock = false;
		return result;
    });
}

bool HashBaker::IsCooked()                                           //check if hash is ready. Return bool var.
{
	return Lock ? false : true;
}

string HashBaker::TakeOut()                                         //hash takeout for finished hashes
{
    if(Hash == "")
        Hash = thread.get();
	return Hash;
}
