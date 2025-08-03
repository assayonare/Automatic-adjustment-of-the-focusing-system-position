#ifndef SERVICE_FUNCTIONS_H
#define SERVICE_FUNCTIONS_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include "tiffio2.h"
#include <QLibrary>
#include <QDebug>

#include <QFileDialog>
#include <QMouseEvent>
#include <QDir>
#include <QPixmap>

#include <QObject>
#include <QDebug>
#include <QFile>
#include "definitions.h"

class service_functions : public QObject
{
    Q_OBJECT
public:
    explicit service_functions(QObject *parent = 0);
    static QPixmap tiffToPixmap(ushort *RAWData, int w, int h);

signals:

public slots:
    static void RotateImage(ushort * dData, int image_width, int image_height);
    static QString RenameOfImages(QString prefix, ushort Count, QString fileExtension);
    static void deletespace(QString adress);
};

#define N(a) (sizeof(a) / sizeof (a[0]))
#define TIFFTAG_INCIDENTPOWER     65000

static const TIFFFieldInfo xtiffFieldInfo[] = {
        { TIFFTAG_INCIDENTPOWER,  1, 1, TIFF_DOUBLE,  FIELD_CUSTOM, 0, 0, const_cast<char*>("LaserIncidentPower") },};


class myTIFF: public QObject

{
    Q_OBJECT

public:
    myTIFF(QString name);
    ~myTIFF();

    TIFFExtendProc parent_extender = NULL;  // In case we want a chain of extensions
    void registerCustomTIFFTags(TIFF *tif);
    void augment_libtiff_with_custom_tags();

    ushort* openFile(const char *  path, uint * width, uint * height);

    void writeFile(const char * path, ushort *data, uint  width, uint  height);

    uint getWidth(const char *path);
    uint getHeight(const char *path);

    void setParametersForTags(QString imageDisc, QString date, QString params);

    void getParametersFromTags(QString *imageDisc, QString *date, QString *params);
    void parseParams(QString params, int *u, double *i, double* t, double* magnificent, int* distSO, int* distDO);



    ushort * data;

    typedef TIFF* (*TIFFOpen)(const char*, const char*);
    TIFFOpen myTIFFOpen;

    typedef int (*TIFFGetField)(TIFF*, ttag_t, ...);
    TIFFGetField myTIFFGetField;

    typedef int (*TIFFReadScanline)(TIFF*, tdata_t, uint32, tsample_t);
    TIFFReadScanline myTIFFReadScanline;

    typedef tdata_t (*_TIFFmalloc)(tsize_t);
    _TIFFmalloc my_TIFFmalloc;

    typedef tsize_t (*TIFFScanlineSize)(TIFF*);
    TIFFScanlineSize myTIFFScanlineSize;

    typedef int (*TIFFWriteScanline)(TIFF*, tdata_t, uint32, tsample_t);
    TIFFWriteScanline myTIFFWriteScanline;

    typedef uint32 (*TIFFDefaultStripSize)(TIFF*, uint32);
    TIFFDefaultStripSize myTIFFDefaultStripSize;

    typedef int (*TIFFSetField)(TIFF*, ttag_t, ...);
    TIFFSetField myTIFFSetField;

    typedef void (*TIFFClose)(TIFF*);
    TIFFClose myTIFFClose;

    typedef void (*TIFFSetupStrips)(TIFF*);
    TIFFSetupStrips myTIFFSetupStrips;

    typedef void (*TIFFMergeFieldInfo)(TIFF*, const TIFFFieldInfo[], int);
    TIFFMergeFieldInfo myTIFFMergeFieldInfo;

    typedef TIFFExtendProc (*TIFFSetTagExtender)(TIFFExtendProc);
    TIFFSetTagExtender myTIFFSetTagExtender;

    QString getByteSize(const char *path);

protected:
    QString nameOfClass;
    QString m_imageDesciption, m_date, m_params;

};

#endif // SERVICE_FUNCTIONS_H
