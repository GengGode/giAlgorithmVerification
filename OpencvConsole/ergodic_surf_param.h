#pragma once
#include <opencv2/opencv.hpp>

std::pair<cv::Mat, int> calc_keypoint(const cv::Mat& img, int h, int l, int c)
{
	// SURF 特征点检测
	cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(h, l, c);

	std::vector<cv::KeyPoint> keypoints;
	detector->detect(img, keypoints);

	cv::Mat keypoint_img;
	cv::drawKeypoints(img, keypoints, keypoint_img, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

	return std::make_pair(keypoint_img, keypoints.size());
}

void ergodlc_surf_parameter()
{
	cv::Mat img = cv::imread("test.png", -1);

	std::map < std::tuple<int, int, int>, std::pair<cv::Mat, int>  > imgs;

	for (int h = 1; h <= 20; h += 1)
	{
		for (int l = 1; l <= 12; l += 1)
		{
			for (int c = 1; c <= 12; c += 1)
			{
				imgs[std::make_tuple(h, l, c)] = calc_keypoint(img, h, l, c);
				// 构造文件名
				std::string filename = std::format("test_{}_{}_{}.png", h, l, c);
				cv::imwrite(filename, imgs[std::make_tuple(h, l, c)].first);
				// 输出到控制台
				std::cout << std::format("h:{} l:{} c:{} keypoints:{}\n", h, l, c, imgs[std::make_tuple(h, l, c)].second);
			}
		}
	}
}