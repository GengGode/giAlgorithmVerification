#pragma once
#include "gi_test.h"

struct INS
{
	double x;
	double y;
	double dx;
	double dy;
};
struct GPS
{
	double x;
	double y;
	double dx;
	double dy;
};
struct FUSION
{
	double x;
	double y;
	double dx;
	double dy;
};
bool ins_gps_data_fusion(const INS& ins, const GPS& gps, double& x, double& y)
{
	//x = ins.x + ins.dx - gps.x - gps.dx;
	//y = ins.y + ins.dy - gps.y - gps.dy;
	
	// 基于kalman filter的ins和gps数据融合
	// 分别使用kalman filter对ins和gps数据进行滤波
	// 将滤波后的ins和gps数据进行融合
	// 从而得到更加准确的位置信息
	// 1. ins数据滤波
	// 2. gps数据滤波
	// 3. ins和gps数据融合
	// 4. 融合后的数据进行滤波
	// 5. 返回融合后的数据
	
	// ins数据滤波
	static cv::KalmanFilter ins_kf(4, 2, 0);
	static bool ins_kf_init = false;
	if (!ins_kf_init)
	{
		ins_kf_init = true;
		ins_kf.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
		ins_kf.statePre.at<float>(0) = ins.x;
		ins_kf.statePre.at<float>(1) = ins.y;
		ins_kf.statePre.at<float>(2) = ins.dx;
		ins_kf.statePre.at<float>(3) = ins.dy;
		ins_kf.measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
		ins_kf.processNoiseCov = (cv::Mat_<float>(4, 4) << 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2);
		ins_kf.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1e-1, 0, 0, 1e-1);
		ins_kf.errorCovPost = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}
	cv::Mat ins_measurement = (cv::Mat_<float>(2, 1) << ins.x, ins.y);
	cv::Mat ins_predict = ins_kf.predict();
	cv::Mat ins_correct = ins_kf.correct(ins_measurement);
	// gps数据滤波
	static cv::KalmanFilter gps_kf(4, 2, 0);
	static bool gps_kf_init = false;
	if (!gps_kf_init)
	{
		gps_kf_init = true;
		gps_kf.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
		gps_kf.statePre.at<float>(0) = gps.x;
		gps_kf.statePre.at<float>(1) = gps.y;
		gps_kf.statePre.at<float>(2) = gps.dx;
		gps_kf.statePre.at<float>(3) = gps.dy;
		gps_kf.measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
		gps_kf.processNoiseCov = (cv::Mat_<float>(4, 4) << 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2);
		gps_kf.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1e-1, 0, 0, 1e-1);
		gps_kf.errorCovPost = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}
	cv::Mat gps_measurement = (cv::Mat_<float>(2, 1) << gps.x, gps.y);
	cv::Mat gps_predict = gps_kf.predict();
	cv::Mat gps_correct = gps_kf.correct(gps_measurement);
	// ins和gps数据融合
	x = ins_correct.at<float>(0) + ins_correct.at<float>(2) - gps_correct.at<float>(0) - gps_correct.at<float>(2);
	y = ins_correct.at<float>(1) + ins_correct.at<float>(3) - gps_correct.at<float>(1) - gps_correct.at<float>(3);
	// 融合后的数据进行滤波
	static cv::KalmanFilter fusion_kf(4, 2, 0);
	static bool fusion_kf_init = false;
	if (!fusion_kf_init)
	{
		fusion_kf_init = true;
		fusion_kf.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
		fusion_kf.statePre.at<float>(0) = x;
		fusion_kf.statePre.at<float>(1) = y;
		fusion_kf.statePre.at<float>(2) = 0;
		fusion_kf.statePre.at<float>(3) = 0;
		fusion_kf.measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
		fusion_kf.processNoiseCov = (cv::Mat_<float>(4, 4) << 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2, 0, 0, 0, 0, 1e-2);
		fusion_kf.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1e-1, 0, 0, 1e-1);
		fusion_kf.errorCovPost = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}
	cv::Mat fusion_measurement = (cv::Mat_<float>(2, 1) << x, y);
	cv::Mat fusion_predict = fusion_kf.predict();
	cv::Mat fusion_correct = fusion_kf.correct(fusion_measurement);
	x = fusion_correct.at<float>(0) + fusion_correct.at<float>(2);
	y = fusion_correct.at<float>(1) + fusion_correct.at<float>(3);
	
	return true;
}
#include <iostream>
#include <fstream>
void read_data(std::vector<INS>& ins_data, std::vector<GPS>& gps_data)
{
	std::ifstream ins_file("ins.txt");
	std::ifstream gps_file("gps.txt");
	if (!ins_file.is_open() || !gps_file.is_open())
	{
		std::cout << "open file failed!" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(ins_file, line))
	{
		std::istringstream iss(line);
		INS ins;
		iss >> ins.x >> ins.y >> ins.dx >> ins.dy ;
		ins_data.push_back(ins);
	}
	while (std::getline(gps_file, line))
	{
		std::istringstream iss(line);
		GPS gps;
		iss >> gps.x >> gps.y >> gps.dx >> gps.dy ;
		gps_data.push_back(gps);
	}
}
void data_fusion(const INS& ins, const GPS& gps, FUSION& fusion)
{
	fusion.x = ins.x + ins.dx - gps.x - gps.dx;
	fusion.y = ins.y + ins.dy - gps.y - gps.dy;
	
	
}
void save_data(std::vector<FUSION>& fusion_data)
{
	std::ofstream fusion_file("fusion.txt");
	if (!fusion_file.is_open())
	{
		std::cout << "open file failed!" << std::endl;
		return;
	}
	for (auto& fusion : fusion_data)
	{
		fusion_file << fusion.x << " " << fusion.y << " " << fusion.dx << " " << fusion.dy << " " << std::endl;
	}
}

void test_data_fusion()
{
	// 读取数据
	std::vector<INS> ins_data;
	std::vector<GPS> gps_data;
	read_data(ins_data, gps_data);
	// 数据融合
	std::vector<FUSION> fusion_data;
	for (int i = 0; i < ins_data.size(); i++)
	{
		FUSION fusion;
		data_fusion(ins_data[i], gps_data[i], fusion);
		fusion_data.push_back(fusion);
	}
	// 保存数据
	save_data(fusion_data);
}