#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <memory>
#include "canvas.h"
#include "library.h"
#include "structures.h"
#include "boundary.h"
#include "path.h"
#include "sref.h"
#include "aref.h"
#include "techfile.h"

int main(int argc, char *argv[])
{

    std::string file_name = "C:/transistor_hisilicon_28_nm_RF_20150322.xsmc.db";
    std::ifstream in(file_name, std::ifstream::binary);
    GDS::Library gds;
    std::string msg;
    int flag = gds.read(in, msg);
    if (flag == GDS::FILE_ERROR)
    {
        std::cout << "Read file error." << std::endl;
        return 1;
    }
    else if (flag == GDS::FORMAT_ERROR)
    {
        std::cout << msg << std::endl;
        return 1;
    }
    gds.BuildCellLinks();
    gds.CollectLayers(GDS::Techfile::instance());
    std::shared_ptr<GDS::Structure> cell = gds.get("CELL_007");

//    GDS::Library gds;
//    std::shared_ptr<GDS::Boundary> polygon = std::make_shared<GDS::Boundary>();
//    std::vector<GDS::Point> xy = {GDS::Point(100, 100), GDS::Point(200, 100), GDS::Point(200, 200), GDS::Point(100, 200), GDS::Point(100, 100)};
//    polygon->set_xy(xy);
//    polygon->set_layer(100);
//    polygon->set_data_type(0);
//    std::shared_ptr<GDS::Path> path = std::make_shared<GDS::Path>();
//    std::vector<GDS::Point> xy2 = {GDS::Point(100, 100), GDS::Point(150, 100), GDS::Point(150, 150), GDS::Point(200, 150)};
//    path->set_xy(xy2);
//    path->set_layer(101);
//    path->set_data_type(0);
//    path->set_width(10);
//    std::shared_ptr<GDS::ARef> sref = std::make_shared<GDS::ARef>();
//    sref->set_struct_name("test");
//    std::vector<GDS::Point> xy3 = {GDS::Point(0, 0), GDS::Point(0, 0), GDS::Point(0, 0)};
//    sref->set_xy(xy3);
//    sref->set_row_col(1, 1);
//    sref->set_angle(180);
//    sref->set_strans(GDS::REFLECTION, true);
//    std::shared_ptr<GDS::Structure> cell = gds.Add("test");
//    std::shared_ptr<GDS::Structure> cell2 = gds.Add("test2");
//    cell->Add(polygon);
//    cell->Add(path);
//    cell2->Add(sref);
//    gds.CollectLayers(GDS::Techfile::instance());
//    gds.BuildCellLinks();

    QApplication a(argc, argv);
    QMainWindow win;
    win.setCentralWidget(new GDS::Canvas(cell));
    win.show();

    return a.exec();
}
