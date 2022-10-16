#pragma once
#include "gi_test.h"


bool func_test_uneven_light(const cv::Mat frame)
{
	if (frame.empty())
	{
		std::cout << "frame is empty" << std::endl;
		return false;
	}

	int MiniMap_Rect_x = cvRound(1920 * 0.033);
	int MiniMap_Rect_y = cvRound(1920 * 0.01);
	int MiniMap_Rect_w = cvRound(1920 * 0.11);
	int MiniMap_Rect_h = cvRound(1920 * 0.11);

	cv::Mat minimap_mat = frame(cv::Rect(MiniMap_Rect_x, MiniMap_Rect_y, MiniMap_Rect_w, MiniMap_Rect_h));
	cv::Mat minimap_mat_rgb;
	cv::cvtColor(minimap_mat, minimap_mat_rgb, COLOR_RGBA2RGB);
	cv::Mat minimap_mat_gray;
	cv::cvtColor(minimap_mat, minimap_mat_gray, COLOR_RGBA2GRAY);

	std::vector<cv::Mat> minimap_mat_split;
	cv::split(minimap_mat, minimap_mat_split);
	
	cv::Mat minimap_mat_hsl;
	cv::cvtColor(minimap_mat_rgb, minimap_mat_hsl, COLOR_RGB2HLS);
	std::vector<cv::Mat> minimap_mat_hsl_split;
	cv::split(minimap_mat_hsl, minimap_mat_hsl_split);

	cv::Mat minimap_mat_hsv;
	cv::cvtColor(minimap_mat_rgb, minimap_mat_hsv, COLOR_RGB2HSV);

	std::vector<cv::Mat> minimap_mat_hsv_split;
	cv::split(minimap_mat_hsv, minimap_mat_hsv_split);

	auto hsl_l = minimap_mat_hsl_split[0];

	// test_l = minimap_mat_gray / hsl_l
	cv::Mat test_l;
	cv::divide(minimap_mat_gray, hsl_l, test_l,10);
	



	
	
	
	
	cv::imshow("minimap_mat", minimap_mat);
	cv::waitKey(1);
	
	
	return false;
}

// GEN_FUNC(uneven_light)
GEN_FUNC_LOCAL(uneven_light)
