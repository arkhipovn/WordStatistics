#ifndef LINE_H
#define LINE_H

#include <QQuickItem>

// Класс для отрисовки линии

class Line : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(qreal dashSize READ dashSize WRITE setDashSize NOTIFY dashSizeChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    explicit Line(QQuickItem *parent = nullptr);

    QColor color() const;
    void setColor(const QColor &color);

    qreal spacing() const;
    void setSpacing(const qreal &spacing);

    qreal dashSize() const;
    void setDashSize(const qreal &dashSize);

    Qt::Orientation orientation() const;
    void setOrientation(const Qt::Orientation &orientation);

    qreal lineWidth() const;
    void setLineWidth(const qreal &lineWidth);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

signals:
    void colorChanged();
    void spacingChanged();
    void dashSizeChanged();
    void lineWidthChanged();
    void orientationChanged();

private:
    QColor color_;

    qreal spacing_;
    qreal dashSize_;

    Qt::Orientation orientation_;

    qreal lineWidth_;

    qreal realLineWidth() const;
};

#endif // CLINE_H



