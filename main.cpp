#include "mainwindow.h"
#include <QApplication>
#include"camera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Camera camera("/home/cvpr/ShanFu/data/1212/intrinsics.yml","/home/cvpr/ShanFu/data/1212/extrinsics.yml");

    return a.exec();
}
