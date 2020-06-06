#include "tools.hpp"
#include <random>

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
