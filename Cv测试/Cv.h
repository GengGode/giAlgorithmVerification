#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cv.h"

class Cv : public QMainWindow
{
    Q_OBJECT

public:
    Cv(QWidget *parent = Q_NULLPTR);

private:
    Ui::CvClass ui;
};
