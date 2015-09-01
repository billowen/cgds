#include "canvas.h"
#include "structures.h"
#include "graphicsitems.h"

namespace GDS
{
Canvas::Canvas(std::shared_ptr<Structure> cell, QWidget *parent)
    : QGraphicsView(parent)
{
    cell_ = cell;
    setScene(new QGraphicsScene());
    scale(1, -1);
    for (size_t i = 0; i < cell_->size(); i++)
    {
        scene()->addItem(new ViewItem(cell_->get(i)));
    }
}

Canvas::~Canvas()
{

}


}
