#include "barchart.h"
#include <QPainter>

#include <QColor>
#include <QDebug>

#include <QtMath>

BarChart::BarChart(QQuickItem *parent) : QQuickPaintedItem(parent),
    wordStatistics_(nullptr)
{
    setFlag(ItemHasContents, true);

    gridPen_ = QPen(Qt::DotLine);
    gridPen_.setColor(QColor("#A0A0A0"));

    axisPen_ = QPen(Qt::SolidLine);
    axisPen_.setColor(Qt::black);

    labelsPen_ = QPen(Qt::SolidLine);
    labelsPen_.setColor(Qt::black);
}

// Для поиска самого широкого прямоугольника
auto rectCompare = [](const QRect &l, const QRect &r) {
    return l.width() < r.width();
};

void BarChart::paint(QPainter *painter)
{
    if(wordStatistics_ == nullptr)
        return;

    const auto data = wordStatistics_->wordList();

    if(!data.isEmpty()) {
        const int max = std::max_element(data.begin(), data.end())->count;

        if(max > 0) {
            painter->setFont(font_);
            QFontMetrics fontMetrics(font_);

            const qreal spacing = font_.pixelSize() * 0.5;  // Отступ у текста


            // Количество тиков на оси Y в зависимости от максимального значения
            const int tickCount = max >= 100 ? 5
                                             : max >= 10 ? 4
                                                         : max > 1 ? 3
                                                                   : 2;

            const qreal interval = max / static_cast<qreal>(tickCount - 1);

            // расчитаем высоту и ширину текста по оси Y

            QList <QRect> xLabelRects;
            QList <QRect> yLabelRects;

            for(auto &d : data) {
                xLabelRects.append(fontMetrics.boundingRect(d.text));
            }

            for(int i = 0; i < tickCount; ++i){
                int value = std::floor(interval * i);
                yLabelRects.append(fontMetrics.boundingRect(QString::number(value)));
            }

            int yMaxLabelWidth = std::max_element(yLabelRects.begin(), yLabelRects.end(), rectCompare)->width();    // Поиска самого широкого прямоугольника
            int xMaxLabelWidth = std::max_element(xLabelRects.begin(), xLabelRects.end(), rectCompare)->width();    // Поиска самого широкого прямоугольника

            QMarginsF margins(yMaxLabelWidth + spacing, spacing, spacing, xMaxLabelWidth + spacing);
            auto plotArea = boundingRect().marginsRemoved(margins);     // Размер области где отрисовывается непосредственно гистграмма

            qreal barWidth = plotArea.width() / data.size();    // Ширина одного элемента
            qreal barMargin = barWidth * 0.1;                   // Сделаем его чуть меньше. Чтобы был отступ между ними

            // Отрисока линий и текста на оси Y
            for(int i = 0; i < tickCount; ++i) {
                int value = std::floor(interval * i);
                qreal ny = value / static_cast<qreal>(max); // нормированное значение
                QPointF p1(plotArea.x() - yLabelRects.at(i).width() - spacing, plotArea.y() + plotArea.height() * (1 - ny) + yLabelRects.at(i).height() * 0.5);

                painter->setPen(labelsPen_);
                painter->drawText(p1, QString::number(value));

                if(i == 0) {
                    painter->setPen(axisPen_);
                }
                else {
                    painter->setPen(gridPen_);
                }

                QPointF lineStart = QPointF(plotArea.x(), plotArea.y() + plotArea.height() * (1 - ny));
                QPointF lineEnd(plotArea.x() + plotArea.width(), plotArea.y() + plotArea.height() * (1 - ny));

                painter->drawLine(lineStart, lineEnd);
            }

            painter->setPen(axisPen_);

            // Ось y выделим
            painter->drawLine(QPointF(plotArea.x(), plotArea.y()),
                              QPointF(plotArea.x(), plotArea.y() + plotArea.height()));

            painter->setPen(labelsPen_);

            for(int i = 0; i < data.size(); ++i) {
                qreal ny = data.at(i).count / static_cast<qreal>(max); // нормированное значение
                auto barColor = getColor(ny);

                auto rect = QRectF(barMargin + plotArea.x() + barWidth * i,
                                   plotArea.y() + (plotArea.height() * (1 - ny)),
                                   barWidth - 2 * barMargin,
                                   plotArea.height() * ny);
                painter->fillRect(rect, barColor);

                // Отрисовка текста на оси X
                painter->save();
                QPointF p1(plotArea.x() + barWidth * (i + 0.5), plotArea.y() + plotArea.height());
                painter->translate(p1.x(), p1.y());
                painter->rotate(-90);
                painter->drawText(QPointF(-xLabelRects.at(i).width() - spacing, xLabelRects.at(i).height() * 0.5), data.at(i).text);
                painter->restore();
            }
        }
    }
}

WordStatistics *BarChart::wordStatistics() const
{
    return wordStatistics_;
}

void BarChart::setWordStatistics(WordStatistics *value)
{
    wordStatistics_ = value;
    if(wordStatistics_ != nullptr) {
        update();
        QObject::connect(wordStatistics_, &WordStatistics::wordListChanged, this, &BarChart::updateBar);
    }
    emit wordStatisticsChanged();
}

void BarChart::updateBar()
{
    update();
}

QFont BarChart::font() const
{
    return font_;
}

void BarChart::setFont(const QFont &font)
{
    font_ = font;
    emit fontChanged();
}

QColor BarChart::getColor(qreal value) const
{
    // Сделаем градиент в зависимости от текущего значения
    static QColor min("#B30010");
    static QColor max("#007DFF");
    return QColor(min.red() + (max.red() - min.red()) * value,
                  min.green() + (max.green() - min.green()) * value,
                  min.blue() + (max.blue() - min.blue()) * value);
}
