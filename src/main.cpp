#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <astra/astra.hpp>

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


    MainWindow w;
    w.show();
    return a.exec();
}
