#ifndef HASH_HPP
#define HASH_HPP

#include "hashlibpp/src/hashlibpp.h"
#include <QThread>
#include <QDebug>
/*#include <future>
#include <thread>
//#include <string>*/
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
/*
class HashBaker
{
private:
    hashwrapper *HashWrapper;
    string FilePath;
	future<string> thread;
	bool Lock;
public:
    HashBaker(HashType Hash, string Path);
    void Bake();
    bool IsCooked();
    string TakeOut();
};*/

class TestBaker : public QObject {
    Q_OBJECT
public:
    TestBaker();
    ~TestBaker();
public slots:
    void process();
signals:
    void finished();
    void error(QString err);
};

#endif
