#include "utility.h"

QString readInput(QString inputBoxTitle, QString inputBoxPrompt, QString readErrorMessage)
{
    QInputDialog *popup = new QInputDialog(NULL, NULL);     // Create popup window
    QString fileName;
    bool read;

    do
    {
        fileName = popup->getText(NULL, inputBoxTitle, inputBoxPrompt, QLineEdit::Normal, "", &read);       // Read user input from window

        if (read && fileName.isEmpty())
            QMessageBox::information(0, "ERROR", readErrorMessage);
    } while (read && fileName.isEmpty());

    if (read)
        return fileName;

    else
        return NULL;
}

float clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;

    return value;
}

/*int findClosestPositionInArray(int value, int array[], int size)
{
    int difference = INT_MAX;
    int position;

    for (int i = 0; i < size; i++)
    {
        int currDiff = std::abs(array[i] - value);
        if (currDiff <= difference && array[i] > 0)
        {
            difference = currDiff;
            position = i;
        }
    }

    return position;
}*/











