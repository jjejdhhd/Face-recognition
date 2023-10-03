#pragma once
#include "afxdialogex.h"
#include "ButtonPNG.h"
#include "VideoPlayer.h"

// WinHome 对话框
class WinHome : public CDialogEx {
	DECLARE_DYNAMIC(WinHome)
public:
	WinHome(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinHome();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    // HOME窗口的控件
    CStatic m_img_headpic;
    CStatic m_text_name;
    CStatic m_text_job;
    CStatic m_text_yanzhi;
    CStatic m_text_q_coin;
    CStatic m_text_id;
    ButtonPNG m_btn_play_pause;
    CStatic m_img_video;
    CImage cimg_head;     // 左上角的头像显示需要先读取
    CImage cimg_video_bg; // 视频显示的初始化背景
    CImage btn_bg;        // 按钮背景，防止按下按钮之后就看不见按钮了
    
    // 存储要显示的用户信息
    char user_name[64]; // HOME窗口要显示的姓名
    char user_job[64];  // HOME窗口要显示的工作
    int  user_yanzhi;   // HOME窗口要显示的颜值

    // HOME窗口的函数
    BOOL OnInitDialog(); // 定义初始化函数
    afx_msg void OnPaint();
    afx_msg void OnBnClickedHomePlayPause();

    // 存储播放器相关的
    VideoPlayer m_player; // VideoPlayer.h封装好的播放器类型
    int status_player{ 0 }; // 播放器状态
};
