#include "service_functions.h"

myTIFF::myTIFF(QString name)
{
    //////////////// Для открытия tiff файлов///////////////////////////////////////////////////////
        nameOfClass = name;
        QLibrary lib2("libTIFF64.dll");
        if(lib2.load())
            qDebug() << QString("myTIFF::%1::Библиотека загружена").arg(nameOfClass);
        else
            qDebug() << QString("myTIFF::%1::Библиотека не загружена").arg(nameOfClass);

        myTIFFOpen = (TIFFOpen) QLibrary::resolve("libTIFF64.dll", "TIFFOpen");
        myTIFFClose = (TIFFClose) QLibrary::resolve("libTIFF64.dll", "TIFFClose");
        myTIFFGetField = (TIFFGetField) QLibrary::resolve("libTIFF64.dll", "TIFFGetField");
        myTIFFReadScanline = (TIFFReadScanline) QLibrary::resolve("libTIFF64.dll", "TIFFReadScanline");
        my_TIFFmalloc = (_TIFFmalloc) QLibrary::resolve("libTIFF64.dll", "_TIFFmalloc");
        myTIFFScanlineSize = (TIFFScanlineSize) QLibrary::resolve("libTIFF64.dll", "TIFFScanlineSize");

        myTIFFWriteScanline = (TIFFWriteScanline) QLibrary::resolve("libTIFF64.dll", "TIFFWriteScanline");
        myTIFFDefaultStripSize = (TIFFDefaultStripSize) QLibrary::resolve("libTIFF64.dll", "TIFFDefaultStripSize");
        myTIFFSetField = (TIFFSetField) QLibrary::resolve("libTIFF64.dll", "TIFFSetField");  
        myTIFFSetupStrips = (TIFFClose) QLibrary::resolve("libTIFF64.dll", "TIFFSetupStrips");

        myTIFFMergeFieldInfo = (TIFFMergeFieldInfo) QLibrary::resolve("libTIFF64.dll", "TIFFMergeFieldInfo");
        myTIFFSetTagExtender = (TIFFSetTagExtender) QLibrary::resolve("libTIFF64.dll", "TIFFSetTagExtender");

        data = new ushort[1000];
}

myTIFF::~myTIFF()
{
    qDebug() << "~myTIFF()0" << nameOfClass;
    if(data) delete[] data;
    qDebug() << "~myTIFF()1" << nameOfClass;
}

ushort * myTIFF::openFile(const char * path, uint * width, uint * height)
{
    augment_libtiff_with_custom_tags();
    TIFF * img = myTIFFOpen(path, "r");
    if (img)
    {
        uint32 w, h;

        myTIFFGetField(img, TIFFTAG_IMAGEWIDTH, &w);
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &h);

        uint32 imagelength;
        tdata_t buf;
        uint32 row;
        delete[] data;
        data = new ushort[w*h];
        memcpy(width, (uint *)&w, SIZEOF_INT);
        memcpy(height, (uint *)&h, SIZEOF_INT);
        tsize_t size = myTIFFScanlineSize(img);
        buf = my_TIFFmalloc(size);
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &imagelength);

        char* imageDesc = nullptr, *params = nullptr, *date = nullptr;
        myTIFFGetField(img, TIFFTAG_IMAGEDESCRIPTION, &imageDesc);
        myTIFFGetField(img, TIFFTAG_DOCUMENTNAME, &params);
        myTIFFGetField(img, TIFFTAG_DATETIME, &date);

        m_date = date;
        m_params = params;
        m_imageDesciption = imageDesc;
        ushort * pix;
        for (int i=0;i<h;i++)
        {
            row = i;
            myTIFFReadScanline(img, buf, row, 0);
            pix = (ushort*) buf;

            memcpy((data+i*w),pix,size);
        }
        myTIFFClose(img);
        return data;
    }
    else
    {
        qDebug() << path;
        return NULL;
    }
}

void myTIFF::writeFile(const char * path, ushort * data, uint  width, uint  height)
{
    qDebug() << "path" << path << width << height;
    augment_libtiff_with_custom_tags();
    TIFF *out = myTIFFOpen(path, "w");
    if (out)
    {
        qDebug() << "inm";
        uint32 imagelength, imagewidth;
        uint16 nsamples =  imagewidth*imagelength;
        imagewidth = width;
        imagelength = height;

        myTIFFSetField(out, TIFFTAG_IMAGELENGTH, imagelength);
        myTIFFSetField(out, TIFFTAG_IMAGEWIDTH,  imagewidth);
        myTIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
        myTIFFSetField(out, TIFFTAG_PHOTOMETRIC, 1);
        myTIFFSetField(out, TIFFTAG_COMPRESSION, 1);
        myTIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 16);
        myTIFFSetField(out, TIFFTAG_ROWSPERSTRIP, myTIFFDefaultStripSize(out, imagelength));
        myTIFFSetField(out, TIFFTAG_COPYRIGHT, "EltechMed");

        myTIFFSetField(out, TIFFTAG_MAKE, "ЗАО ЭЛТЕХ-Мед; ©ЗАО ЭлтехМед"); //инфо об установке  константное НЕ передаем
        myTIFFSetField(out, TIFFTAG_MODEL, "ПРДУ-Электроник");
        myTIFFSetField(out, TIFFTAG_IMAGEDESCRIPTION, m_imageDesciption.toStdString().c_str()); // описание исследования необхоимо составлять и ПЕРЕДАВАТЬ параметром
        myTIFFSetField(out, TIFFTAG_SOFTWARE, "PRDU-Electronics"); //примененные инструменты НЕ передаем из основной программы по умолчанию ""
        myTIFFSetField(out, TIFFTAG_DOCUMENTNAME, m_params.toStdString().c_str()); // описание исследования, ПЕРЕДАЕМ
        myTIFFSetField(out, TIFFTAG_DATETIME, m_date.toStdString().c_str()); // ПЕРЕДАЕМ

        myTIFFSetupStrips(out);

        for (int i = 0; i < height; i++)
        {
            myTIFFWriteScanline(out, &data[i*width], i, 0);
        }

        myTIFFClose(out);
    }
    else
    {
        qDebug() << QString("myTIFF::%1::writeFile ERROR!").arg(nameOfClass) << path;
    }
}

void myTIFF::registerCustomTIFFTags(TIFF *tif)
{
    /* Install the extended Tag field info */
    /*int error = */myTIFFMergeFieldInfo(tif, xtiffFieldInfo, N(xtiffFieldInfo));
    if (parent_extender)
        (*parent_extender)(tif);
}

void myTIFF::augment_libtiff_with_custom_tags() {
    static bool first_time = true;
    if (!first_time) return;
    first_time = false;
    myTIFFSetTagExtender(parent_extender);
}

uint myTIFF::getWidth(const char *path)
{
    TIFF * img = myTIFFOpen(path, "r");
    uint w;
    if (img)
    {
        myTIFFGetField(img, TIFFTAG_IMAGEWIDTH, &w);
    }
    myTIFFClose(img);
    return w;
}

uint myTIFF::getHeight(const char *path)
{
    TIFF * img = myTIFFOpen(path, "r");
    uint h;
    if (img)
    {
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &h);
        myTIFFClose(img);
    }
    else
    {
        qDebug() << QString("myTIFF::%1::getWidth ERROR!").arg(nameOfClass) << path;
        return 0;
    }

    return h;
}

void myTIFF::setParametersForTags(QString imageDisc, QString date, QString params)
{
    m_imageDesciption = imageDisc;
    m_date = date;
    m_params = params;
}

void myTIFF::getParametersFromTags(QString *imageDisc, QString *date, QString *params)
{
    *imageDisc = m_imageDesciption;
    *date = m_date ;
    *params = m_params;
}

QString myTIFF::getByteSize(const char *path)
{
    return QString().number(getHeight(path) * getWidth(path) * 2);
}

service_functions::service_functions(QObject *parent) : QObject(parent)
{

}

QPixmap service_functions::tiffToPixmap(ushort *RAWData, int w, int h)
{
    QImage image = QImage(w, h, QImage::Format_Indexed8);
    quint8 * line = image.scanLine(0);
    int stride = image.bytesPerLine();
    qint32 pixel = 0;

    for (uint y = 0; y < h; ++y, line += stride)
    {
        quint8 * pix = line;

        for ( uint x = 0; x < w; ++x, pix += 1)
        {
            pixel = RAWData[(y*w)+x]/255;
            if (pixel > 255) pixel = 255;
            if (pixel < 0) pixel = 0;
            pix[0] = pixel;
        }
    }

    static QPixmap pix;
    pix.convertFromImage(image);
    return pix;
}

QString service_functions::RenameOfImages(QString prefix, ushort Count, QString fileExtension)
{
    QString FormatOfName;
    FormatOfName = QString("%1").arg(Count);
    while (FormatOfName.length() <= FORMAT_NAME_OF_IMAGE)
    {
        FormatOfName = '0' + FormatOfName;
    }
    FormatOfName = prefix + FormatOfName + fileExtension;
    return FormatOfName;
}

void service_functions::deletespace(QString adress)
{
    QFile file(adress);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Service::Error";
    }
    QByteArray tmp = file.readAll();
    file.close();

    QFile file_tmp(adress);
    if (file_tmp.open(QIODevice::WriteOnly | QIODevice::Truncate))
    file_tmp.close();

    QFile file_out(adress);
    if (!file_out.open(QIODevice::WriteOnly))
    {
        qDebug() << "Service::Error";
    }
    tmp.replace("%20"," ");
    tmp.replace("%28","(");
    tmp.replace("%29",")");
    tmp.replace("%25","%");
    tmp.replace("%2C",",");
    file_out.write(tmp);
}


void service_functions::RotateImage(ushort * dData, int image_width, int image_height)
{
    // Поворот изображения на заданный угол
    ushort * tempData;
    tempData = new ushort[image_width * image_height];
    memcpy(tempData, dData, image_width * image_height * 2);

    float a = -1*M_PI/2;
    float sinC, cosC, r;
    int x,y;
    int io = image_height / 2;
    int jo = image_width / 2;
    for (int j=1; j<image_height-1; j++) // Разобраться где ширина где высота TODO: Допилить для прямоугольных изображений
    {
        for (int i=1; i<image_height-1; i++)
        {
            r = sqrt(pow(i - io,2) + pow(j - jo,2));
                sinC = sin(a + atan2((j - jo), (i - io)));
                cosC = cos(a + atan2((j - jo), (i - io)));
                x = (int)round(io + r * cosC);
                y = (int)round(jo + r * sinC);

                dData[i+j*image_width] = tempData[x+y*image_width];
        }
    }
    delete[] tempData;
}

void myTIFF::parseParams(QString params, int *u, double *i, double* t, double* magnificent, int* distSO, int* distDO)
{
    QStringList parseParams;
    parseParams  = params.split(';');
    *u = QString(parseParams.at(0)).toInt();
    *i = QString(parseParams.at(1)).toDouble()/1000;
    *t = QString(parseParams.at(2)).toDouble()*1000;

    *magnificent = QString(parseParams.at(3)).toDouble();
    *distSO = QString(parseParams.at(4)).toDouble();
    *distDO = QString(parseParams.at(5)).toDouble();
}
