#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QString>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QWidget>
#include <QTimer>
#include <QHash>
#include <QList>
#include <QPoint>
#include <QDir>

#include "Histogram/histogram.h"
#include "Tiff/service_functions.h"
#include "MotionSystem/motionsystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MotionSystem *motionSystemObject = nullptr;



    QHash<int, QString> imageMap;

    void startMove();
    void startXRay();



protected:
    void wheelEvent(QWheelEvent *event);

    // void mousePressEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    // void mouseReleaseEvent(QMouseEvent *event) override;


signals:
    void progressValueChanged(int32_t value);

private slots:
    void slot(ushort *Ddata);
    void on_nextImageButton_clicked();
    void on_imageQualityTextEdit_textChanged(QString score);

    void updateProgressMove();
    void updateProgressXRayImage();
    void on_progressBar_valueChanged(int32_t progress);

    void onStepComboBoxValueChanged(const QString &value);
    void onVoltageComboBoxValueChanged(const QString &value);

    void on_coordXTextEdit_textChanged(QString coord);
    void on_moveButton_clicked();

    void on_textEdit_textChanged();

    void on_autoFocusButton_clicked();

    void delay(int millisecondsToWait);

    // void onCloseButtonClicked();
    // void onMinimizeButtonClicked();
private:


    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *image_scene = nullptr;
    int32_t userCoord;
    int32_t auto_coord;
   // bool xRayStatus;
    myTIFF *tiff = nullptr;
    Histogram *histogr = nullptr;
    QWidget *s = nullptr;

    QTimer *timerXRay = nullptr;
    QTimer *timerMove = nullptr;
    // QTimer timer;
    int32_t progress;
    int32_t startPoint;
    QString coord;
    QString inputCoord;
    QList<int> points;
    QDir dir;
    QList<QString> listImage;
    void showXRayImage();
    int32_t step_move;
    ushort *data = nullptr;
    uint w, h;
    QString quality_image;
    int voltageXRay;
    double max_score;
    //bool fromButton;
    bool quality;


    // QPoint m_dragPosition;
    // QPushButton *m_closeButton;
    // QPushButton *m_minimizeButton;

};

#endif // MAINWINDOW_H
