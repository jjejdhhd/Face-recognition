#pragma once
#include "afxdialogex.h"
#include "ButtonPNG.h"

// WinRegister 对话框

class WinRegister : public CDialogEx
{
	DECLARE_DYNAMIC(WinRegister)

public:
	WinRegister(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinRegister();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACE_RECOGNITION_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CImage m_imgBG;                             // 定义“注册窗口”左侧的背景图片//
    ButtonPNG m_btnCamera;                      // 定义“刷脸按钮”变量
    CEdit m_editName;                           // 定义“名字编辑框”变量
    CStatic m_imgSnow_single;                   // 定义单张雪花图片变量
    HBITMAP m_imgsnows[16];                     // 定义存储所有雪花图片的数组//
    CStatic m_imgCamera_single;                 // 定义来自摄像头的显示图片

    BOOL OnInitDialog();                        // 定义初始化函数//
    afx_msg void OnPaint();                     // 窗口的绘制函数
    afx_msg void OnTimer(UINT_PTR nIDEvent);    // 定时器函数
    afx_msg void OnBnClickedButton1();          // “刷脸按钮”的跳转函数
};
