#pragma once
#include "gi_test.h"




bool func_test_calc_angle(const cv::Mat frame)
{

	int MiniMap_Rect_x = cvRound(1920 * 0.033);
	int MiniMap_Rect_y = cvRound(1920 * 0.01) ;
	int MiniMap_Rect_w = cvRound(1920 * 0.11) ;
	int MiniMap_Rect_h = cvRound(1920 * 0.11) ;

	cv::Mat minimap_mat = frame(cv::Rect(MiniMap_Rect_x, MiniMap_Rect_y, MiniMap_Rect_w, MiniMap_Rect_h));


	/* python
Alpha=imsrc[: , : , 3:]
Alpha = 255.0 - Alpha
Alpha = Alpha * 2

_,Alpha=cv2.threshold(Alpha, 503, 0, cv2.THRESH_TOZERO_INV)
_,Alpha=cv2.threshold(Alpha, 50, 0, cv2.THRESH_TOZERO)
_,Alpha=cv2.threshold(Alpha, 50, 255, cv2.THRESH_BINARY)

cv2.circle(Alpha, 
           (int(Alpha.shape[0] / 2), int(Alpha.shape[1] / 2)), int((min(int(Alpha.shape[0] / 2), int(Alpha.shape[1] / 2)) * 1.15)),# 1.21
            (0, 0, 0), int((min(int(Alpha.shape[0] / 2), int(Alpha.shape[1] / 2)) * 0.6)))# 0.42

cv2.circle(Alpha, 
           (int(Alpha.shape[0] / 2), int(Alpha.shape[1] / 2)), int((min(int(Alpha.shape[0] / 2), int(Alpha.shape[1] / 2)) * 0.6)), (0, 0, 0), -1)

dilate_element = cv2.getStructuringElement(cv2.MORPH_RECT, (4, 4))
Alpha=cv2.dilate(Alpha, dilate_element)
erode_element = cv2.getStructuringElement(cv2.MORPH_RECT, (4, 4))
Alpha=cv2.erode(Alpha, erode_element)

erode_element = cv2.getStructuringElement(cv2.MORPH_RECT, (4, 4))
Alpha=cv2.erode(Alpha, erode_element)
dilate_element = cv2.getStructuringElement(cv2.MORPH_RECT, (4, 4))
Alpha=cv2.dilate(Alpha, dilate_element)
Alpha=Alpha.astype('uint8')

contours, hierarcy=cv2.findContours(Alpha, 0, 1)
maxBlack=0
maxId=0
boundRect=[]
for i in range(len(contours)):
    boundRect.append([])
    if (len(contours[i]) > maxBlack):
        maxBlack = len(contours[i])
        maxId = i
    boundRect[i] = cv2.boundingRect(cv2.Mat(contours[i]))

if len(boundRect)==0:
    logger.warning('找不到小地图')
    return -1
x,y,w,h=boundRect[maxId]

p = [x+w/2, y+h/2]

origin_point=[int(Alpha.shape[0]/2)+1,int(Alpha.shape[1]/2)+1]
point=[p[0]-origin_point[0],-p[1]+origin_point[1]]
	*/
    // to c++
	cv::Mat Alpha;
	cv::extractChannel(minimap_mat, Alpha, 3);
	cv::subtract(cv::Scalar(255), Alpha, Alpha);
	cv::multiply(Alpha, 2, Alpha);
	cv::threshold(Alpha, Alpha, 503, 0, cv::THRESH_TOZERO_INV);
	cv::threshold(Alpha, Alpha, 50, 0, cv::THRESH_TOZERO);
	cv::threshold(Alpha, Alpha, 50, 255, cv::THRESH_BINARY);
	cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), min(Alpha.cols / 2, Alpha.rows / 2) * 1.15, cv::Scalar(0), min(Alpha.cols / 2, Alpha.rows / 2) * 0.6);
	cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), min(Alpha.cols / 2, Alpha.rows / 2) * 0.6, cv::Scalar(0), -1);
	cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::dilate(Alpha, Alpha, dilate_element);
	cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::erode(Alpha, Alpha, erode_element);
	cv::erode(Alpha, Alpha, erode_element);
	cv::dilate(Alpha, Alpha, dilate_element);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;
	cv::findContours(Alpha, contours, hierarcy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	int maxBlack = 0;
	int maxId = 0;
	std::vector<cv::Rect> boundRect;
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect.push_back(cv::Rect());
		if (contours[i].size() > maxBlack)
		{
			maxBlack = contours[i].size();
			maxId = i;
		}
		boundRect[i] = cv::boundingRect(contours[i]);
	}
	if (boundRect.size() == 0)
	{
		return false;
	}
	cv::Rect rect = boundRect[maxId];
	cv::Point p = cv::Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
	cv::Point origin_point = cv::Point(Alpha.cols / 2 + 1, Alpha.rows / 2 + 1);
	cv::Point point = cv::Point(p.x - origin_point.x, -p.y + origin_point.y);
	// end c++

	std::cout << "point: " << point << std::endl;
	
	return true;
}


//GEN_FUNC(calc_angle)
GEN_FUNC_LOCAL(calc_angle)