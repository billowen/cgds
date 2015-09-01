#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include "canvas.h"
#include "library.h"
#include "structures.h"

int main(int argc, char *argv[])
{

    std::string file_name = "transistor_hisilicon_28_nm_RF_20150322.xsmc.db";
    std::ifstream in(file_name, std::ifstream::binary);
    GDS::Library gds;
    std::string msg;
    if (!gds.read(in, msg))
    {
        std::cout << msg << std::endl;
        return 1;
    }
    QApplication a(argc, argv);
    GDS::Canvas canvas(gds.get("105lvtnmos1_001"));
    QMainWindow win;
    win.setCentralWidget(&canvas);
    win.show();

    return a.exec();
}
