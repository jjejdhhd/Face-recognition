// WinLogin.cpp: 实现文件
//

#include "pch.h"
#include "face_recognition.h"
#include "afxdialogex.h"
#include "WinLogin.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h> // 用到了cvGetWindowHandle

// 使用 Windows 多媒体 API 提供的音频和多媒体功能，包括播放声音、音乐和控制多媒体设备。
#include <mmsystem.h> // 该头文件包含了许多用于音频、视频、音乐和多媒体设备控制的函数和数据类型的声明。
#pragma comment(lib, "winmm.lib") // 将Windows多媒体库文件 winmm.lib 关联到程序中，以便调用多媒体API函数。

#include "faceTool.h" // 人脸识别模块

#include "WinWelcome.h" // 欢迎界面

#include <vector> // 人脸识别函数FaceCheck
#include <string>

using namespace cv;
static cv::VideoCapture cap_WinLog; // 定义登录窗口的摄像头(static只能本文件使用)
extern MHandle faceModel; // 人脸识别模块

// WinLogin 对话框

IMPLEMENT_DYNAMIC(WinLogin, CDialogEx)

WinLogin::WinLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACE_RECOGNITION_LOG, pParent)
{

}

WinLogin::~WinLogin()
{
}

BOOL WinLogin::OnInitDialog()
{
    CDialogEx::OnInitDialog();                                      // 父类的(同名)初始化方法
    SetWindowPos(NULL, 0, 0, 1080 + 25, 609 + 70, SWP_NOMOVE);      // 设置窗口大小
    SetWindowText(L"登录窗口");                                     // 设置窗口标题
    LoadPicture(m_imgBG, IDB_PNG3);                                 // 背景图片的初始化
    m_btnCamera.Init(IDB_PNG7, 4, BTN_TYPE_NORMAL);                 // “刷脸”按钮的初始化
    ::MoveWindow(m_btnCamera.m_hWnd, 220-90/2-8, 420, 90, 50, 0);   // 调整“刷脸”按钮的位置(背景图片440x610)

    // 初始化snow图片组
    //char filename_snow[256];
    CString filename_snows; // 存储文件名(MFC提供的类型)
    for (int i = 0; i < 16; i++) {
        //sprintf(filename_snow, "res/snow/snow_%d.bmp", i);
        filename_snows.Format(L"res/snow/snow_%d.bmp", i); // 定义每个图片的文件名
        m_imgsnows[i] = (HBITMAP)LoadImage(0, filename_snows, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); // 将对应的雪花图片存储到数组中
    }

    // 调整雪花视频显示的位置
    ::MoveWindow(m_imgSnow_signal.m_hWnd, 440, 0, 640, 609, 1); // 前面两个冒号表示使用全局

    // 启动定时器来播放雪花状文件
    SetTimer(1, 30, NULL); // 设置定时器1，每个30ms执行空函数NULL

    // 调整摄像头显示的图片控件的位置(440,64)和大小(640x480)
    ::MoveWindow(m_imgCamera_single.m_hWnd, 440, 64, 640, 480, 1);

    return 0;
}

void WinLogin::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON1, m_btnCamera);
    DDX_Control(pDX, IDC_IMG_SNOWS, m_imgSnow_signal);
    DDX_Control(pDX, IDC_STATIC_CAMERA_LOG, m_imgCamera_single);
}


BEGIN_MESSAGE_MAP(WinLogin, CDialogEx)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON1, &WinLogin::OnBnClickedButton1)
END_MESSAGE_MAP()


// WinLogin 消息处理程序


void WinLogin::OnPaint()
{
    //CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
    drawPicOnPait(&m_imgBG, this, 0, 0); // 在当前窗口(登录窗口)的(0,0)位置绘制IDB_PNG3
}

// 进行人脸识别
// 基本思路：将待识别的人脸 face_check 与user库中的所有图片进行一一对比，
//           然后返回相似度80%以上的图片名称。
BOOL FaceCheck(cv::Mat face_check, char* res_filename) {
    // 存放所有的文件名
    std::vector<CString> filename_all;
    WIN32_FIND_DATA filedata;
    HANDLE file = FindFirstFile(L"users/*.jpg", &filedata);// 找到users目录下的第一个文件
    if (file != INVALID_HANDLE_VALUE) {
        do {
            filename_all.push_back(filedata.cFileName);
        } while (FindNextFile(file, &filedata));
        // 逐个文件进行对比
        char filepath_single[100]; // 单个图片的库路径
        for (int i = 0; i < filename_all.size(); i++) {
            // 将 CString 转换成 char*，获取单个图片的库路径
            USES_CONVERSION;
            char* filename_char = T2A(filename_all[i]);
            sprintf_s(filepath_single, sizeof(filepath_single), "users/%s", filename_char);

            // 读取users库中的人脸
            cv::Mat face_USER = cv::imread(filepath_single, 1);

            // 进行人脸对比，并返回结果
            if (faceCompare(faceModel, face_check, face_USER) >= 0.80) {
                // 去掉后缀“.jpg”
                std::string tmp_str{ filename_char };
                std::string res_str = tmp_str.substr(0, tmp_str.size() - 4);
                // 返回识别到的人脸信息
                //strcpy_s(res_filename, sizeof(res_filename), res_str.c_str());
                strcpy_s(res_filename, sizeof(res_str)+1, res_str.c_str());
                return true;
            }
        }
    }
    else {
        ::MessageBox(NULL, _T("人脸库为空！\n请先进行注册！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
        //c++中MessageBox弹窗的用法大全：https://blog.csdn.net/LCR2025/article/details/129223538
    }
    return false;
}


// 自动生成定时器的中断函数
void WinLogin::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    static int snowIndex = 0; // 雪花图片的编号索引
    static int count_timer = 0; // 初始化定时器计数
    static char res_filename[100]{ "" }; // 人脸识别结果(图片的名称)
    if (nIDEvent == 1) {
        m_imgSnow_signal.SetBitmap(m_imgsnows[snowIndex]); // 更新图片控件的显示
        snowIndex = (snowIndex + 1) % 16; // 更新雪花图片索引。对16取余是因为16张雪花照片重复。
    }
    else if (nIDEvent == 2) {
        cv::Mat cam_frame;       // 定义摄像头单帧
        cv::Rect face_rect;      // 定义人脸区域
        cap_WinLog >> cam_frame; // 获取摄像头拍摄的单帧
        if (faceRegion(faceModel, cam_frame, face_rect)) {
            // 将图片中的人脸框出来
            cv::rectangle(cam_frame, face_rect, cv::Scalar(0, 255, 255), 2);
        }
        imshow("m_imgCamera_single", cam_frame); // 显示画面
        count_timer++;
        
        if (count_timer == 1000 / 20) { // 1000意为1000ms
            count_timer = 0;                        // 清零计数
            KillTimer(2);                           // 关闭定时器2
            cap_WinLog.release();                   // 关闭摄像头
            cv::destroyWindow("m_imgCamera_single");// 关闭摄像头显示窗口

            // 进行人脸识别
            if (FaceCheck(cam_frame, res_filename)) {
                // 播放登录成功提示音
                mciSendString(L"play res/login.mp3", 0, 0, 0);

                // 存储用户信息
                char* context = NULL;
                strcpy_s(user_job, sizeof(user_job), strtok_s(res_filename, "-", &context));
                strcpy_s(user_name, sizeof(user_name), strtok_s(NULL, "-", &context));
                user_yanzhi = atoi(strtok_s(NULL, "-", &context)); // 字符串转整数
                user_logined = true; // 表明用户成功登录

                // 正常关闭“登录窗口”
                //::MessageBox(NULL, _T("登录成功！"), _T("提示"), MB_OK | MB_ICONASTERISK);
                //c++中MessageBox弹窗的用法大全：https://blog.csdn.net/LCR2025/article/details/129223538
                CDialogEx::OnOK(); // 关闭后就会跳转到“启动窗口”中的“登录按钮”函数中
            }
            else {
                // 登陆失败提示窗口
                ::MessageBox(NULL, _T("登录失败！"), _T("错误"), MB_OK | MB_ICONHAND);
                //c++中MessageBox弹窗的用法大全：https://blog.csdn.net/LCR2025/article/details/129223538

                // 重新回到登录窗口
                user_logined = false;
                SetTimer(1, 30, NULL); // 启动雪花视频定时器
                return;
            }
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}


void WinLogin::OnBnClickedButton1()
{
    // 关闭雪花定时器
    KillTimer(1);
    // 将雪花图片更换成墙纸
    HBITMAP bg_wall = (HBITMAP)LoadImage(NULL, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_imgSnow_signal.SetBitmap(bg_wall);

    // 将opencv的窗体嵌入到图片控件m_imgCamera_single中
    cv::namedWindow("m_imgCamera_single", cv::WINDOW_AUTOSIZE);     // 打开一个opencv窗口，注意名称要与图片控件一致
    // 第二个选项是cv::WindowFlags：https://vovkos.github.io/doxyrest-showcase/opencv/sphinxdoc/enum_cv_WindowFlags.html
    HWND hWnd = (HWND)cvGetWindowHandle("m_imgCamera_single");      // 获取opencv窗口句柄
    HWND hParent = ::GetParent(hWnd);                               // 获取opencv窗口的父窗口句柄
    ::SetParent(hWnd, GetDlgItem(IDC_STATIC_CAMERA_LOG)->m_hWnd);   // 将opencv窗口的句柄设置为图片控件的句柄
    ::ShowWindow(hParent, SW_HIDE);                                 // 隐藏原父窗口

    // 打开默认摄像头0
    cap_WinLog.open(0);
    if (!cap_WinLog.isOpened()) {
        ::MessageBox(NULL, _T("摄像头打开失败！\n请检查摄像头是否正确连接并开启！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
        //c++中MessageBox弹窗的用法大全：https://blog.csdn.net/LCR2025/article/details/129223538
        return;
    }

    // 开启摄像头的定时器
    SetTimer(2, 20, NULL);
}
