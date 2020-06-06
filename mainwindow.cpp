#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QThread* thread = new QThread;
    TestBaker* worker = new TestBaker();
    worker->moveToThread(thread);
    //QObject::connect(worker, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    connect(thread, SIGNAL (started()), worker, SLOT (process()));
    connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
    connect(thread, SIGNAL (finished()), this, SLOT (SetLabelText()));
    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    thread->start();
}

void MainWindow::SetLabelText()
{
    ui->label1->setText("text");
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

