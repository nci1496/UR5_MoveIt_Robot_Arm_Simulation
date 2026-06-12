#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QVector>
#include <QPointF>
#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>
#include <QGroupBox>

// ROS headers
#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>

#include "canvas_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onExecuteClicked();
    void onClearClicked();
    void onSamplingChanged(int value);
    void onMirrorChanged(bool checked);
    void onMirrorZChanged(bool checked);
    void onSpeedChanged(int value);

private:
    void publishTrajectory();
    void updateStatus(const QString& message);

    CanvasWidget *canvas;
    QTextEdit *statusText;
    ros::NodeHandle nh_;
    ros::Publisher trajectory_pub_;

    // Settings
    QSpinBox *samplingSpinBox;
    QCheckBox *mirrorCheckBox;
    QCheckBox *mirrorZCheckBox;
    QSlider *speedSlider;
    QLabel *speedLabel;
};

#endif
