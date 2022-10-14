#pragma once
#include "gi_test.h"

bool func_test_diff_match(const cv::Mat frame)
{
	static cv::Mat last_minimap_mat;
	static cv::Point2d pos_minimap = { 0,0 };
	if (frame.empty())
	{
		std::cout << "frame is empty" << std::endl;
		return false;
	}
	
	int MiniMap_Rect_x = cvRound(1920 * 0.033)+44;
	int MiniMap_Rect_y = cvRound(1920 * 0.01)+44;
	int MiniMap_Rect_w = cvRound(1920 * 0.11)-88;
	int MiniMap_Rect_h = cvRound(1920 * 0.11)-88;
	
	cv::Mat now_minimap_mat = frame(cv::Rect(MiniMap_Rect_x, MiniMap_Rect_y, MiniMap_Rect_w, MiniMap_Rect_h));
	// 去除中心半径为12的圆形区域
	cv::circle(now_minimap_mat, cv::Point2d(MiniMap_Rect_w / 2, MiniMap_Rect_h / 2), 12, cv::Scalar(128,128,128,128), -1);

	if (last_minimap_mat.empty())
	{
		last_minimap_mat = now_minimap_mat.clone();
		std::cout << "last_minimap_mat is empty" << std::endl;
		return false;
	}
	if (last_minimap_mat.size != now_minimap_mat.size)
	{
		last_minimap_mat = now_minimap_mat.clone();
		std::cout << "last_minimap_mat.size != now_minimap_mat.size" << std::endl;
		return false;
	}
	cv::Mat now_minimap_mat64;
	cv::Mat last_minimap_mat64;
	cv::cvtColor(now_minimap_mat, now_minimap_mat64, cv::COLOR_RGBA2GRAY);
	cv::cvtColor(last_minimap_mat, last_minimap_mat64, cv::COLOR_RGBA2GRAY);
	
	now_minimap_mat64.convertTo(now_minimap_mat64, CV_32FC1);
	last_minimap_mat64.convertTo(last_minimap_mat64, CV_32FC1);
	
	auto diff_pos = cv::phaseCorrelate(now_minimap_mat64, last_minimap_mat64);
	diff_pos = diff_pos - cv::Point2d(0.5, 0.5);
	last_minimap_mat = now_minimap_mat.clone();

	if (diff_pos.x != 0 || diff_pos.y != 0)
	{
		// std::cout << "diff_pos.x = " << diff_pos.x << " diff_pos.y = " << diff_pos.y << std::endl;
		pos_minimap += diff_pos;
		std::cout << "pos_minimap = " << pos_minimap.x << " , " << pos_minimap.y << std::endl;
		// 小地图叠加绘制展示
		static cv::Mat minimap_mat(500, 500, CV_8UC4, cv::Scalar(0, 0, 0, 0));
		static cv::Point center(250, 250);
		static bool is_frist = true;
		if (is_frist)
		{
			is_frist = false;
			// 拷贝小地图到偏移位置
			cv::Mat minimap_mat_roi = minimap_mat(cv::Rect(static_cast<int>(250 - MiniMap_Rect_w / 2), static_cast<int>(250 - MiniMap_Rect_h / 2), MiniMap_Rect_w, MiniMap_Rect_h));
			now_minimap_mat.copyTo(minimap_mat_roi);
		}
		
		cv::Point2d pos_minimap_draw = pos_minimap + cv::Point2d(center);
		static std::vector<cv::Point2d> pos_history_draw;
		static int count = 0;
		count++;
		if (count == 10)
		{
			count = 0;
			pos_history_draw.push_back(pos_minimap_draw);
			if (pos_history_draw.size() > 100)
			{
				pos_history_draw.erase(pos_history_draw.begin());
			}
		}
		
		try {

			// 绘制偏移位置
			cv::Mat minimap_mat_roi = minimap_mat(cv::Rect(static_cast<int>(pos_minimap_draw.x - MiniMap_Rect_w / 2), static_cast<int>(pos_minimap_draw.y - MiniMap_Rect_h / 2), MiniMap_Rect_w, MiniMap_Rect_h));
			cv::addWeighted(minimap_mat_roi, 0.5, now_minimap_mat, 0.5, 0, minimap_mat_roi);

		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		//cv::line(minimap_mat, center, pos_minimap_draw, cv::Scalar(0, 0, 255), 2);
		for (auto pos : pos_history_draw)
		{
			//cv::line(minimap_mat, center, pos, cv::Scalar(0, 0, 255), 2);
			cv::circle(minimap_mat, pos, 2, cv::Scalar(0, 255, 255), 2);
		}
		cv::imshow("minimap_mat", minimap_mat);
		cv::waitKey(1);
		

		return true;
	}
	
	return false;
}

void test_diff_match()
{
	test(func_test_diff_match,30);
}
