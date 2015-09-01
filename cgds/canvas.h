#ifndef GDS_CANVAS_H
#define GDS_CANVAS_H
#include <QtWidgets/QGraphicsView>
#include <memory>
namespace GDS
{
class Structure;

class Canvas : public QGraphicsView
{
public:
    Canvas(std::shared_ptr<Structure> cell, QWidget *parent = 0);
    virtual ~Canvas();

private:
    std::shared_ptr<Structure> cell_;
};
}
#endif
