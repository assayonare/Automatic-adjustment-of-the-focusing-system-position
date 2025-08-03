#ifndef IMAGEPROC_H
#define IMAGEPROC_H


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <QDebug>
#include <QObject>


class Imageproc : public QObject
{
    Q_OBJECT

public:
    explicit Imageproc(QObject *parent = nullptr);
    void setImageName(std::string name_image);
    cv::Mat image;
    double blur_score(cv::Mat image);
    double lumiance_score(cv::Mat image);
    int f_change;
    ~Imageproc();
private:
    // std::string name_image;



signals:
    void signalImageProcOut(int f_change);

private slots:
    void slotFocusChange(int f_change);
};
#endif // IMAGEPROC_H
