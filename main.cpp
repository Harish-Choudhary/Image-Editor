#include <iostream>
#include "image.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString imageName;
    Image image;
    bool isLoaded = false;

    do
    {
        imageName = readInput(LOADTITLE, LOADPROMPT, INPUTERROR);

        if (imageName != NULL)
            isLoaded = image.loadNewImage(imageName);
        else
            isLoaded = true;
    } while (isLoaded == false);

    if (imageName == NULL)
        return 0;

    QWidget *oWindow = new QWidget();
    QWidget *eWindow = new QWidget();
    QWidget *menu = new QWidget();
    QWidget *filtersMenu = new QWidget();
    QLabel *oLabel = new QLabel(oWindow);
    QLabel *eLabel = new QLabel(eWindow);
    QVBoxLayout menuOrganizer(menu), filtersOrganizer(filtersMenu);
    image.setWindow(eLabel, eWindow);

    oWindow->setWindowTitle("Original Image");
    eWindow->setWindowTitle("Editing Image");
    menu->setWindowTitle("Options");
    filtersMenu->setWindowTitle("Filters");
    oWindow->move(0, 0);
    eWindow->move(image.width()+70, 0);
    menu->move(2*image.width()+140, 0);
    filtersMenu->move(0, image.height()+70);

    oLabel->setPixmap(QPixmap::fromImage(image.getOriginal()));
    oLabel->show();
    eLabel->setPixmap(QPixmap::fromImage(image.getEdited()));
    eLabel->show();

    QPushButton *bSave = new QPushButton("Save image");
    menuOrganizer.addWidget(bSave);
    QObject::connect(bSave, &QPushButton::clicked, &image, &Image::saveNewImage);

    QPushButton *bFlipH = new QPushButton("Horizontal Flip");
    menuOrganizer.addWidget(bFlipH);
    QObject::connect(bFlipH, &QPushButton::clicked, &image, &Image::flipHorizontal);

    QPushButton *bFlipV = new QPushButton("Vertical Flip");
    menuOrganizer.addWidget(bFlipV);
    QObject::connect(bFlipV, &QPushButton::clicked, &image, &Image::flipVertical);


    QPushButton *bBrightness = new QPushButton("Adjust Brightness");
    menuOrganizer.addWidget(bBrightness);
    QObject::connect(bBrightness, &QPushButton::clicked, &image, &Image::adjustBrightness);

    QPushButton *bContrast = new QPushButton("Adjust Contrast");
    menuOrganizer.addWidget(bContrast);
    QObject::connect(bContrast, &QPushButton::clicked, &image, &Image::adjustContrast);


    QPushButton *bZoomOut = new QPushButton("Zoom Out");
    menuOrganizer.addWidget(bZoomOut);
    QObject::connect(bZoomOut, &QPushButton::clicked, &image, &Image::zoomOut);

    QPushButton *bZoomIn = new QPushButton("Zoom In");
    menuOrganizer.addWidget(bZoomIn);
    QObject::connect(bZoomIn, &QPushButton::clicked, &image, &Image::zoomIn);

    QPushButton *bClockwise = new QPushButton("Rotate 90º (Clockwise)");
    menuOrganizer.addWidget(bClockwise);
    QObject::connect(bClockwise, &QPushButton::clicked, &image, &Image::rotateClockwise);

    QPushButton *bCounterCW = new QPushButton("Rotate 90º (Counter Clockwise)");
    menuOrganizer.addWidget(bCounterCW);
    QObject::connect(bCounterCW, &QPushButton::clicked, &image, &Image::rotateCounterClockwise);

    QPushButton *bRestore = new QPushButton("Restore Original");
    menuOrganizer.addWidget(bRestore);
    QObject::connect(bRestore, &QPushButton::clicked, &image, &Image::restoreOriginal);



    oWindow->show();
    //eWindow->show();
    menu->show();
    //filtersMenu->show();

    return app.exec();
}
