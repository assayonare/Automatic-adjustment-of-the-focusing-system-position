#include <QString>
#include <QApplication>
#include <QDir>
#include "mainwindow.h"
#include "MotionSystem/motionsystem.h"
//#include "imageproc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Imageproc i;

//    i.f_change = 5;
//    double score_image = 0;
//   // bool optimal_f = false;

//    QDirIterator it("D:\\OpenCV_Project\\photo_set2\\");
//    int count_dir = 0;
//    while (it.hasNext()){
//        count_dir += 1;
//        QString dir = it.next();
//        if (count_dir > 2){
//        i.setImageName(dir.toStdString());

//        qDebug() << dir;
//        if (i.blur_score(i.image) > score_image){

//            score_image = i.blur_score(i.image);
//            i.f_change = 5;
//        }
//        else if(i.blur_score(i.image) < score_image){
//               i.f_change = -5;

//        }
//        else{
//               break;
//        }
//        qDebug() << "Final blur score:" << score_image; //i.blur_score(i.image);



//        }
//    }
//    //qDebug() << "lumiance_score:" << i.lumiance_score(i.image);

    MainWindow w;
    w.show();

    //MotionSystem m;

    //cv::imshow("image", i.image);
    //cv::waitKey(0);

    return a.exec();
}
