#pragma once
#include "3rdparty/libocr/include/libocr.h"
#pragma comment(lib,"3rdparty/libocr/lib/libocr.lib")

#include "gi_test.h"



auto left_rect(const cv::Mat& frame)
{
	auto rect = cv::Rect(static_cast<int>(frame.cols / 20.0), static_cast<int>(frame.rows / 2.0), static_cast<int>(frame.cols / 6.0), static_cast<int>(frame.rows / 3.0));
	return rect;
}
auto right_rect(const cv::Mat& frame)
{
	auto rect = cv::Rect(static_cast<int>(frame.cols / 5.0 * 3.0), static_cast<int>(frame.rows / 4.0), static_cast<int>(frame.cols / 6.0), static_cast<int>(frame.rows / 2.0));
	return rect;
}

std::string ocr_call(cv::Mat& mat)
{
	cv::Mat mat_gray;
	cv::cvtColor(mat, mat_gray, COLOR_BGRA2BGR);
	std::string text;
	{
		const char* t = libocr::ocr_image(mat_gray.cols, mat_gray.rows, (char*)mat_gray.data, mat_gray.channels() * mat_gray.rows * mat_gray.cols);
		text = t;
		libocr::free_char((char*)t);
	}
	return text;
}
std::vector<std::string> ocr_call(std::vector<cv::Mat>& vec)
{
	std::vector<std::string> str;
	for (int i = 0; i < vec.size(); i++)
	{
		str.push_back(ocr_call(vec[i]));
	}
	return str;
}

auto get_right_object(const cv::Mat& frame)
{
	auto rect = right_rect(frame);
	auto roi = frame(rect);
	using namespace std;
	using namespace cv;

	roi = roi(cv::Rect(64, 0, roi.cols - 64, roi.rows));
	vector<Mat> layers;
	split(roi, layers);

	// 横向拼接
	Mat h_all = Mat::zeros(Size(roi.cols * layers.size(), roi.rows), CV_8UC1);
	for (int i = 0; i < layers.size(); i++)
	{
		Mat r = h_all(Rect(roi.cols * i, 0, roi.cols, roi.rows));
		layers[i].copyTo(r);
	}
	// show
	//imshow("h_all", h_all);
	// show alpha layer
	Mat alpha_layer, alpha_layer_max;
	threshold(layers[3], alpha_layer, 245, 255, THRESH_BINARY);

	// show alpha layer
	//imshow("alpha_layer", alpha_layer);
	// 扩散三个像素
	Mat kernel = getStructuringElement(MORPH_RECT, Size(11, 3));
	dilate(alpha_layer, alpha_layer, kernel);

	// 查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(alpha_layer, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 画出轮廓
	Mat drawing = Mat::zeros(alpha_layer.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		Rect rect = boundingRect(contours[i]);
		rectangle(layers[3], rect, color, 1);
	}
	imshow("alpha right", layers[3]);


	// 从原图中取出矩形
	vector<Mat> roi_vec;
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);
		if (rect.width < 16 || rect.height < 16 ||
			rect.width / rect.height < 0.3)
		{
			continue;
		}
		roi_vec.push_back(roi(rect));
	}

	// ocr
	auto res = ocr_call(roi_vec);
	return res;
}

auto get_left_object(const cv::Mat& frame)
{
	auto rect = left_rect(frame);
	auto roi = frame(rect);
	using namespace std;
	using namespace cv;

	roi = roi(cv::Rect(56, 0, roi.cols - 64, roi.rows));
	vector<Mat> layers;
	split(roi, layers);

	// 横向拼接
	Mat h_all = Mat::zeros(Size(roi.cols * layers.size(), roi.rows), CV_8UC1);
	for (int i = 0; i < layers.size(); i++)
	{
		Mat r = h_all(Rect(roi.cols * i, 0, roi.cols, roi.rows));
		layers[i].copyTo(r);
	}
	// show
	//imshow("h_all", h_all);
	// show alpha layer
	Mat alpha_layer, alpha_layer_max;
	threshold(layers[3], alpha_layer, 245, 255, THRESH_BINARY);

	// show alpha layer
	//imshow("alpha_layer", alpha_layer);
	// 扩散三个像素
	Mat kernel = getStructuringElement(MORPH_RECT, Size(13, 7));
	dilate(alpha_layer, alpha_layer, kernel);

	// 查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(alpha_layer, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 画出轮廓
	Mat drawing = Mat::zeros(alpha_layer.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		Rect rect = boundingRect(contours[i]);
		rectangle(layers[3], rect, color, 1);
	}
	imshow("alpha left", layers[3]);


	// 从原图中取出矩形
	vector<Mat> roi_vec;
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);
		if (rect.width < 16 || rect.height < 16 ||
			rect.width / rect.height < 0.3)
		{
			continue;
		}
		roi_vec.push_back(roi(rect));
	}

	// ocr 需要异步
	auto res = ocr_call(roi_vec);
	return res;
}
bool func_test_picking_ocr(const cv::Mat frame)
{
	auto res_left = get_left_object(frame);
	auto res_right = get_right_object(frame);
	
	for (auto& s : res_left)
	{
		cout <<"picked: "<< s << "\n";
	}
	for (auto& s : res_right)
	{
		cout << "can pick: " << s << "\n";
	}

	//cv::imshow("asd", roi);
	cv::waitKey(1);
	return true;
}

GEN_FUNC(picking_ocr);
