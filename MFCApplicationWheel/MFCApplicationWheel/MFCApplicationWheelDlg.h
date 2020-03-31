
// MFCApplicationWheelDlg.h : 头文件
//

#pragma once

#include "WheelZoom.h"
// CMFCApplicationWheelDlg 对话框
class CMFCApplicationWheelDlg : public CDialogEx
{
// 构造
public:
	CMFCApplicationWheelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATIONWHEEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void ShowMat(cv::Mat img, UINT nID);
	WheelZoom m_wheel;
};
