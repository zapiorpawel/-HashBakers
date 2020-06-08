#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hash.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HashBaker testhash(HashType::MD5,QString::fromUtf8("D:\\Pobrane\\sad"));
    QObject::connect(&testhash, SIGNAL(Cooked(QString)),this, SLOT(SetLabelText(QString)));
    testhash.Bake();
}

void MainWindow::SetLabelText(QString text)
{
    qDebug(text.toLatin1());
    ui->label1->setText(text);
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

