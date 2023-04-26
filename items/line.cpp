#include "line.h"
#include <QtMath>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QQuickWindow>
#include <QRectF>

Line::Line(QQuickItem *parent) : QQuickItem(parent),
    color_(Qt::black),
    spacing_(0),
    dashSize_(-1),
    orientation_(Qt::Horizontal),
    lineWidth_(1)
{
    setFlag(ItemHasContents, true);

    if(orientation_ == Qt::Horizontal)
        setHeight(1);
    else
        setWidth(1);
}

QColor Line::color() const
{
    return color_;
}

void Line::setColor(const QColor &color)
{
    if(color_ != color) {
        color_ = color;
        emit colorChanged();
        update();
    }
}

qreal Line::spacing() const
{
    return spacing_;
}

void Line::setSpacing(const qreal &spacing)
{
    if(spacing_ != spacing) {
        spacing_ = spacing;
        emit spacingChanged();
        update();
    }
}

qreal Line::dashSize() const
{
    return dashSize_;
}

void Line::setDashSize(const qreal &dashSize)
{
    if(dashSize_ != dashSize) {
        dashSize_ = dashSize;
        emit dashSizeChanged();
        update();
    }
}

Qt::Orientation Line::orientation() const
{
    return orientation_;
}

void Line::setOrientation(const Qt::Orientation &orientation)
{
    if(orientation_ != orientation) {
        orientation_ = orientation;
        emit orientationChanged();
        update();
    }
}

qreal Line::lineWidth() const
{
    return lineWidth_;
}

void Line::setLineWidth(const qreal &lineWidth)
{
    if(lineWidth_ != lineWidth) {
        lineWidth_ = lineWidth;
        emit lineWidthChanged();
        update();
    }
}

QSGNode *Line::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData)

    QSGGeometryNode *node = static_cast <QSGGeometryNode *> (oldNode);
    QRectF rect = boundingRect();

    // Если при обновлении нода не существует, то необходимо создать все объекты и прикрепить их к ноде
    if (!node){
        node = new QSGGeometryNode();

        QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);

#if QT_VERSION < QT_VERSION_CHECK(5, 8, 0)
        g->setDrawingMode(GL_LINES);        //@N проверить..
#else
        g->setDrawingMode(QSGGeometry::DrawLines);
#endif

        g->setLineWidth(realLineWidth());

        node->setGeometry(g);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *m = new QSGFlatColorMaterial;
        m->setColor(color_);
        node->setMaterial(m);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else {
        node->geometry()->setLineWidth(realLineWidth());
        QSGFlatColorMaterial *m = static_cast <QSGFlatColorMaterial *> (node->material());
        m->setColor(color_);
    }

    int count = 1;
    if(spacing_ > 0 && dashSize_ > 0)
        count = orientation_ == Qt::Horizontal ? qMax(1, qCeil(qAbs(rect.width() / (spacing_ + dashSize_))))
                                               : qMax(1, qCeil(qAbs(rect.height() / (spacing_ + dashSize_))));

    node->geometry()->allocate(count * 2);

    QSGGeometry::Point2D *vertexes = node->geometry()->vertexDataAsPoint2D();

    if(orientation_ == Qt::Horizontal) {
        if(count == 1) {
            vertexes[0].set(0, rect.height() * 0.5);
            vertexes[1].set(rect.width(), rect.height() * 0.5);
        }
        else {

            float x = 0;
            for(int i = 0; i < 2 * count; i += 2) {
                vertexes[i].set(x, rect.height() * 0.5);
                x += dashSize_;
                if(x > rect.width()) x = rect.width();  // чтобы не вышли за пределы..

                vertexes[i + 1].set(x, rect.height() * 0.5);
                x += spacing_;
            }
        }

    }   // orientation
    else {
        if(count == 1) {
            vertexes[0].set(rect.width() * 0.5, 0);
            vertexes[1].set(rect.width() * 0.5, rect.height());
        }
        else {
            float y = 0;
            for(int i = 0; i < 2 * count; i += 2) {
                vertexes[i].set(rect.width() * 0.5, y);
                y += dashSize_;
                if(y > rect.height()) y = rect.height();  // чтобы не вышли за пределы..
                vertexes[i + 1].set(rect.width() * 0.5, y);
                y += spacing_;
            }
        }
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

qreal Line::realLineWidth() const
{
    return orientation_ == Qt::Horizontal ? std::min(lineWidth_, height())
                                          : std::min(lineWidth_, width());
}
