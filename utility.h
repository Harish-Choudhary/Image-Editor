#ifndef UTILITY
#define UTILITY

#include <iostream>
#include <cmath>
#include <QtWidgets>
#include <QMessageBox>
#include <QInputDialog>


QString readInput(QString inputBoxTitle, QString inputBoxPrompt, QString readErrorMessage);

float clamp(float value, float min, float max);



#endif // UTILITY

