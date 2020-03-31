
// MFCApplicationWheelDlg.h : ͷ�ļ�
//

#pragma once

#include "WheelZoom.h"
// CMFCApplicationWheelDlg �Ի���
class CMFCApplicationWheelDlg : public CDialogEx
{
// ����
public:
	CMFCApplicationWheelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATIONWHEEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
