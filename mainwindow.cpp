#include <QDirIterator>
#include <QMessageBox>
#include <QTextEdit>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageproc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);

    histogr = new Histogram();
    s = new QWidget();
    ui->inverseLayout->addWidget(s);
    // connect(histogr, &Histogram::histogramDone, this, &MainWindow::slot);

    motionSystemObject = new MotionSystem;


    timerMove = new QTimer(this);
    connect(timerMove, &QTimer::timeout, this, &MainWindow::updateProgressMove);

    timerXRay = new QTimer(this);
    connect(timerXRay, &QTimer::timeout, this, &MainWindow::updateProgressXRayImage);

    scene = new QGraphicsScene;
    image_scene = new QGraphicsPixmapItem;
    //image_scene = new QGraphicsPixmapItem;
    ui->graphicsView->setScene(scene);

    connect(this, &MainWindow::progressValueChanged, this, &MainWindow::on_progressBar_valueChanged);
    connect(this, &MainWindow::progressValueChanged, this, &MainWindow::on_progressBar_valueChanged);
    //connect(this, &MainWindow::progressXRayImage, this, &MainWindow::on_progressBar_valueChanged);
    //onnect(this, &MainWindow::progressValueChanged, this, &MainWindow::on_progressBar_valueChanged);
    //connect(ui->moveButton, &QPushButton:: clicked, this, &MainWindow::startMove);
    ui -> progressBar->hide();

    on_coordXTextEdit_textChanged(QString::number(0));
    ui->voltageComboBox->addItem("0");
    ui->voltageComboBox->addItem("80");
    ui->stepComboBox->addItem("0");
    ui->stepComboBox->addItem("100");
    connect(ui->stepComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onStepComboBoxValueChanged);
    connect(ui->voltageComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onVoltageComboBoxValueChanged);
    //QDir dir;
    dir.cd("photo_set2");
    listImage = dir.entryList(QDir::NoDot|QDir::NoDotDot|QDir::AllEntries);

    int point = -1200;
    for (int i =0; i <listImage.size() ; i++){
        point += 100;
        points.append(point);
    }
    for (int i = 0; i < listImage.size(); ++i) {
        imageMap.insert(points[i], listImage[i]);
    }
    qDebug() << imageMap;


    userCoord = 0;
    startPoint = 0;
    auto_coord = 0;
    step_move = 0;
    //fromButton = true;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timerMove;
    delete timerXRay;
    delete scene;
    delete image_scene;
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        ui->graphicsView->scale(1.1, 1.1);
    else
        ui->graphicsView->scale(1/1.1, 1/1.1);
}


void MainWindow::on_nextImageButton_clicked()
{

    ui->progressBar->setValue(0);
    startXRay();
    ui->progressBar->show();

}
void MainWindow:: showXRayImage(){

    Imageproc i;
    double score_image = 0;
    scene->clear();
    // преобразование тифа
    tiff = new myTIFF("test");
    QString path = dir.absoluteFilePath(imageMap.value(userCoord));
    QByteArray ba = path.toLocal8Bit();
    const char *cstr = ba.data();
    ushort *data = tiff->openFile(cstr, &w, &h);


    ui->inverseLayout->removeWidget(s);
    histogr->getHistogram(data, w, h);
    s = new QWidget();
    s = histogr->getPointerToPlot();
    ui->inverseLayout->addWidget(s);
    connect(histogr, &Histogram::histogramDone, this, &MainWindow::slot);


    QPixmap  pix = service_functions::tiffToPixmap(data, w, h);
    std:: string file_dir = dir.absoluteFilePath(imageMap.value(userCoord)).toStdString();

    image_scene = scene->addPixmap(pix);
    image_scene->setFlags(QGraphicsItem::ItemIsMovable);
    scene->setSceneRect(0, 0, scene->width(), scene->width());
    ui->graphicsView->fitInView(image_scene, Qt::KeepAspectRatio);
    i.setImageName(file_dir);

    score_image = i.blur_score(i.image);

    on_imageQualityTextEdit_textChanged(QString::number(score_image));
    qDebug() << imageMap.value(userCoord);


}

void MainWindow::on_imageQualityTextEdit_textChanged(QString score)
{
    ui->imageQualityTextEdit->setPlainText(score);
}


void MainWindow::on_progressBar_valueChanged(int32_t progress)
{

    ui->progressBar->setValue(progress);

    if (progress == 100) {
        ui->nextImageButton->setEnabled(true);
    } else {
        ui->nextImageButton->setEnabled(false);
    }

    //qDebug() << progress;

}
void MainWindow::startMove()
{
    progress = 0;
    timerMove->start(5);

}
void MainWindow::startXRay()
{
    progress = 0;
    timerXRay->start(20);

}

void MainWindow:: updateProgressMove()
{

    ui->progressBar->setFormat("Перемещение: %p%");
    progress = motionSystemObject->getCoordinates();
    //if(startPoint>userCoord){
    progress = abs((double(abs(progress-startPoint))/(abs(startPoint-userCoord)))*100);
    //}
    //else{
        //progress = abs((double(userCoord-progress)/(abs(startPoint)-abs(userCoord)))*100);
    //}


    if (userCoord == startPoint){
        progress = 100;

    }
    qDebug() << progress;

    emit progressValueChanged(progress);


    if (progress >= 100){
        timerMove->stop();
        on_coordXTextEdit_textChanged(QString::number(motionSystemObject->getCoordinates()));
        startPoint = motionSystemObject->getCoordinates();
        //ui -> progressBar->hide();
    }

}

void MainWindow::updateProgressXRayImage()
{
    ui->progressBar->setFormat("Получение снимка: %p%");
    progress += 1;

    qDebug() << progress;
    emit progressValueChanged(progress);

    if (progress >= 100) {
        showXRayImage();
        timerXRay->stop();
    }
}


void MainWindow::on_coordXTextEdit_textChanged(QString coord)
{
    ui->coordXTextEdit->setPlainText(coord);
}

//void mainWindow::getImage(QString coord)


void MainWindow::on_textEdit_textChanged()
{
    inputCoord = ui->textEdit->toPlainText();
    userCoord = inputCoord.toInt();
    qDebug() << userCoord;

}



void MainWindow::on_moveButton_clicked()
{
    ui->progressBar->setValue(0);
    startMove();
    ui->progressBar->show();
    on_imageQualityTextEdit_textChanged(QString::number(0));
    // if (fromButton = true)
    // {
    //    auto_coord = userCoord;
    // }


    GoTo *goToObject = new GoTo;

    goToObject->speed.speed = 2000;
    goToObject->axes.a1 = 1;
    goToObject->acceleration.acceleration = 0;

    motionSystemObject->goTo(goToObject->nCmd, goToObject->axes, goToObject->speed, goToObject->acceleration, userCoord);

    delete goToObject;

}
void MainWindow::onStepComboBoxValueChanged(const QString &value){

    step_move = static_cast<int32_t>(value.toInt());
    qDebug() << step_move;
}
void MainWindow::onVoltageComboBoxValueChanged(const QString &value){

    voltageXRay = static_cast<int32_t>(value.toInt());
    qDebug() << voltageXRay;
}

void MainWindow::on_autoFocusButton_clicked()
{
    Imageproc autoIm;
    quality = false;
    GoTo *goToObject = new GoTo;
    goToObject->speed.speed = 2000;
    goToObject->axes.a1 = 1;
    goToObject->acceleration.acceleration = 0;
    int count = 0 ;
    int32_t max_coord = 0;
    max_score = 0;
    auto_coord = 0;
    bool orientation = true;
    //fromButton = false;

    while (quality != true){

        ui->progressBar->setValue(0);
        startMove();
        ui->progressBar->show();

        motionSystemObject->goTo(goToObject->nCmd, goToObject->axes, goToObject->speed, goToObject->acceleration, auto_coord);
        qDebug() << "moving";
        delay(4000);
        on_coordXTextEdit_textChanged(QString::number(auto_coord));



        qDebug() << auto_coord;
        double score_im = 0;
        std:: string file_dir = dir.absoluteFilePath(imageMap.value(auto_coord)).toStdString();
        autoIm.setImageName(file_dir);

        if (orientation == false){
            step_move = -step_move;
        }

        score_im = autoIm.blur_score(autoIm.image);
        if (score_im >= max_score){

            max_score = score_im;
            max_coord = auto_coord;
            quality_image = dir.absoluteFilePath(imageMap.value(auto_coord));
            userCoord = auto_coord;



            //simage_scene = new QGraphicsPixmapItem;
            on_imageQualityTextEdit_textChanged(QString::number(0));
            ui->progressBar->setValue(0);
            startXRay();
            ui->progressBar->show();
            qDebug() << "screen";

            delay(3000);


            auto_coord += step_move;
        }
        else if (score_im < max_score){
            if (count < 3){
                auto_coord += step_move;
                userCoord = auto_coord;


                on_imageQualityTextEdit_textChanged(QString::number(0));
                ui->progressBar->setValue(0);
                startXRay();
                ui->progressBar->show();
                qDebug() << "screen";

                delay(3000);


                //showXRayImage();
                count++;
            }
            else {
                if (orientation == false){
                    quality = true;
                }
                orientation = false;

            }
        }

        qDebug() << imageMap.value(auto_coord);
        if (auto_coord == 1100 || auto_coord == -1100){
            break;
        }
    }
    delete goToObject;


    //userCoord = max_coord;


    scene->clear();
    // преобразование тифа
    tiff = new myTIFF("test");
    QString path = dir.absoluteFilePath(imageMap.value(max_coord));
    QByteArray ba = path.toLocal8Bit();
    const char *cstr = ba.data();
    ushort *data = tiff->openFile(cstr, &w, &h);

    QPixmap  pix = service_functions::tiffToPixmap(data, w, h);

    image_scene = scene->addPixmap(pix);
    image_scene->setFlags(QGraphicsItem::ItemIsMovable);
    scene->setSceneRect(0, 0, scene->width(), scene->width());
    ui->graphicsView->fitInView(image_scene, Qt::KeepAspectRatio);

    on_coordXTextEdit_textChanged(QString::number(max_coord));
    on_imageQualityTextEdit_textChanged(QString::number(max_score));
    //fromButton = true;

}
void MainWindow::delay(int millisecondsToWait)
{
    QEventLoop loop;
    QTimer::singleShot(millisecondsToWait, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow:: slot(ushort *Ddata){
    // ui->gridLayout->removeWidget(s);
    // histogr->getHistogram(data, w, h);
    // s = histogr->getPointerToPlot();
    // ui->gridLayout->addWidget(s);
    scene->clear();
    QPixmap  pix = service_functions::tiffToPixmap(Ddata, w, h);
    image_scene = scene->addPixmap(pix);
    ui->graphicsView->fitInView(image_scene, Qt::KeepAspectRatio);
}
