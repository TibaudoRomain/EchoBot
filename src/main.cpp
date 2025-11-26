#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <astra/astra.hpp>

#define NUMBER_OF_HORIZONTAL_MARKER 6
#define NUMBER_OF_VERTICAL_MARKER 4

#define MARKER_SIZE 200
#define MARGIN 30

#define MARKER_BORDER_BITS 1

void aruco_grid_gen(){

    int canvasWidth = NUMBER_OF_HORIZONTAL_MARKER * (MARKER_SIZE + 2 * MARGIN);
    int canvasHeight = NUMBER_OF_VERTICAL_MARKER * (MARKER_SIZE + 2 * MARGIN);

    cv::Mat markerImage(canvasHeight, canvasWidth, CV_8UC1, cv::Scalar::all(255));

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    // --- 2. Boucler et dessiner les marqueurs ---
    int markerId = 0; // Commence toujours par l'ID 0

    for (int y = 0; y < NUMBER_OF_VERTICAL_MARKER; y++)
    {
        for (int x = 0; x < NUMBER_OF_HORIZONTAL_MARKER; x++)
        {
            // Calculer le coin supérieur gauche de la *zone* où dessiner
            int startX = x * (MARKER_SIZE + 2 * MARGIN) + MARGIN;
            int startY = y * (MARKER_SIZE + 2 * MARGIN) + MARGIN;

            // Créer une "Région d'Intérêt" (ROI) qui pointe vers le canevas
            cv::Rect roi(startX, startY, MARKER_SIZE, MARKER_SIZE);
            cv::Mat markerRegion = markerImage(roi);

            // Dessiner le marqueur directement dans cette région
            cv::aruco::generateImageMarker(
                dictionary,
                markerId,
                MARKER_SIZE,
                markerRegion,
                MARKER_BORDER_BITS
                );

            markerId++; // Passe à l'ID suivant
        }
    }
    cv::imwrite("ArucoGrid.png", markerImage);
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//ASTRA CAM INIT TEST------------------------------------------------------------------------------------
    bool cam_found = false;
    qDebug() << "--- DEBUT TEST INTEGRATION ASTRA SDK ---";
    try
    {
        astra::initialize();
        qDebug() << ">>> SUCCES : astra::initialize() a reussi.";

        astra::StreamSet streamSet;
        astra::StreamReader reader = streamSet.create_reader();
        astra::DepthStream teststream = reader.stream<astra::DepthStream>();
        teststream.start();
        qDebug() << ">>> Demarrage du flux de test...";
        astra::Frame frame = reader.get_latest_frame(5000);
        if (frame.is_valid()){
            cam_found = true;
            qDebug() << ">>> SUCCES : Caméra RGB-D trouvée et valide.";
        }
        else{
            qDebug() << ">>> ERROR : Pas de caméra RGB-D valide.";
        }
        teststream.stop();
        astra::terminate();
        qDebug() << ">>> SUCCES : astra::terminate() a reussi.";
    }
    catch (const std::exception& e)
    {
        qWarning() << "!!! INFO : Camera non detectee ou erreur SDK.";
        qWarning() << "    Message d'erreur : " << e.what();
    }

    if (cam_found)
    {
        qDebug() << ">>> RESULTAT : Une camera Astra est connectee et prete.";
    }
    else
    {
        qDebug() << ">>> RESULTAT : Aucune camera Astra n'a ete trouvee.";
    }

    qDebug() << "--- FIN TEST INTEGRATION ASTRA SDK ---";
//END ASTRA CAM INIT TEST--------------------------------------------------------------------------------

    //marker image creation
    aruco_grid_gen();

    MainWindow w;
    w.show();
    return a.exec();
}

