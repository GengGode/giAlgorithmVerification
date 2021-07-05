#pragma once
#include <opencv2/opencv.hpp>
#include <qpainter.h>
#include <QtWidgets/QMainWindow>
#include "ui_Cv.h"
using namespace cv;

class Cv : public QMainWindow
{
    Q_OBJECT

public:
    Cv(QWidget *parent = Q_NULLPTR);

private:
    Ui::CvClass ui;

	Mat MainMat;
	QImage MainImg;
	bool isE = false;
protected:
	void paintEvent(QPaintEvent *event);

private:
	Mat calMask(int col, int row, double r, int flag = 0);
	Mat calMask2(int col, int row, int r, double r0, int flag = 0);
	Mat calMask3(int col, int row, int r, double r0, int flag = 0);
private slots:
	void a10();
	void a20();
	void a30();
	//void a30();
};
