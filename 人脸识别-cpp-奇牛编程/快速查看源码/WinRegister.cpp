// WinRegister.cpp: 实现文件
//

#include "pch.h"
#include "face_recognition.h"
#include "afxdialogex.h"
#include "WinRegister.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h> // 用到了cvGetWindowHandle

// 使用 Windows 多媒体 API 提供的音频和多媒体功能，包括播放声音、音乐和控制多媒体设备。
#include <mmsystem.h> // 该头文件包含了许多用于音频、视频、音乐和多媒体设备控制的函数和数据类型的声明。
#pragma comment(lib, "winmm.lib") // 将Windows多媒体库文件 winmm.lib 关联到程序中，以便调用多媒体API函数。

#include "faceTool.h" // 人脸识别模块

#include "WinWelcome.h" // 欢迎界面

using namespace cv;
static cv::VideoCapture cap_WinReg; // 定义注册窗口的摄像头(static只能本文件使用)
extern MHandle faceModel; // 人脸识别模块


// WinRegister 对话框

IMPLEMENT_DYNAMIC(WinRegister, CDialogEx)

WinRegister::WinRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACE_RECOGNITION_REG, pParent)
{

}

WinRegister::~WinRegister()
{
}

void WinRegister::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON1, m_btnCamera);
    DDX_Control(pDX, IDC_EDIT1, m_editName);
    DDX_Control(pDX, IDC_IMG_SNOWS, m_imgSnow_single);
    DDX_Control(pDX, IDC_STATIC_CAMERA_REG, m_imgCamera_single);
}

// 本函数为“注册窗口”的初始化函数
BOOL WinRegister::OnInitDialog()
{
    // 父类的(同名)初始化方法
    CDialogEx::OnInitDialog();

    // 设置注册窗口的大小
    SetWindowPos(NULL, 0, 0, 1080 + 25, 609 + 70, SWP_NOMOVE);
    // 设置窗口标题
    SetWindowText(L"注册窗口");

    // 初始化注册窗口左侧的背景图片
    LoadPicture(m_imgBG, IDB_PNG20);

    // 设置“刷脸按钮”
    //m_btnCamera.Init(IDB_PNG7, 4, BTN_TYPE_NORMAL);        // “刷脸”按钮的初始化
    //::MoveWindow(m_btnCamera.m_hWnd, 170, 450, 90, 50, 0); // 调整“刷脸”按钮的位置(背景图片440x610)和大小(90x50)。
    m_btnCamera.Init(IDB_PNG21, 4, BTN_TYPE_NORMAL);        // “刷脸”按钮的初始化
    ::MoveWindow(m_btnCamera.m_hWnd, 140, 450, 150, 45, 0); // 调整“刷脸”按钮的位置(背景图片440x610)和大小(90x50)。

    // 设置“名字编辑框”
    CFont font;
    font.CreatePointFont(150, L"华文行楷", NULL);          // 定义字号150、字体为华文行楷
    m_editName.SetFont(&font);                             // 设置编辑框的字体
    ::MoveWindow(m_editName.m_hWnd, 140, 350, 150, 60, 0); // 调整编辑框位置和大小

    // 设置窗口右侧的雪花视频显示
    CString filename_snows; // 存储文件名(MFC提供的类型)
    for (int i = 0; i < 16; i++) {
        filename_snows.Format(L"res/snow/snow_%d.bmp", i); // 定义每个图片的文件名
        m_imgsnows[i] = (HBITMAP)LoadImage(0, filename_snows, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); // 将对应的雪花图片存储到数组中
    }
    ::MoveWindow(m_imgSnow_single.m_hWnd, 440, 0, 640, 609, 1); // 调整雪花视频显示的位置

    // 启动定时器来播放雪花状文件
    SetTimer(1, 30, NULL); // 设置定时器1，每个30ms执行空函数NULL

    // 调整摄像头显示的图片控件的位置(440,64)和大小(640x480)
    ::MoveWindow(m_imgCamera_single.m_hWnd, 440, 64, 640, 480, 1);

    // 设置随机数种子
    srand(time(NULL));

    return 0;
}


BEGIN_MESSAGE_MAP(WinRegister, CDialogEx)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON1, &WinRegister::OnBnClickedButton1)
    ON_EN_CHANGE(IDC_EDIT1, &WinRegister::OnEnChangeEdit1)
END_MESSAGE_MAP()


// WinRegister 消息处理程序


void WinRegister::OnPaint()
{
    //CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
    drawPicOnPait(&m_imgBG, this, 0, 0); // 在当前窗口(注册窗口)的(0,0)位置绘制IDB_PNG3
}


// 整个注册窗口的中断函数
void WinRegister::OnTimer(UINT_PTR nIDEvent)
{
    static int snowIndex = 0;   // 雪花图片的编号索引
    static int shoot_count = 0; // 3秒倒计时拍摄的时间计数
    // 定时器1用于循环播放16张雪花背景图片
    if (nIDEvent == 1) {
        m_imgSnow_single.SetBitmap(m_imgsnows[snowIndex]); // 更新图片控件的显示
        snowIndex = (snowIndex + 1) % 16; // 更新雪花图片索引。对16取余是因为16张雪花照片重复。
    }
    // 定时器2用于摄像头显示，并在3s时拍摄照片
    else if (nIDEvent == 2) {
        // 获取摄像头拍摄的单帧，并进行显示
        cv::Mat cam_frame;       // 定义摄像头单帧
        cv::Rect face_rect;      // 定义人脸区域
        cap_WinReg >> cam_frame; // 获取摄像头拍摄的单帧
        
        if (shoot_count < 3000 / 30) { // 3000表示3000ms(3s)，30是定时器2的中断间隔时间
            if (shoot_count == 0) {
                mciSendString(L"play res/zhuce.mp3", 0, 0, 0); // 播放3秒倒计时音效
            }
            shoot_count++;
            // 将图片中的人脸框出来
            if (faceRegion(faceModel, cam_frame, face_rect)) {
                cv::rectangle(cam_frame, face_rect, cv::Scalar(0, 255, 255), 2);
            }
            imshow("m_imgCamera_single", cam_frame);
        }
        else if (shoot_count == 3000 / 30) {
            //cv::imwrite("tmp.jpg", cam_frame);     // 保存单帧照片
            shoot_count = 0;                         // 清零计数
            KillTimer(2);                            // 关闭定时器
            cap_WinReg.release();                    // 关闭摄像头
            cv::destroyWindow("m_imgCamera_single"); // 关闭摄像头显示窗口
            // 若拍摄的照片没有人脸就给出提示并返回，若有则保存信息并跳转到“欢迎窗口”
            if (!faceRegion(faceModel, cam_frame, face_rect)) {
                ::MessageBox(NULL, _T("未检测到人脸！"), _T("错误"), MB_OK | MB_ICONHAND);
                SetTimer(1, 30, NULL); // 打开雪花视频定时器
                return;
            }
            else {
                // 计算所拍摄人脸与img2的人脸相似度
                cv::Mat img1{ cam_frame };                        // 直接初始化可以减少IO操作，节省时间
                cv::Mat img2 = cv::imread("res/leijun.jpg");      // 读取参照人脸的头像
                float xishu = faceCompare(faceModel, img1, img2); // 获取与参照人脸的相似度
                //int yanzhi = xishu * 1000 + 80 + rand() % 15;     // Rock的人脸相似度修正公式
                int yanzhi = (int)(xishu * 100) + rand() % 25;           // 人脸相似度修正公式(防止相似度过低)
                yanzhi = (yanzhi < 0) ? 0 : yanzhi;               // 为负数时表示人脸比对失败
                yanzhi = (yanzhi >= 100) ? 99 : yanzhi;           // 若超过100还是限定一下范围
                // 生成职业
                const char* jobs[] = { "高级驾驶员", "中级驾驶员", "初级驾驶员", "汽车修理工" };
                // 获取编辑框内容
                CString edit_text;
                m_editName.GetWindowTextW(edit_text);
                USES_CONVERSION;                  // 声明接下来要进行字符集转换，常在 MFC 或 ATL 项目中使用
                char* user_name = T2A(edit_text); // 将CString转换成char*
                // 保存文件：工作-用户名-颜值.jpg
                char user_filename[100];
                sprintf_s(user_filename, sizeof(user_filename), "users\\%s-%s-%d.jpg", jobs[rand() % 4], user_name, yanzhi);
                cv::imwrite((cv::String)user_filename, cam_frame);
                //sprintf_s(cmd_savejpg, sizeof(cmd_savejpg), "move tmp.jpg users\\%s-%s-%d.jpg", jobs[rand() % 5], user_name, yanzhi);
                //system(cmd_savejpg);

                CDialogEx::OnOK();                  // 关闭注册窗口

                // 以模态方式打开“欢迎窗口”
                WinWelcome win_welcome;
                win_welcome.DoModal();
            }
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}


// “注册窗口”中的“刷脸按钮”点击操作
void WinRegister::OnBnClickedButton1()
{
    // 检查用户名编辑框是否有内容
    if (m_editName.GetWindowTextLengthW() == 0) {
        ::MessageBox(NULL, _T("请输入用户名！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    // 关闭雪花视频的定时器（MFC框架中自带函数）
    KillTimer(1);
    // 将雪花图片更换成墙纸
    HBITMAP bg_wall = (HBITMAP)LoadImage(NULL, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_imgSnow_single.SetBitmap(bg_wall);

    // 将opencv的窗体嵌入到图片控件m_imgCamera_single中
    cv::namedWindow("m_imgCamera_single", cv::WINDOW_AUTOSIZE);     // 打开一个opencv窗口，注意名称要与图片控件一致
    // 第二个选项是cv::WindowFlags：https://vovkos.github.io/doxyrest-showcase/opencv/sphinxdoc/enum_cv_WindowFlags.html
    HWND hWnd = (HWND)cvGetWindowHandle("m_imgCamera_single");      // 获取opencv窗口句柄
    HWND hParent = ::GetParent(hWnd);                               // 获取opencv窗口的父窗口句柄
    ::SetParent(hWnd, GetDlgItem(IDC_STATIC_CAMERA_REG)->m_hWnd);   // 将opencv窗口的句柄设置为图片控件的句柄
    ::ShowWindow(hParent, SW_HIDE);                                 // 隐藏原父窗口

    // 打开默认摄像头0
    cap_WinReg.open(0);
    if (!cap_WinReg.isOpened()) {
        ::MessageBox(NULL, _T("摄像头打开失败！\n请检查摄像头是否正确连接并开启！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
        //c++中MessageBox弹窗的用法大全：https://blog.csdn.net/LCR2025/article/details/129223538
        return;
    }

    // 开启摄像头的定时器
    SetTimer(2, 30, NULL);
}


void WinRegister::OnEnChangeEdit1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}
