#include "hash.hpp"
using namespace std;

HashBaker::HashBaker(HashType Hash, QString Path)
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
}

HashBaker::~HashBaker()
{
    delete HashWrapper;
}

void HashBaker::Bake()
{
    qDebug("bake here");
    connect(&watcher, SIGNAL(finished()), this, SLOT(CallDinner()));
    thread = QtConcurrent::run(this->HashWrapper,&hashwrapper::getHashFromFile,FilePath.toStdString());
    watcher.setFuture(thread);
}

void HashBaker::CallDinner()
{
    qDebug("dinner's ready");
    hash = QString::fromStdString(thread.result());
    emit Cooked(hash);
}

QString HashBaker::TakeOut()
{
    return hash;
}


/*void TestBaker::process() {
    hashwrapper *wrapper = new md5wrapper();
    QString result = QString::fromStdString(wrapper->getHashFromFile("D:\\Pobrane\\install66.iso"));
    hashes[0] = result.toLatin1();
    qDebug("finished");
    qDebug("1. "+hashes[0].toLatin1());
    emit finished();
}*/
