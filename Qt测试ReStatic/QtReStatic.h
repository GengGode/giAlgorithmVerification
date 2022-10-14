#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtReStatic.h"

class QtReStatic : public QMainWindow
{
    Q_OBJECT

public:
    QtReStatic(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtReStaticClass ui;
};
