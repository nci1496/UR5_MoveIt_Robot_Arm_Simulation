#include <QApplication>
#include <ros/ros.h>
#include "main_window.h"

int main(int argc, char *argv[])
{
    // Initialize ROS first
    ros::init(argc, argv, "virtual_canvas_gui", ros::init_options::NoSigintHandler);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
