#ifndef ARMGEOMETRY_H
#define ARMGEOMETRY_H

#include <QObject>
#include <vector>
#include <map>
#include <cmath>
#include <QDebug> // Pour les logs

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#define BASE 0
#define MEMBRE_1 1
#define MEMBRE_2 2
#define MEMBRE_3 3
#define MEMBRE_4 4


using namespace std;

class ArmGeometry : public QObject
{
    Q_OBJECT
public:
    ArmGeometry();

signals:
    void anglesCalculated(std::vector<double> angles);

public slots:
    void on_sendCalcResults(std::map<int, cv::Mat> results);

private:
    //map<int, cv::Mat> _arucoAngles_old;
    vector<double> current_joint_angles;
    map<int, int> whereAreArucos = {{0,BASE},{1,BASE},
                                    {2,MEMBRE_1},
                                    {3,MEMBRE_2},{4,MEMBRE_2},{5,MEMBRE_2},{6,MEMBRE_2},
                                    {7,MEMBRE_3},{8,MEMBRE_3},{9,MEMBRE_3},{10,MEMBRE_3},
                                    {11,MEMBRE_4},{12,MEMBRE_4},{13,MEMBRE_4},{14,MEMBRE_4}};
    // Helpers
    cv::Mat getRotationForMember(int memberId, const std::map<int, cv::Mat> &detected_arucos);
    cv::Vec3d rotationMatrixToEulerAngles(cv::Mat &R);
};

#endif // ARMGEOMETRY_H
