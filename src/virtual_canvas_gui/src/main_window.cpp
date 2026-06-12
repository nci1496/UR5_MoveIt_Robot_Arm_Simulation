#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize ROS publisher
    trajectory_pub_ = nh_.advertise<geometry_msgs::PoseArray>(
        "/canvas_trajectory", 10, true);

    setWindowTitle("Virtual Canvas - Robot Arm Drawing");
    setFixedSize(900, 550);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *titleLabel = new QLabel("Robot Arm Virtual Canvas", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Instructions
    QLabel *instructionLabel = new QLabel(
        "Draw on the canvas below. Coordinates will be mapped to robot arm space:\n"
        "X: -0.4 ~ 0.3, Y: 0.55 (fixed), Z: 0.2 ~ 0.55",
        this);
    instructionLabel->setStyleSheet("color: #666; padding: 5px;");
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    // Canvas
    canvas = new CanvasWidget(this);
    mainLayout->addWidget(canvas);

    // Buttons
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

    // Status text
    statusText = new QTextEdit(this);
    statusText->setMaximumHeight(80);
    statusText->setReadOnly(true);
    statusText->setPlaceholderText("Status: Ready...");
    mainLayout->addWidget(statusText);

    canvas->setStatusText(statusText);

    // Connect signals
    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::onExecuteClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    updateStatus("Ready. Draw on canvas and click Execute.");
}

MainWindow::~MainWindow()
{
    // ROS will be cleaned up automatically
}

void MainWindow::updateStatus(const QString& message)
{
    statusText->append(message);
}

void MainWindow::onExecuteClicked()
{
    QVector<QPointF> armPoints = canvas->getDrawingPoints();

    if (armPoints.isEmpty()) {
        updateStatus("<span style='color: orange;'>No drawing detected! Draw something first.</span>");
        return;
    }

    updateStatus(QString("<span style='color: green;'>Publishing trajectory with %1 points...</span>")
        .arg(armPoints.size()));

    // Create PoseArray message
    geometry_msgs::PoseArray trajectory_msg;
    trajectory_msg.header.stamp = ros::Time::now();
    trajectory_msg.header.frame_id = "base_link";

    // Get current orientation for consistency
    // For simplicity, we'll use a default orientation
    trajectory_msg.poses.resize(armPoints.size());

    for (int i = 0; i < armPoints.size(); ++i) {
        trajectory_msg.poses[i].position.x = armPoints[i].x();       // arm_x
        trajectory_msg.poses[i].position.y = 0.55;                   // arm_y (fixed)
        trajectory_msg.poses[i].position.z = armPoints[i].y();       // arm_z
        trajectory_msg.poses[i].orientation.x = 1.0;
        trajectory_msg.poses[i].orientation.y = 0.0;
        trajectory_msg.poses[i].orientation.z = 0.0;
        trajectory_msg.poses[i].orientation.w = 0.0;
    }

    // Publish trajectory
    trajectory_pub_.publish(trajectory_msg);

    updateStatus("<span style='color: blue;'>Trajectory published to /canvas_trajectory!</span>");
    updateStatus("<span style='color: gray;'>Run 'rosrun canvas_executor canvas_executor' to execute.</span>");

    // Also log to console
    ROS_INFO("Published trajectory with %d points", armPoints.size());
}

void MainWindow::onClearClicked()
{
    canvas->clear();
    updateStatus("<span style='color: gray;'>Canvas cleared.</span>");
}
