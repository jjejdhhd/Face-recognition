// WinHome.cpp: 实现文件
// 
#include "pch.h"
#include "face_recognition.h"
#include "afxdialogex.h"
#include "WinHome.h"
#include <opencv2/opencv.hpp>

// WinHome 对话框
IMPLEMENT_DYNAMIC(WinHome, CDialogEx)

WinHome::WinHome(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_HOME, pParent)
{}

WinHome::~WinHome() {}

void WinHome::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_HOME_HEAD, m_img_headpic);
    DDX_Control(pDX, IDC_HOME_NAME, m_text_name);
    DDX_Control(pDX, IDC_HOME_JOB, m_text_job);
    DDX_Control(pDX, IDC_HOME_YANZHI, m_text_yanzhi);
    DDX_Control(pDX, IDC_HOME_Q_COIN, m_text_q_coin);
    DDX_Control(pDX, IDC_HOME_ID, m_text_id);
    DDX_Control(pDX, IDC_HOME_PLAY_PAUSE, m_btn_play_pause);
    DDX_Control(pDX, IDC_HOME_VIDEO, m_img_video);
}


BEGIN_MESSAGE_MAP(WinHome, CDialogEx)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_HOME_PLAY_PAUSE, &WinHome::OnBnClickedHomePlayPause)
END_MESSAGE_MAP()


// WinHome 消息处理程序


BOOL WinHome::OnInitDialog() {
    // 父类的(同名)初始化方法
    CDialogEx::OnInitDialog();

    // 设置窗口大小
    SetWindowPos(NULL, 0, 0, 1080 + 25, 609 + 70, SWP_NOMOVE);
    // 设置窗口标题
    SetWindowText(L"HOME窗口");

    // 左上角显示头像
    int face_width{ 200 }, face_height{ 150 };  // 设置头像大小
    char filepath[256];                         // 定义注册照片(背景)路径
    sprintf_s(filepath, sizeof(filepath), "users/%s-%s-%d.jpg", user_job, user_name, user_yanzhi);
    cv::Mat img_bg, img_logo;
    img_bg = cv::imread(filepath);                                          // 加载注册照
    img_logo = cv::imread("res/logo.png");                                  // 加载水印
    cv::Mat imgROI = img_bg(cv::Rect(0, 0, img_logo.cols, img_logo.rows));  // 要合成的区域
    addWeighted(imgROI, 0.5, img_logo, 0.5, 0, imgROI);                     // 合成图片
    cv::resize(img_bg, img_bg, cv::Size{ face_width,face_height });         // 缩放图片
    cv::imwrite("res\\tmp_home_face.jpg", img_bg);                          // 保存合成后的图片
    ::MoveWindow(m_img_headpic.m_hWnd, 20, 20, face_width, face_height, 1); // 调整头像显示窗口的位置(20,20)和大小
    cimg_head.Load(L"res\\tmp_home_face.jpg");                             // 读取头像
    m_img_headpic.SetBitmap((HBITMAP)cimg_head);                           // 显示头像


    // 定义5个标签的显示
    CFont font_home;
    font_home.CreatePointFont(500, L"宋体", NULL);     // 5个标签的字体
    int pos_x{ 40 }, pos_y{ 200 }, pos_interval{ 40 }; // 5个标签的位置参数
    int text_width{ 150 }, text_height{ 30 };          // 5个标签的大小
    CString text_tmp;                                  // 临时存储标签的显示内容

    text_tmp = (CString)"姓名：" + (CString)user_name;                          // 生成内容
    m_text_name.SetWindowText(text_tmp);                                        // 显示内容
    m_text_name.SetFont(&font_home);                                            // 使用设置的字体
    ::MoveWindow(m_text_name.m_hWnd, pos_x, pos_y, text_width, text_height, 0); // 调整位置和大小

    text_tmp = (CString)"工作：" + (CString)user_job;                                       // 生成内容
    m_text_job.SetWindowText(text_tmp);                                                     // 显示内容
    m_text_job.SetFont(&font_home);                                                         // 使用设置的字体
    ::MoveWindow(m_text_job.m_hWnd, pos_x, pos_y+pos_interval, text_width, text_height, 0); // 调整位置和大小

    text_tmp.Format(L"颜值：%d", user_yanzhi);                                                     // 生成内容
    m_text_yanzhi.SetWindowText(text_tmp);                                                         // 显示内容
    m_text_yanzhi.SetFont(&font_home);                                                             // 使用设置的字体
    ::MoveWindow(m_text_yanzhi.m_hWnd, pos_x, pos_y + pos_interval*2, text_width, text_height, 0); // 调整位置和大小
    
    m_text_q_coin.SetWindowText(L"Q币：99,999,999");                                               // 显示内容
    m_text_q_coin.SetFont(&font_home);                                                             // 使用设置的字体
    ::MoveWindow(m_text_q_coin.m_hWnd, pos_x, pos_y + pos_interval*3, text_width, text_height, 0); // 调整位置和大小

    m_text_id.SetWindowText(L"ID：NB0001");                                                    // 显示内容
    m_text_id.SetFont(&font_home);                                                             // 使用设置的字体
    ::MoveWindow(m_text_id.m_hWnd, pos_x, pos_y + pos_interval*4, text_width, text_height, 0); // 调整位置和大小


    // 左下角显示按钮
    m_btn_play_pause.Init(IDB_PNG12, 4, BTN_TYPE_NORMAL);        // 按钮的初始化
    ::MoveWindow(m_btn_play_pause.m_hWnd, 80, 430, 120, 120, 0); // 调整按钮的位置(80,430)和大小(120x120)


    // 右侧视频播放界面的背景图片的初始化
    ::MoveWindow(m_img_video.m_hWnd, 1080 - 802, 0, 802, 609, 1); // 调整窗口大小
    cimg_video_bg.Load(L"res/videoBG.png");                       // 读取背景
    m_img_video.SetBitmap((HBITMAP)cimg_video_bg);                // 显示背景

    // 初始化视频播放器
    videoPlayerInit(&m_player);

    // 按钮背景
    btn_bg.Load(L"res/boardBg.bmp");

    return 0;
}

// 绘制函数
void WinHome::OnPaint() {
    drawPicOnPait(&btn_bg, this, 450, 0); // 绘制按钮背景
}

// 按钮点击函数
void WinHome::OnBnClickedHomePlayPause() {
    // 开始播放
    if (status_player == 0) {
        m_player.hwnd = GetDlgItem(IDC_HOME_VIDEO)->GetSafeHwnd();
        //videoPlayerPlay(&m_player, "res\\流浪地球2-太空电梯超燃混剪.mp4");
        videoPlayerPlay(&m_player, "C:\\Users\\14751\\Desktop\\face_recognition\\res\\LLDQ.mp4");//注意这里必须是完整路径，且不能有中文
        m_btn_play_pause.Init(IDB_PNG11, 4, BTN_TYPE_NORMAL); // 按钮换皮肤
        status_player = 1;
    }
    // 暂停
    else if (status_player == 1) {
        m_btn_play_pause.Init(IDB_PNG12, 4, BTN_TYPE_NORMAL); // 按钮换皮肤
        videoPlayerPause(&m_player); // 暂停视频
        status_player = 2;
    }
    // 继续播放
    else if (status_player == 2) {
        m_btn_play_pause.Init(IDB_PNG11, 4, BTN_TYPE_NORMAL); // 按钮换皮肤
        videoPlayerPause(&m_player); // 继续播放视频
        status_player = 1;
    }
}