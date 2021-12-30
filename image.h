#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cmath>
#include <QApplication>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QColor>
#include <QMenu>
#include "utility.h"

#define FORMAT              "JPG"
#define LOADTITLE           "Load Image"
#define LOADPROMPT          "Please enter name of file to be loaded:"
#define SAVETITLE           "Save Image As"
#define SAVEPROMPT          "Please enter desired image name:"
#define QUANTTITLE          "Quantization"
#define QUANTPROMPT         "Quantize in how many shades?"
#define INPUTERROR          "Could not read input."
#define BRIGHTTITLE         "Adjusting Brightness"
#define BRIGHTPROMPT        "New brightness value:"
#define CONTTITLE           "Adjusting Contrast"
#define CONTPROMPT          "New contrast value:"
#define ZOOMTITLE           "Zooming Out"
#define ZOOMXPROMPT         "How much to zoom horizontally?"
#define ZOOMYPROMPT         "How much to zoom vertically?"
#define KERNELSIZE          3





class Image : public QWidget
{
    Q_OBJECT

private:
    QImage original;
    QImage edited;
    QLabel *displayLabel;
    QWidget *displayWindow;
    bool isGrayscaled;


    void linearTransformation(int bias, int gain);

public:
    bool loadNewImage(QString fileName);
    void saveNewImage();


    void flipHorizontal();
    void flipVertical();

    void adjustBrightness();
    void adjustContrast();

    void zoomOut();
    void zoomIn();
    void rotateClockwise();
    void rotateCounterClockwise();


    int height();
    int width();
    void setWindow(QLabel* label, QWidget* window);
    QImage getOriginal();
    QImage getEdited();
    void restoreOriginal();

    Image()
    {
        isGrayscaled = false;
        displayLabel = new QLabel();
        displayWindow = new QWidget();
    };

    ~Image()
    {
        displayLabel->~QLabel();
        displayWindow->~QWidget();
    };
};

#endif // IMAGE
