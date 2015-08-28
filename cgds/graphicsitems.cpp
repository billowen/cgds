#include <assert.h>
#include <vector>
#include <QtCore/QPoint>
#include <QtGui/QPolygon>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include "techfile.h"
#include "structures.h"
#include "boundary.h"
#include "path.h"
#include "aref.h"
#include "sref.h"
#include "graphicsitems.h"

namespace GDS
{
void PaintPolygon(QPainter &painter, std::shared_ptr<Element> data);
void PaintPath(QPainter &painter, std::shared_ptr<Element> data);
void PaintSRef(QPainter &painter, std::shared_ptr<Element> data, int level = -1);
void PaintARef(QPainter &painter, std::shared_ptr<Element> data, int level = -1);
void PaintCell(QPainter &painter, std::shared_ptr<Structure> cell,
               int level = -1,
               int offset_x = 0, int offset_y = 0,
               double mag = 1.0, double angle = 0.0,
               bool reflect = false);
void InitPainter(QPainter &painter, short layer, short purpose);


ViewItem::ViewItem(std::shared_ptr<Element> data, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    data_ = data;
    setFlags(ItemIsSelectable);
}

QRectF ViewItem::boundingRect() const
{
    assert(!Data.expired());
    int x, y, w, h;
    auto entity = Data.lock();
    if (entity->bbox(x, y, w, h))
        return QRectF(x, y, w, h);
    else
        return QRectF();
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(tag())
    {
    case BOUNDARY:
        PaintPolygon(*painter, data_.lock());
        break;
    case PATH:
        PaintPath(*painter, data_.lock());
        break;
    case SREF:
        PaintSRef(*painter, data_.lock());
        break;
    case AREF:
        PaintARef(*painter, data_.lock());
        break;
    default:
        break;
    }
}

Record_type ViewItem::tag() const
{
    assert(!data_.expired());
    return (data_.lock())->tag();
}

std::shared_ptr<Element> ViewItem::data()
{
    return data_.lock();
}

void PaintPolygon(QPainter &painter, std::shared_ptr<Element> data)
{
    auto polygon = std::dynamic_pointer_cast<Boundary>(data);
    if (!polygon)
        return;
    InitPainter(painter, polygon->layer(), polygon->dataType());
    QPolygon tmp;
    for (Point pt : polygon->xy())
        tmp << QPoint(pt.x, pt.y);
    painter.drawPolygon(tmp);
}

void PaintPath(QPainter &painter, std::shared_ptr<Element> data)
{
    auto path = std::dynamic_pointer_cast<Path>(data);
    if (!path)
        return;
    InitPainter(painter, path->layer(), path->dataType());
    auto pts = path->xy();
    if (pts.size() < 2)
        return;
    QPainterPath tmp;
    tmp.moveTo(pts[0].x, pts[0].y);
    for (size_t i = 1; i < pts.size(); i++)
        tmp.lineTo(pts[i].x, pts[i].y);
    QPainterPathStroker stroker;
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::FlatCap);
    stroker.setWidth(path->width());
    painter.drawPath(stroker.createStroke(tmp));
}

void PaintSRef(QPainter &painter, std::shared_ptr<Element> data, int level)
{
    auto sref = std::dynamic_pointer_cast<SRef>(data);
    if (!sref)
        return;
    PaintCell(painter, sref->reference(), level,
              (sref->xy()).x, (sref->xy()).y,
              sref->mag(), sref->angle(), sref->stransFlag(REFLECTION));
}

void PaintARef(QPainter &painter, std::shared_ptr<Element> data, int level)
{
    auto aref = std::dynamic_pointer_cast<ARef>(data);
    if (!aref)
        return;
    auto pts = aref->xy();
    int row_pitch_x = (pts[2].x - pts[0].x) / aref->row();
    int row_pitch_y = (pts[2].y - pts[0].y) / aref->row();
    int col_pitch_x = (pts[1].x - pts[0].x) / aref->col();
    int col_pitch_y = (pts[1].y - pts[0].y) / aref->col();
    for (int i = 0; i < aref->row(); i++)
    {
        int row_offset_x = pts[0].x + i * row_pitch_x;
        int row_offset_y = pts[0].y + i * row_pitch_y;
        for (int j = 0; j < aref->col(); j++)
        {
            int cur_x = row_offset_x + j * col_pitch_x;
            int cur_y = row_offset_y + j * col_pitch_y;
            PaintCell(painter, aref->reference(), level,
                      cur_x, cur_y,
                      aref->mag(), aref->angle(), aref->stransFlag(REFLECTION));
        }
    }
}

void PaintCell(QPainter &painter, std::shared_ptr<Structure> cell, int level, int offset_x, int offset_y, double mag, double angle, bool reflect)
{
    QTransform back = painter.transform();
    QTransform reflect_transform, mag_transform, rotate_transform, shift_transform;
    if(reflect)
        reflect_transform.scale(1, -1);
    mag_transform.scale(mag, mag);
    rotate_transform.rotate(angle);
    shift_transform.translate(offset_x, offset_y);

    QTransform transform = reflect_transform * mag_transform * rotate_transform * shift_transform;
    painter.setTransform(transform, true);

    if(level < 0)
        level = 99;
    if (level == 0)
    {
        int x, y, w, h;
        cell->bbox(x, y, w, h);
        QRectF rect(x, y, w, h);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, QString(cell->name().c_str()));
    }
    else
    {
        for (size_t i = 0; i < cell->size(); i++)
        {
            std::shared_ptr<Element> node = cell->get(i);
            switch (node->tag())
            {
            case BOUNDARY:
                PaintPolygon(painter, node);
                break;
            case PATH:
                PaintPath(painter, node);
                break;
            case AREF:
                PaintARef(painter, node, level - 1);
                break;
            case SREF:
                PaintSRef(painter, node, level - 1);
                break;
            default:
                break;
            }
        }
    }
    painter.setTransform(back);
}

void InitPainter(QPainter &painter, short layer, short purpose)
{
    Techfile& techfile = Techfile::instance();
    char r, g, b;
    bool flag;
    flag = techfile.GetLayerColor(layer, purpose, r, g, b);
    assert(flag);
    std::string stipple_name;
    StipplePattern stipple_pattern;
    flag = techfile.GetLayerStippleName(layer, purpose, stipple_name);
    assert(flag);
    flag = techfile.GetLayerStipplePattern(layer, purpose, stipple_pattern);
    assert(flag);
    painter.setPen(QColor(r, g, b));
    QBrush brush;
    brush.setColor(QColor(r, g, b));
    brush.setStyle(kBuildInStipple[stipple_name]);
    painter.setBrush(brush);
}

}
