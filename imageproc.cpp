#include "imageproc.h"

Imageproc::Imageproc(QObject *parent)
    : QObject{parent}
{
    connect(this, &Imageproc::signalImageProcOut, this, &Imageproc::slotFocusChange);
}

void Imageproc::setImageName(std::string name_image)
{
    image = cv::imread(name_image, cv:: IMREAD_COLOR);
}

double Imageproc::blur_score(cv::Mat image)
{
    cv::Mat src_gray;

    //GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
    cvtColor(image, src_gray, cv:: COLOR_BGR2GRAY);

    cv::Mat dest;
    int kernel_size = 3; // Размер ядра оператора Собеля
    int scale = 1;
    int delta = 0;
    int ddepth = CV_64F; // Глубина конечного изображения

    // Производим преобразование Лапласа
    Laplacian(src_gray, dest, ddepth, kernel_size, scale, delta);
    //Mat abs_dest;
    //convertScaleAbs(dest, abs_dest);

    //imshow("test1", abs_dest);


    //imshow("as",dest);
    //waitKey(0);

    cv::Mat mean, stddev;

    // Находим среднеквадратичное отклонение
    meanStdDev(dest, mean, stddev);
    auto res = stddev.at<double>(0);

    emit signalImageProcOut(f_change);

    return res;
}

//double Imageproc::lumiance_score(cv::Mat image)
//{
//    cv::Mat src_lum;

//    //Преобразуем в цветовое пространство градаций серого

//    cvtColor(image, src_lum, cv:: COLOR_BGR2GRAY);
//    cv::Mat mean, stddev2;

//    //Находим среднее значение яркости по всем пикселям

//    meanStdDev(src_lum, mean, stddev2);

//    auto sc_lum = mean.at<double>(0);

//    return sc_lum;
//}

Imageproc::~Imageproc()
{
    qDebug() << "Destructor";
}

void Imageproc::slotFocusChange(int f_change)
{
    qDebug() << "Changing the focal length by" << f_change;
}
