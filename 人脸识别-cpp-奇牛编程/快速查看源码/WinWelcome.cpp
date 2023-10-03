// WinWelcome.cpp: 实现文件
//

#include "pch.h"
#include "face_recognition.h"
#include "afxdialogex.h"
#include "WinWelcome.h"
#include "ButtonPNG.h" // LoadPicture


// WinWelcome 对话框

IMPLEMENT_DYNAMIC(WinWelcome, CDialogEx)

WinWelcome::WinWelcome(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WELCOME, pParent)
{
    // 设置窗口图标
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // 关于MFC窗口图标：https://blog.csdn.net/deep_kang/article/details/72722692
}

WinWelcome::~WinWelcome()
{
}

void WinWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL WinWelcome::OnInitDialog()
{
    // 父类的(同名)初始化方法
    CDialogEx::OnInitDialog();

    // 设置注册窗口的大小
    SetWindowPos(NULL, 0, 0, 1080 + 25, 609 + 70, SWP_NOMOVE);

    // 设置注册窗口的标题
    SetWindowText(L"欢迎窗口");

    // 初始化注册窗口左侧的背景图片
    LoadPicture(m_imgBG, IDB_PNG15);
    return 0;
}


BEGIN_MESSAGE_MAP(WinWelcome, CDialogEx)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// WinWelcome 消息处理程序


void WinWelcome::OnPaint()
{
    // 绘制背景图片
    drawPicOnPait(&m_imgBG, this, 0, 0); // 在当前窗口(欢迎窗口)的(0,0)位置绘制
}
