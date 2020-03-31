#include"stdafx.h"
#include"WheelZoom.h"

WheelZoom::WheelZoom()
{

}

WheelZoom::~WheelZoom()
{
}


void WheelZoom::Set(CWnd* wnd, cv::Mat &srcImg)
{
	m_srcImg = srcImg;
	m_pWnd = wnd;
	CRect rect;
	m_pWnd->GetWindowRect(&rect);
	m_rect0 = cv::Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	m_rect1.x = 0;
	m_rect1.y = 0;
	m_rect1.width = srcImg.cols;
	m_rect1.height = srcImg.rows;

	CompactRect(m_rect0, m_rect1, m_rect1);

	cv::Rect2d winRect;
	cv::Mat roi;
	RectCut(srcImg, roi, winRect);
}

bool WheelZoom::Reponse(CPoint pt, double k)
{
	bool re = false;
	FreshWin(pt);
	cv::Point p(pt.x, pt.y);
	re = m_rect0.contains(p);
	if (re)
	{
		RectZoom(m_rect1, p, k, m_rect1);

		cv::Rect2d winRect;
		cv::Mat roi;
		RectCut(m_srcImg, roi, winRect);
	}

	return re;
}

void WheelZoom::Get(cv::Mat &dst, cv::Mat &newSrcImg, cv::Scalar bgColor)
{
	cv::Mat src;
	if (newSrcImg.empty())
	{
		src = m_srcImg;
	}
	else
	{
		src = newSrcImg;
	}
	cv::Rect2d winRect;

	cv::Mat temp = cv::Mat(m_rect0.height, m_rect0.width, src.type(), bgColor);
	cv::Mat roi;
	if (RectCut(src, roi, winRect))
	{
		roi.copyTo(temp(winRect));
	}

	dst = temp.clone();
}

void WheelZoom::FreshWin(CPoint &pt)
{
	CRect rect;
	m_pWnd->GetParent()->GetWindowRect(&rect);
	pt.x -= rect.left;
	pt.y -= rect.top;

}

void WheelZoom::CompactRect(cv::Rect2d base, cv::Rect2d rect1, cv::Rect2d &rect2)
{
	cv::Rect2d temp;
	if (base.width*rect1.height < base.height*rect1.width)
	{
		temp.x = base.x;
		temp.width = base.width;
		temp.height = float(rect1.height)*float(base.width) / float(rect1.width);
		temp.y = base.y + 0.5*(base.height - temp.height);
	}
	else if (base.width*rect1.height > base.height*rect1.width)
	{
		temp.y = base.y;
		temp.height = base.height;
		temp.width = float(rect1.width)*float(base.height) / float(rect1.height);
		temp.x = base.x + 0.5*(base.width - temp.width);
	}
	else
	{
		temp = base;
	}
	rect2 = temp;
}

void WheelZoom::RectZoom(cv::Rect2d rect1, cv::Point2d p, double k, cv::Rect2d &rect2)
{
	cv::Rect2d temp;

	temp.x = k*(rect1.x - p.x) + p.x;
	temp.y = k*(rect1.y - p.y) + p.y;
	temp.width = k*rect1.width;
	temp.height = k*rect1.height;
	//if (temp.width == rect1.width)
	//{
	//	temp.width++;
	//}
	//if (temp.height == rect1.height)
	//{
	//	temp.height++;
	//}
	if (temp.width < 10 || temp.height < 10)
	{
		temp = rect1;
	}

	rect2 = temp;
}


void WheelZoom::RelativeRect(cv::Rect2d base, cv::Rect2d rect1, cv::Rect2d &rect2)
{
	rect2.x = rect1.x - base.x;
	rect2.y = rect1.y - base.y;
	rect2.width = rect1.width;
	rect2.height = rect1.height;

}

void WheelZoom::RectInt(cv::Rect2d &rect)
{
	rect.x = int(rect.x);
	rect.y = int(rect.y);
	rect.width = int(rect.width);
	rect.height = int(rect.height);
}

bool WheelZoom::RectCut(cv::Mat src, cv::Mat &roi, cv::Rect2d &winRect)
{
	cv::Rect2d rect = m_rect0&m_rect1;

	if (rect.width > 0 && rect.height > 0)
	{
		RelativeRect(m_rect0, rect, winRect);


		winRect.x = round(winRect.x);
		winRect.y = round(winRect.y);
		winRect.width = round(winRect.width);
		winRect.height = round(winRect.height);
		//RectInt(winRect);
		//RectInt(imgRect);

		if (winRect.x < 0 || winRect.y < 0 || winRect.width <= 0 || winRect.height <= 0)
		{
			return false;
		}

		if (winRect.x + winRect.width > m_rect0.width || winRect.y + winRect.height > m_rect0.height)
		{
			return false;
		}

		cv::Rect2d imgRect, imgCutRect;
		RelativeRect(m_rect1, rect, imgRect);

		m_center = cv::Point2d(m_rect1.x - m_rect0.x + 0.5*m_rect1.width, m_rect1.y - m_rect0.y + 0.5*m_rect1.height);

		m_sdR = double(m_rect1.width) / double(src.cols);

		imgCutRect.x = floor(imgRect.x / m_sdR);
		imgCutRect.y = floor(imgRect.y / m_sdR);
		imgCutRect.width = ceil(imgRect.width / m_sdR) + 1;
		imgCutRect.height = ceil(imgRect.height / m_sdR) + 1;

		if (imgCutRect.x < 0 || imgCutRect.y < 0 || imgCutRect.width <= 0 || imgCutRect.height <= 0)
		{
			return false;
		}
		if (imgCutRect.x + imgCutRect.width > src.cols)
		{
			imgCutRect.width = src.cols - imgCutRect.x;

		}
		if (imgCutRect.y + imgCutRect.height > src.rows)
		{
			imgCutRect.height = src.rows - imgCutRect.y;
		}
		cv::Mat roiSrc;
		src(imgCutRect).copyTo(roiSrc);
		cv::resize(roiSrc, roiSrc, cv::Size(0, 0), m_sdR, m_sdR);
		int rx, ry;
		rx = round(imgRect.x - imgCutRect.x*m_sdR);
		ry = round(imgRect.y - imgCutRect.y*m_sdR);
		rx = (rx<0 ? 0 : rx);
		ry = (ry<0 ? 0 : ry);

		roiSrc(cv::Rect(rx, ry, winRect.width, winRect.height)).copyTo(roi);

	}
	else
	{
		return false;
	}
	return true;
}
