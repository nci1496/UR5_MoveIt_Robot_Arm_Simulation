#include "canvas_widget.h"

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), statusText(nullptr)
{
    setMinimumSize(800, 400);
    setMaximumSize(800, 400);
    setStyleSheet("background-color: white; border: 1px solid #ccc;");

    coordMapping = {800.0, 400.0, -0.4, 0.3, 0.2, 0.55, 0.55};
}

void CanvasWidget::setCoordMapping(const CoordMapping &mapping)
{
    coordMapping = mapping;
}

QPointF CanvasWidget::screenToArm(const QPoint &screenPos) const
{
    double arm_x = coordMapping.arm_x_min +
                   (screenPos.x() / coordMapping.screen_width) *
                   (coordMapping.arm_x_max - coordMapping.arm_x_min);

    double arm_z = coordMapping.arm_z_min +
                   (screenPos.y() / coordMapping.screen_height) *
                   (coordMapping.arm_z_max - coordMapping.arm_z_min);

    return QPointF(arm_x, arm_z);
}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        currentStroke.clear();
        currentStroke.append(event->pos());
        lastPoint = event->pos();
        update();
    }
}

bool CanvasWidget::shouldAddPoint(const QPoint &newPoint)
{
    // 使用曼哈顿距离作为近似
    int distance = (newPoint - lastPoint).manhattanLength();
    return distance >= MIN_DISTANCE;
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (shouldAddPoint(event->pos())) {
            currentStroke.append(event->pos());
            lastPoint = event->pos();
            update();
        }
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !currentStroke.isEmpty()) {
        allStrokes.append(currentStroke);
        currentStroke.clear();
        emit drawingUpdated();
        if (statusText) {
            statusText->append("Drawing updated.");
        }
    }
}

void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(3);
    painter.setPen(pen);

    for (const QVector<QPoint> &stroke : allStrokes) {
        if (stroke.size() > 1) {
            for (int i = 1; i < stroke.size(); ++i) {
                painter.drawLine(stroke[i-1], stroke[i]);
            }
        }
    }

    if (currentStroke.size() > 1) {
        for (int i = 1; i < currentStroke.size(); ++i) {
            painter.drawLine(currentStroke[i-1], currentStroke[i]);
        }
    }
}

QVector<QPointF> CanvasWidget::getDrawingPoints() const
{
    QVector<QPointF> armPoints;

    for (const QVector<QPoint> &stroke : allStrokes) {
        for (const QPoint &pt : stroke) {
            armPoints.append(screenToArm(pt));
        }
    }

    for (const QPoint &pt : currentStroke) {
        armPoints.append(screenToArm(pt));
    }

    return armPoints;
}

void CanvasWidget::clear()
{
    allStrokes.clear();
    currentStroke.clear();
    update();
}
