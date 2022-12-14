#pragma once
#include "gi_test.h"

cv::Point2d calc_diff_pos(cv::Mat& now_minimap_mat_f64, cv::Mat& last_minimap_mat_f64)
{
	auto diff_pos = cv::phaseCorrelate(now_minimap_mat_f64, last_minimap_mat_f64);
	return diff_pos - cv::Point2d(0.5, 0.5);
}
cv::Point2d calc_diff_pos_match_surf(cv::Mat& now_minimap_mat_f64, cv::Mat& last_minimap_mat_f64)
{
	// SURF
	cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create(1, 1, 1);
	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
	cv::Mat descriptors_object, descriptors_scene;
	surf->detectAndCompute(now_minimap_mat_f64, cv::noArray(), keypoints_object, descriptors_object);
	surf->detectAndCompute(last_minimap_mat_f64, cv::noArray(), keypoints_scene, descriptors_scene);
	// Matching descriptor vectors using BFMatcher
	cv::BFMatcher matcher;
	std::vector<cv::DMatch> matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	// Draw matches
	cv::Mat img_matches;
	cv::drawMatches(now_minimap_mat_f64, keypoints_object, last_minimap_mat_f64, keypoints_scene, matches, img_matches);
	// Show detected matches
	cv::imshow("Good Matches & Object detection", img_matches);
	// Localize the object
	std::vector<cv::Point2f> obj;
	std::vector<cv::Point2f> scene;
	for (int i = 0; i < matches.size(); i++)
	{
		// Get the keypoints from the good matches
		obj.push_back(keypoints_object[matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[matches[i].trainIdx].pt);
	}
	cv::Mat H = cv::findHomography(obj, scene, cv::RANSAC);
	// Get the corners from the image_1 ( the object to be "detected" )
	std::vector<cv::Point2f> obj_corners(4);
	obj_corners[0] = cv::Point(0, 0); obj_corners[1] = cv::Point(now_minimap_mat_f64.cols, 0);
	obj_corners[2] = cv::Point(now_minimap_mat_f64.cols, now_minimap_mat_f64.rows); obj_corners[3] = cv::Point(0, now_minimap_mat_f64.rows);
	std::vector<cv::Point2f> scene_corners(4);
	cv::perspectiveTransform(obj_corners, scene_corners, H);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	cv::Point2f diff_pos = scene_corners[0] - obj_corners[0];
	return diff_pos;
}

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
	// ??????????????12??????????
	cv::circle(now_minimap_mat, cv::Point2d(MiniMap_Rect_w / 2, MiniMap_Rect_h / 2), 15, cv::Scalar(128, 128, 128, 128), -1);
	cv::circle(now_minimap_mat, cv::Point2d(MiniMap_Rect_w / 2, MiniMap_Rect_h / 2), 87, cv::Scalar(128,128,128,128), 10);

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

	cv::Mat  now_minimap_mat_gray;
	cv::Mat last_minimap_mat_gray;
	cv::cvtColor(now_minimap_mat, now_minimap_mat_gray, cv::COLOR_RGBA2GRAY);
	cv::cvtColor(last_minimap_mat, last_minimap_mat_gray, cv::COLOR_RGBA2GRAY);
	cv::Mat now_minimap_mat_f64;
	cv::Mat last_minimap_mat_f64;
	now_minimap_mat_gray.convertTo(now_minimap_mat_f64,   CV_64FC1);
	last_minimap_mat_gray.convertTo(last_minimap_mat_f64, CV_64FC1);
	
	//auto diff_pos = cv::phaseCorrelate(now_minimap_mat_f64, last_minimap_mat_f64);
	//diff_pos = diff_pos - cv::Point2d(0.5, 0.5);
	
	//auto diff_pos = calc_diff_pos_match_surf(now_minimap_mat_gray, last_minimap_mat_gray);
	auto diff_pos = calc_diff_pos(now_minimap_mat_f64, last_minimap_mat_f64);


	last_minimap_mat = now_minimap_mat.clone();

	//if (diff_pos.x != 0 || diff_pos.y != 0)
	//{
		// std::cout << "diff_pos.x = " << diff_pos.x << " diff_pos.y = " << diff_pos.y << std::endl;
		pos_minimap += diff_pos;
		std::cout << "pos_minimap = " << pos_minimap.x << " , " << pos_minimap.y << std::endl;
		// ??????????????????
		static cv::Mat minimap_mat(500, 500, CV_8UC4, cv::Scalar(0, 0, 0, 0));
		static cv::Point center(250, 250);
		static bool is_frist = true;
		if (is_frist)
		{
			is_frist = false;
			// ????????????????????
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

			// ????????????
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
	//}
	//return false;
}

//void test_diff_match()
//{
//	test(func_test_diff_match,30);
//}

GEN_FUNC(diff_match)
//GEN_FUNC_LOCAL(diff_match)
