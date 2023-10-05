#pragma once
#include "afxdialogex.h"
#include "ButtonPNG.h"

// WinLogin 对话框
class WinLogin : public CDialogEx
{
	DECLARE_DYNAMIC(WinLogin)

public:
	WinLogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinLogin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACE_RECOGNITION_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    ButtonPNG m_btnCamera;      // 定义“刷脸”按钮
    CImage m_imgBG;             // 定义“登录窗口”的背景图片//
    CStatic m_imgSnow_signal;   // 雪花图片显示控件
    HBITMAP m_imgsnows[16];     // 定义雪花图片数组//
    CStatic m_imgCamera_single; // 摄像机显示画面

    BOOL OnInitDialog();                     // 从父类的父类复制过来的初始化函数
    afx_msg void OnPaint();                  // 绘制函数
    afx_msg void OnTimer(UINT_PTR nIDEvent); // 定时器函数
    afx_msg void OnBnClickedButton1();       // 刷脸按钮操作

public:
    char user_job[64];          // 用户的工作
    char user_name[64];         // 用户的名称
    int  user_yanzhi;           // 用户的颜值
    bool user_logined{ false }; // 用户是否登陆成功
};
