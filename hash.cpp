#include "hash.hpp"
using namespace std;

HashBaker::HashBaker(HashType Hash, string Path)
{
	switch(Hash)
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
	FilePath = Path;
	Lock = false;
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

bool HashBaker::IsCooked()
{
	return Lock ? false : true;
}

string HashBaker::TakeOut()
{
    if(Hash == "")
        Hash = thread.get();
	return Hash;
}
