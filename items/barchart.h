#ifndef BARCHART_H
#define BARCHART_H

#include <QQuickPaintedItem>
#include <QPen>
#include <QFont>

#include "wordstatistics.h"

// Класс для отрисовки гистограммы

class BarChart : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(WordStatistics *wordStatistics READ wordStatistics WRITE setWordStatistics NOTIFY wordStatisticsChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)

public:
    explicit BarChart(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    WordStatistics *wordStatistics() const;
    void setWordStatistics(WordStatistics *value);

    QFont font() const;
    void setFont(const QFont &font);

private slots:
    void updateBar();

signals:
    void fontChanged();
    void wordStatisticsChanged();

private:
    WordStatistics *wordStatistics_;

    QFont font_;

    QPen labelsPen_;
    QPen gridPen_;
    QPen axisPen_;

    QColor getColor(qreal value) const;
};


#endif // BARCHART_H
