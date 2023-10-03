
// face_recognitionDlg.h: 头文件
//

#pragma once
# include "ButtonPNG.h"

// CfacerecognitionDlg 对话框
class CfacerecognitionDlg : public CDialogEx
{
// 构造
public:
	CfacerecognitionDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACE_RECOGNITION_DIALOG };
#endif

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
    afx_msg void OnBnClickedButton5Log();
    ButtonPNG m_btnLogin; // “启动窗口”的“登录按钮”变量
    ButtonPNG m_btnReg;   // “启动窗口”的“注册按钮”变量
    CImage m_imgBG;       // “启动窗口”的“背景图片”变量
    afx_msg void OnBnClickedButtonReg();
};
