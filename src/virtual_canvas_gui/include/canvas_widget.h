#ifndef CANVAS_WIDGET_H
#define CANVAS_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>
#include <QPointF>
#include <QTextEdit>

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    ~CanvasWidget() = default;

    struct CoordMapping {
        double screen_width;
        double screen_height;
        double arm_x_min;
        double arm_x_max;
        double arm_z_min;
        double arm_z_max;
        double arm_y_fixed;
    };

    void setCoordMapping(const CoordMapping &mapping);
    QVector<QPointF> getDrawingPoints() const;
    void clear();
    void setStatusText(QTextEdit *text) { statusText = text; }

signals:
    void drawingUpdated();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPoint> currentStroke;
    QVector<QVector<QPoint>> allStrokes;
    CoordMapping coordMapping;
    QTextEdit *statusText;

    QPointF screenToArm(const QPoint &screenPos) const;
};

#endif
