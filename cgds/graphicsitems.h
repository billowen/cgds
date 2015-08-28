#ifndef GDS_GRAPHICS_ITEMS_H
#define GDS_GRAPHICS_ITEMS_H

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

    Record_type tag() const;
    std::shared_ptr<Element> data();

private:
    std::weak_ptr<Element> data_;
};

}

#endif
