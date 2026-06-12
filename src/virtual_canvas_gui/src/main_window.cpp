#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Virtual Canvas - Robot Arm Drawing");
    setFixedSize(900, 550);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Robot Arm Virtual Canvas", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel *instructionLabel = new QLabel(
        "Draw on the canvas below. Coordinates will be mapped to robot arm space:\n"
        "X: -0.4 ~ 0.3, Y: 0.55 (fixed), Z: 0.2 ~ 0.55",
        this);
    instructionLabel->setStyleSheet("color: #666; padding: 5px;");
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    canvas = new CanvasWidget(this);
    mainLayout->addWidget(canvas);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *executeBtn = new QPushButton("Execute (执行)", this);
    executeBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 10px 30px; font-size: 14px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #45a049; }");
    buttonLayout->addWidget(executeBtn);

    QPushButton *clearBtn = new QPushButton("Clear (清空)", this);
    clearBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; "
        "padding: 10px 30px; font-size: 14px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #da190b; }");
    buttonLayout->addWidget(clearBtn);

    mainLayout->addLayout(buttonLayout);

    statusText = new QTextEdit(this);
    statusText->setMaximumHeight(80);
    statusText->setReadOnly(true);
    statusText->setPlaceholderText("Status: Ready...");
    mainLayout->addWidget(statusText);

    canvas->setStatusText(statusText);

    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::onExecuteClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

void MainWindow::onExecuteClicked()
{
    QVector<QPointF> armPoints = canvas->getDrawingPoints();

    if (armPoints.isEmpty()) {
        statusText->append("<span style='color: orange;'>No drawing detected!</span>");
        return;
    }

    statusText->append(QString("<span style='color: green;'>Executing trajectory with %1 points...</span>").arg(armPoints.size()));

    for (int i = 0; i < armPoints.size(); i += 50) {
        QPointF pt = armPoints[i];
        statusText->append(QString("  Point %1: X=%2, Z=%3")
            .arg(i)
            .arg(pt.x(), 0, 'f', 3)
            .arg(pt.y(), 0, 'f', 3));
    }

    if (armPoints.size() > 0) {
        statusText->append(QString("  ... Total %1 points")
            .arg(armPoints.size()));
    }

    statusText->append("<span style='color: blue;'>Note: Publish to ROS Topic '/canvas_trajectory' for arm execution.</span>");
}

void MainWindow::onClearClicked()
{
    canvas->clear();
    statusText->append("<span style='color: gray;'>Canvas cleared.</span>");
}
