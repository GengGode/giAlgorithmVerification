#include "Cv.h"

Cv::Cv(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &Cv::a10);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &Cv::a20);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &Cv::a30);

	MainMat = imread("C:/Users/GengG/source/repos/Cv测试/Cv测试/res/mat.png");
}
void Cv::paintEvent(QPaintEvent * event)
{
	//设置画面为地图
	QPainter painter(this);
	if (isE)
	{
		painter.drawImage(0, 0, MainImg);

	}
}

double dis2(double a, double b)
{
	return a * a + b * b;
}

Mat Cv::calMask(int col, int row, double r, int flag)
{
	Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));

	Rect roi = Rect(0, 0, r, r);
	//Mat ReRoi = out(roi);
	int len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if(dis > len -r)
			{
				out.at<uchar>(i, j) =  255.0/(r)*( len-dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	 roi = Rect(0, r, r * 2, col - r * 2);
	 len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, 0));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, col-r, r, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height-r  - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}
	return out;
}


void Cv::a10()
{
	Mat MAINMASK = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_AutoMap/resource/BackGroundMask.bmp");
	
	std::vector<Mat> mv0;
	std::vector<Mat> mv1;
	//通道分离
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols*(MainMat.channels()), QImage::Format_RGBA8888);
	isE = true;
	update();
}

void Cv::a20()
{
	MainMat = imread("C:/Users/GengG/source/repos/Cv测试/Cv测试/res/mat.png");

	Mat MAINMASK = calMask(MainMat.cols, MainMat.rows, 20);

	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	mv0.clear();
	mv1.clear();

	//通道分离
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols*(MainMat.channels()), QImage::Format_ARGB32);
	isE = true;
	update();
}

void Cv::a30()
{
}

