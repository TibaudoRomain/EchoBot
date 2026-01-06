/*#ifndef ARMGEOMETRY_H
#define ARMGEOMETRY_H

#include <QObject>
#include <vector>
#include <map>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace std;

class ArmGeometry
{
    Q_OBJECT
public:
    ArmGeometry();

signals:


public slots:
    void on_aruco_found();

private:
    map<int, cv::Vec3d> aruco_angles;
    map<int, bool> whosfound;
    vector<int> motor_angles;

    void update_aruco_angles();
    void find_motor_angles();
};

#endif // ARMGEOMETRY_H*/
