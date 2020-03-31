#pragma once
#include "afxwin.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//
class WheelZoom
{
public:
	WheelZoom();

	~WheelZoom();
	void Get(cv::Mat &dst, cv::Mat &newSrcImg = cv::Mat(), cv::Scalar bgColor = cv::Scalar(0, 0, 0));
	bool Reponse(CPoint pt, double k);

	void Set(CWnd* wnd, cv::Mat &srcImg);

	double m_sdR;
	cv::Point2d m_center;

private:
	cv::Mat m_srcImg;
	CWnd* m_pWnd;
	bool RectCut(cv::Mat src, cv::Mat &roi, cv::Rect2d &winRect);

	cv::Rect2d m_rect0;
	cv::Rect2d m_rect1;
	void FreshWin(CPoint &pt);
	void CompactRect(cv::Rect2d base, cv::Rect2d rect1, cv::Rect2d &rect2);
	void RectZoom(cv::Rect2d rect1, cv::Point2d p, double k, cv::Rect2d &rect2);
	void RelativeRect(cv::Rect2d base, cv::Rect2d rect1, cv::Rect2d &rect2);
	void RectInt(cv::Rect2d &rect);
};