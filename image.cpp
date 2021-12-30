#include "image.h"

bool Image::loadNewImage(QString fileName)
{
    bool oWorked, eWorked;

    oWorked = original.load(fileName);      // Saves original image for recovery purposes
    eWorked = edited.load(fileName);        // Saves image to be edited at will

    return oWorked && eWorked;
}

void Image::saveNewImage()
{
    QString imageName;
    imageName = readInput(SAVETITLE, SAVEPROMPT, INPUTERROR);

    if (imageName == nullptr)
        return;

    edited.save(imageName, FORMAT, 100);
}



void Image::flipHorizontal()
{
    int w = edited.width();
    int h = edited.height();
    QImage flipped = QPixmap(w, h).toImage();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            pixelValue = edited.pixel(i, j);
            flipped.setPixel(w-i-1, j, pixelValue.rgb());
        }
    }
    edited = flipped;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
}

void Image::flipVertical()
{
    int w = edited.width();
    int h = edited.height();
    QImage flipped = QPixmap(w, h).toImage();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            pixelValue = edited.pixel(i, j);
            flipped.setPixel(i, h-j-1, pixelValue.rgb());
        }
    }
    edited = flipped;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
}



void Image::linearTransformation(int bias, int gain)
{
    int w = edited.width(), h = edited.height();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            int newValue;
            pixelValue = edited.pixel(i, j);

            newValue = static_cast<int>(gain*pixelValue.red() + bias);
            newValue = static_cast<int>(clamp(newValue, 0, 255));
            pixelValue.setRed(newValue);

            newValue = static_cast<int>(gain*pixelValue.green() + bias);
            newValue = static_cast<int>(clamp(newValue, 0, 255));
            pixelValue.setGreen(newValue);

            newValue = static_cast<int>(gain*pixelValue.blue() + bias);
            newValue = static_cast<int>(clamp(newValue, 0, 255));
            pixelValue.setBlue(newValue);

            edited.setPixel(i, j, pixelValue.rgb());
        }
    }
}

void Image::adjustBrightness()
{
    int bias;
    QString input = readInput(BRIGHTTITLE, BRIGHTPROMPT, INPUTERROR);

    if (input == nullptr)
        return;

    bias = input.toInt();
    bias = static_cast<int>(clamp(bias, -255, 255));

    linearTransformation(static_cast<double>(bias), 1);
    displayLabel->setPixmap(QPixmap::fromImage(edited));
}

void Image::adjustContrast()
{
    double gain;
    QString input = readInput(CONTTITLE, CONTPROMPT, INPUTERROR);

    if (input == nullptr)
        return;

    gain = input.toDouble();
    gain = clamp(gain, 0.001, 255.0);

    linearTransformation(0, gain);
    displayLabel->setPixmap(QPixmap::fromImage(edited));
}

void Image::zoomOut()
{
    QString input = readInput(ZOOMTITLE, ZOOMXPROMPT, INPUTERROR);
    int sx = input.toInt();

    if (input == nullptr)
        return;

    input = readInput(ZOOMTITLE, ZOOMXPROMPT, INPUTERROR);
    int sy = input.toInt();

    if (input == nullptr)
        return;

    int w = edited.width(), h = edited.height();
    QImage zoomedOut = QPixmap(w/sx, h/sy).toImage();

    for (int i = 0; i < w-1; i+=sx)
    {
        for (int j = 0; j < h-1; j+=sy)
        {
            // Use a rectangle of size sx by sy to reduce image with factors sx and sy
            QColor originalPixel, newPixel;
            int newRed = 0, newGreen = 0, newBlue = 0;

            originalPixel = edited.pixel(i, j);
            // Calculate average of pixel values inside rectangle
            for (int k = 0; k < sx; k++)
            {
                for (int m = 0; m < sy; m++)
                {
                    newPixel = edited.pixel(i+k, j+m);
                    newRed += newPixel.red();
                    newGreen += newPixel.green();
                    newBlue += newPixel.blue();
                }
            }


            // Average becomes new pixel value (for each channel)
            newPixel.setRed(newRed/(sx*sy));
            newPixel.setGreen(newGreen/(sx*sy));
            newPixel.setBlue(newBlue/(sx*sy));

            zoomedOut.setPixel(i/sx, j/sy, newPixel.rgb());
        }
    }

    edited = zoomedOut;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
    displayLabel->setGeometry(0, 0, edited.width(), edited.height());
    displayWindow->setGeometry(original.width()+70, 0, edited.width(), edited.height());
}

void Image::zoomIn()
{
    int w = edited.width(), h = edited.height();
    QImage zoomedIn = QPixmap(2*w-1, 2*h-1).toImage();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            pixelValue = edited.pixel(i, j);
            zoomedIn.setPixel(2*i, 2*j, pixelValue.rgb());
        }
    }

    for (int i = 1; i < 2*w-1; i+=2)
    {
        for (int j = 0; j < 2*h-1; j+=2)
        {
            QColor pixelValue1, pixelValue2, newValue;
            pixelValue1 = zoomedIn.pixel(i-1, j);
            pixelValue2 = zoomedIn.pixel(i+1, j);

            newValue.setRed((pixelValue1.red() + pixelValue2.red())/2);
            newValue.setGreen((pixelValue1.green() + pixelValue2.green())/2);
            newValue.setBlue((pixelValue1.blue() + pixelValue2.blue())/2);

            zoomedIn.setPixel(i, j, newValue.rgb());
        }
    }

    for (int i = 0; i < 2*w-1; i+=2)
    {
        for (int j = 1; j < 2*h-1; j+=2)
        {
            QColor pixelValue1, pixelValue2, newValue;
            pixelValue1 = zoomedIn.pixel(i, j-1);
            pixelValue2 = zoomedIn.pixel(i, j+1);

            newValue.setRed((pixelValue1.red() + pixelValue2.red())/2);
            newValue.setGreen((pixelValue1.green() + pixelValue2.green())/2);
            newValue.setBlue((pixelValue1.blue() + pixelValue2.blue())/2);

            zoomedIn.setPixel(i, j, newValue.rgb());
        }
    }

    for (int i = 1; i < 2*w-1; i+=2)
    {
        for (int j = 1; j < 2*h-1; j += 2)
        {
            QColor pixel1, pixel2, pixel3, pixel4, newValue;
            pixel1 = zoomedIn.pixel(i-1, j);
            pixel2 = zoomedIn.pixel(i, j-1);
            pixel3 = zoomedIn.pixel(i+1, j);
            pixel4 = zoomedIn.pixel(i, j+1);

            newValue.setRed((pixel1.red() + pixel2.red() + pixel3.red() + pixel4.red())/4);
            newValue.setGreen((pixel1.green() + pixel2.green() + pixel3.green() + pixel4.green())/4);
            newValue.setBlue((pixel1.blue() + pixel2.blue() + pixel3.blue() + pixel4.blue())/4);

            zoomedIn.setPixel(i, j, newValue.rgb());
        }
    }

    edited = zoomedIn;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
    displayLabel->setGeometry(0, 0, edited.width(), edited.height());
    displayWindow->setGeometry(original.width()+70, 0, edited.width(), edited.height());
}

void Image::rotateClockwise()
{
    int w = edited.width(), h = edited.height();
    QImage rotated = QPixmap(h, w).toImage();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            pixelValue = edited.pixel(i, j);
            rotated.setPixel(h-j-1, i, pixelValue.rgb());
        }
    }

    edited = rotated;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
    displayLabel->setGeometry(0, 0, edited.width(), edited.height());
    displayWindow->setGeometry(original.width()+70, 0, edited.width(), edited.height());
}

void Image::rotateCounterClockwise()
{
    int w = edited.width(), h = edited.height();
    QImage rotated = QPixmap(h, w).toImage();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            QColor pixelValue;
            pixelValue = edited.pixel(i, j);
            rotated.setPixel(j, w-i-1, pixelValue.rgb());
        }
    }

    edited = rotated;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
    displayLabel->setGeometry(0, 0, edited.width(), edited.height());
    displayWindow->setGeometry(original.width()+70, 0, edited.width(), edited.height());
}



void Image::setWindow(QLabel* label, QWidget* window)
{
    displayLabel = label;
    displayWindow = window;
}

QImage Image::getOriginal()
{
    return original;
}

QImage Image::getEdited()
{
    return edited;
}

void Image::restoreOriginal()
{
    edited = original;
    displayLabel->setPixmap(QPixmap::fromImage(edited));
    displayLabel->setGeometry(0, 0, edited.width(), edited.height());
    displayWindow->setGeometry(original.width()+70, 0, edited.width(), edited.height());
    isGrayscaled = false;
}

int Image::height()
{
    return edited.height();
}

int Image::width()
{
    return edited.width();
}
