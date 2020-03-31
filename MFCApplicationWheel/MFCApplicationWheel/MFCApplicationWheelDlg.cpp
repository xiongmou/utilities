
// MFCApplicationWheelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplicationWheel.h"
#include "MFCApplicationWheelDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationWheelDlg 对话框



CMFCApplicationWheelDlg::CMFCApplicationWheelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplicationWheelDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationWheelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationWheelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplicationWheelDlg::OnBnClickedOk)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CMFCApplicationWheelDlg 消息处理程序

BOOL CMFCApplicationWheelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CRect rect;
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(&rect);

	cv::Mat src(960, 1280, CV_8UC3, cv::Scalar(0, 255, 255));
	m_wheel.Set(GetDlgItem(IDC_STATIC_VIEW), src);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplicationWheelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplicationWheelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationWheelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplicationWheelDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

#include "atlimage.h"
void MatToCImage(cv::Mat &mat, CImage &cImage)
{

	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();
	if (!cImage.IsNull())
	{
		cImage.Destroy(); //clear 
	}

	cImage.Create(width,
		height, //positive: left-bottom-up   or negative: left-top-down  
		8 * channels); //numbers of bits per pixel  
	if (1 == channels)
	{
		RGBQUAD* ColorTable;
		int MaxColors = 256;
		ColorTable = new RGBQUAD[MaxColors];
		cImage.GetColorTable(0, MaxColors, ColorTable);
		for (int i = 0; i<MaxColors; ++i)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cImage.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;
	}

	uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //A pointer to the bitmap buffer  

	//The pitch is the distance, in bytes. represent the beginning of   
	// one bitmap line and the beginning of the next bitmap line  
	int step = cImage.GetPitch();

	for (int i = 0; i < height; ++i)
	{
		ps = (mat.ptr<uchar>(i));
		for (int j = 0; j < width; ++j)
		{
			if (channels == 1) //gray  
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if (channels == 3) //color  
			{
				for (int k = 0; k < 3; ++k)
				{
					*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
				}
			}
		}
	}

}
void CMFCApplicationWheelDlg::ShowMat(cv::Mat img, UINT nID)
{

	if (img.data == NULL)
	{
		return;
	}

	CImage cimg;
	MatToCImage(img, cimg);

	CWnd* pWnd = GetDlgItem(nID);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(rect);

	int img_width = cimg.GetWidth(), img_height = cimg.GetHeight();
	int win_width = rect.Width(), win_height = rect.Height();
	float win_ratio = (float)win_width / win_height, img_ratio = (float)img_width / img_height;
	CRect CRectImgPos;
	if (win_ratio>img_ratio)
	{
		CRectImgPos.top = rect.top;
		CRectImgPos.bottom = rect.bottom;
		CRectImgPos.left = rect.left + (win_width - (img_ratio*win_height)) / 2;
		CRectImgPos.right = CRectImgPos.left + (img_ratio*win_height);
	}
	else if (win_ratio<img_ratio)
	{
		CRectImgPos.top = rect.top + (win_height - (win_width / img_ratio)) / 2;
		CRectImgPos.bottom = CRectImgPos.top + (win_width / img_ratio);
		CRectImgPos.left = rect.left;
		CRectImgPos.right = rect.right;
	}
	else
	{
		CRectImgPos = rect;
	}


	CDC* pDC = this->GetDC();
	SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	cimg.Draw(pDC->m_hDC, CRectImgPos);
	ReleaseDC(pDC);

}
BOOL CMFCApplicationWheelDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	double k = 1.0;
	if (zDelta > 0)
	{
		k = 1.1;
	}
	else
	{
		k = 1.0 / 1.1;
	}
	static int n = 1;
	
	if (m_wheel.Reponse(pt, k))
	{
		cv::Mat dst;
		cv::Mat src(960, 1280, CV_8UC3, cv::Scalar(n*10, 255/n, 255/n));
		cv::line(src, cv::Point(640, 0), cv::Point(640, src.rows), cv::Scalar(255, 0, 255),1);
		cv::line(src, cv::Point(0, 480), cv::Point(src.cols, 480), cv::Scalar(255, 0, 255),1);
		//cv::circle(src, cv::Point(640,480), 80 * m_wheel.m_sdR, cv::Scalar(255, 0, 255));
		m_wheel.Get(dst, src,cv::Scalar(n*10,n*10,n*10));
		

		cv::line(dst, cv::Point(round(m_wheel.m_center.x), 0), cv::Point(round(m_wheel.m_center.x), dst.rows), cv::Scalar(0, 0, 255));
		cv::line(dst, cv::Point(0, round(m_wheel.m_center.y)), cv::Point(dst.cols, round(m_wheel.m_center.y)), cv::Scalar(0, 0, 255));
		cv::circle(dst, m_wheel.m_center, 100 * m_wheel.m_sdR, cv::Scalar(0, 0, 255));
		

		ShowMat(dst, IDC_STATIC_VIEW);

		//n++;
		if (n > 20)
		{
			n = 1;
		}
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
