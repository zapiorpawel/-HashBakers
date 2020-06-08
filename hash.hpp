#ifndef HASH_HPP
#define HASH_HPP

#include "hashlibpp/src/hashlibpp.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QDebug>
#include <QString>
/*#include <future>
#include <thread>
//#include <string>*/

//zamień na połączenie e.g HashClass->HashMD5
enum HashType
{
	MD5,
	SHA1,
	SHA256,
	SHA384,
	SHA512
};

class HashBaker : public QObject
{
    Q_OBJECT
private:
    hashwrapper *HashWrapper;
    QString hash;
    QString FilePath;
    QFuture<std::string> thread;
    QFutureWatcher<std::string> watcher;
public:
    HashBaker(HashType Hash, QString Path);
    ~HashBaker();
    void Bake();
    bool IsCooked();
    QString TakeOut();
signals:
    void Cooked(QString hash);
private Q_SLOTS:
    void CallDinner();
};



static QString hashes[5];

#endif
