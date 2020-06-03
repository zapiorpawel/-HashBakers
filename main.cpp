#include "hashlibpp/src/hashlibpp.h"
#include "hash.hpp"
#include "tools.hpp"
#include <iostream>
using namespace std;

void PrintHelp()
{
	cout << "hasherino v0.69" << endl;
	cout << "needs a file argument" << endl;
	return;
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		PrintHelp();
		return -1;
	}
	string FilePath = argv[1];
	string Hashes[5] = {"","","","",""};
	int check;
	HashBaker MD5Oven(HashType::MD5,FilePath);
	HashBaker SHA1Oven(HashType::SHA1,FilePath);
	HashBaker SHA256Oven(HashType::SHA256,FilePath);
	HashBaker SHA384Oven(HashType::SHA384,FilePath);
	HashBaker SHA512Oven(HashType::SHA512,FilePath);
	MD5Oven.Bake();
	SHA1Oven.Bake();
	SHA256Oven.Bake();
	SHA384Oven.Bake();
	SHA512Oven.Bake();
	while(true)
	{
		check = 0;
		this_thread::sleep_for(chrono::milliseconds(100));
		if(!MD5Oven.IsCooked())
			cout << "MD5: " << GenerateRandomString(HashType::MD5) << "\n";
		else
		{
			check++;
			if(Hashes[0] == "")
				Hashes[0] = MD5Oven.TakeOut();
			cout << "MD5: " << Hashes[0] << "\n";
		}
		if(!SHA1Oven.IsCooked())
			cout << "SHA1: " << GenerateRandomString(HashType::SHA1) << endl;
		else
		{
			check++;
			if(Hashes[1] == "")
				Hashes[1] = SHA1Oven.TakeOut();
			cout << "SHA1: " << Hashes[1] << "\n";
		}
		if(!SHA256Oven.IsCooked())
			cout << "SHA256: " << GenerateRandomString(HashType::SHA256) << endl;
		else
		{
			check++;
			if(Hashes[2] == "")
				Hashes[2] = SHA256Oven.TakeOut();
			cout << "SHA256: " << Hashes[2] << "\n";
		}
		if(!SHA384Oven.IsCooked())
			cout << "SHA384: " << GenerateRandomString(HashType::SHA384) << endl;
		else
		{
			check++;
			if(Hashes[3] == "")
				Hashes[3] = SHA384Oven.TakeOut();
			cout << "SHA384: " << Hashes[3] << "\n";
		}
		if(!SHA512Oven.IsCooked())
			cout << "SHA512: " << GenerateRandomString(HashType::SHA512) << endl;
		else
		{
			check++;
			if(Hashes[4] == "")
				Hashes[4] = SHA512Oven.TakeOut();
			cout << "SHA512: " << Hashes[4] << "\n";
		}
		if(check == 5)
			break;
		cout << "\033[F\033[F\033[F\033[F\033[F";
	}
	return 0;
}
