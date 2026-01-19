#ifndef ARMGEOMETRY_H
#define ARMGEOMETRY_H

#include <QObject>
#include <vector>
#include <map>
#include <QDebug> // Pour les logs

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#define BASE 0
#define MEMBRE_1 1
#define MEMBRE_2 2
#define MEMBRE_3 3
#define MEMBRE_4 4

#define FACE_BAS 10
#define FACE_HAUT 11
#define FACE_SUD 12
#define FACE_NORD 13
#define FACE_EST 14
#define FACE_OUEST 15

#define FAILURE_INDEX 999


using namespace std;

class ArmGeometry : public QObject
{
    Q_OBJECT
public:
    ArmGeometry();

signals:
    void anglesCalculated(std::vector<double> angles);
    void SClog(QString message_to_display);

public slots:
    void on_sendCalcResults(std::map<int, cv::Mat> results);
    void calibrateZero();

private:
    //map<int, cv::Mat> _arucoAngles_old;
    std::vector<double> m_offsets = {0.0, 0.0, 0.0, 0.0};
    std::vector<double> m_raw_angles = {0.0, 0.0, 0.0, 0.0};
    vector<double> current_joint_angles;


    map<int, pair<int,int>> whereAreArucos = {{0,{BASE,FACE_SUD}},{1,{BASE,FACE_NORD}},
                                               {2,{MEMBRE_1,FACE_EST}},
                                               {3,{MEMBRE_2, FACE_HAUT}},{4,{MEMBRE_2, FACE_SUD}},{5,{MEMBRE_2, FACE_BAS}},{6,{MEMBRE_2, FACE_NORD}},
                                               {7,{MEMBRE_3, FACE_HAUT}},{8,{MEMBRE_3, FACE_SUD}},{9,{MEMBRE_3, FACE_BAS}},{10,{MEMBRE_3,FACE_NORD}},
                                               {11,{MEMBRE_4, FACE_HAUT}},{12,{MEMBRE_4,FACE_SUD}},{13,{MEMBRE_4,FACE_BAS}},{14,{MEMBRE_4,FACE_NORD}}};


    // Helpers
    cv::Mat getRotationForMember(int memberId, const std::map<int, cv::Mat> &detected_arucos);
    cv::Mat getCorrectionMatrix(int faceId);
    cv::Vec3d rotationMatrixToEulerAngles(cv::Mat &R);
};

#endif // ARMGEOMETRY_H
