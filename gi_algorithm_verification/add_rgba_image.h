#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

/// <summary>
/// 叠加 RGBA 图像，以 正片叠底 的方式
/// </summary>
/// <param name="src1">底图 RGBA</param>
/// <param name="src2">要叠加图像 RGBA</param>
/// <param name="dst">输出图像 RGBA</param>
/// <param name="alpha">要叠加图像的透明度 0-1</param>
void add_rgba_image_1st(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst, double alpha = 1)
{
	assert(src1.size() != src2.size());
	assert(src1.channels() == 4 && src2.channels() == 4);
	
	// 5. 创建输出图像
	dst = cv::Mat(src1.size(), CV_8UC4);
	
	// 6. 根据透明通道叠加 Color = Src1.rgb * Src1.a + Src2.rgb * Src2.a * alpha
	std::vector<cv::Mat> src1_split;
	std::vector<cv::Mat> src2_split;
	std::vector<cv::Mat> dst_merge;
	
	cv::split(src1, src1_split);
	cv::split(src2, src2_split);
	
	cv::Mat alpha_dst;
	alpha_dst = src1_split[3] + src2_split[3] * alpha;
	
	for (int i = 0; i < 3; i++)
	{
		cv::Mat dst_src1;
		cv::multiply(src1_split[i], src1_split[3], dst_src1,1.0/255.0);
		cv::Mat dst_src2;
		cv::multiply(src2_split[i], src2_split[3], dst_src2, alpha/255.0);
		cv::Mat dst_channel;
		dst_channel = dst_src1 + dst_src2;
		dst_merge.push_back(dst_channel);
	}
	dst_merge.push_back(alpha_dst);
	cv::merge(dst_merge, dst);
}
void add_rgba_image_2st(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, double alpha)
{
	//assert(src1.size() != src2.size());
	assert(src1.channels() == 4 && src2.channels() == 4);

	// 根据透明通道叠加 Color = Src1.rgb * Src1.a + Src2.rgb * Src2.a * alpha

	std::vector<cv::Mat> src1_split;
	std::vector<cv::Mat> src2_split;
	std::vector<cv::Mat> dst_merge;

	cv::split(src1, src1_split);
	cv::split(src2, src2_split);

	for (int i = 0; i < 3; i++)
	{
		cv::Mat dst_src1 = src1_split[i].mul(src1_split[3], 1.0 / 255.0);
		cv::Mat dst_src2 = src2_split[i].mul(src2_split[3], alpha / 255.0);
		cv::Mat dst_channel = dst_src1 + dst_src2;
		dst_merge.push_back(dst_channel);
	}

	cv::Mat alpha_dst = src1_split[3] + src2_split[3] * alpha;
	// cv::scaleAdd(src2_split[3], alpha, src1_split[3], alpha_dst);
	dst_merge.push_back(alpha_dst);

	cv::merge(dst_merge, dst);
}
void add_rgba_image(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, double alpha)
{
	add_rgba_image_2st(src1, src2, dst, alpha);
}
void test_add_rgba_image()
{
	// 1. 读取图像
	cv::Mat src1 = cv::imread("001.png",-1);
	cv::Mat src2 = cv::imread("002.png",-1);

	// 2. 检查图像是否读取成功
	if (src1.empty() || src2.empty())
	{
		std::cout << "test_add_rgba_image: src1.empty() || src2.empty()" << std::endl;
		return;
	}
	cv::cvtColor(src1, src1, COLOR_BGR2BGRA);

	// 3. 创建输出图像
	cv::Mat dst;
	cv::Mat src_roi = src1(cv::Rect(cv::Point(0, 0), src2.size()));
	// 4. 叠加图像
	add_rgba_image(src_roi, src2, dst, 1);

	// 5. 显示图像
	cv::imshow("src1", src1);
	cv::imshow("src2", src2);
	cv::imshow("dst", dst);

	// 6. 等待按键
	cv::waitKey(0);
}