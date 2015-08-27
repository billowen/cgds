#include <QtWidgets/QGraphicsItem>
#include <QRectF>
#include <memory>
#include "tags.h"

namespace GDS
{
class Element;

class ViewItem : public QGraphicsItem
{
public:
    ViewItem(std::shared_ptr<Element> data, QGraphicsItem *parent = 0);
    virtual ~ViewItem() {}

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    Record_type type() const;
    std::shared_ptr<Element> data() const;

private:
    std::weak_ptr<Element> Data;
};

}
