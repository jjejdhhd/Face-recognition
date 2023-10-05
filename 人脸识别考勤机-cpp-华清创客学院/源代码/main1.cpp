//////////////////////////////终端编译指令/////////////////////////////
g++ main.cpp -o main -std=c++11 -lopencv_videoio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_objdetect -lopencv_imgcodecs -lcurl -lcrypto -ljsoncpp

//////////////////////////////cpp源代码/////////////////////////////
#include <iostream>
#include "opencv2/opencv.hpp"
#include "face.h"
using namespace std;
using namespace cv;
using namespace aip;
int main() {
    // 打开摄像头
    //VideoCapture cap(0);
    VideoCapture cap(0, cv::CAP_V4L2);  // 打开(默认的)摄像头0（编译时加上-lopencv_videoio）
    if(!cap.isOpened()) {               // 检查摄像头是否成功打开
        cout << "Camera open failed!" << endl;
        return -1;
    }
    cout << "Camera open success." << endl;

    // 新建与百度云通信的客户端
    // 设置APPID/AK/SK，在刚刚创建的百度云人脸识别应用中复制下来即可
    std::string app_id = "39837794";
    std::string api_key = "feHHZyllLWoo0pidqVraCaol";
    std::string secret_key = "zLAaFUqs2IZ8GHtqVsGFlvby4pWI8wP6";
    aip::Face client(app_id, api_key, secret_key);

    /////////////////////显示视频-开始/////////////////////
    Mat img_color; // 定义彩色图像（注意因为用到了mat，所以编译时加上-lopencv_core）
    Mat img_gray;  // 定义灰度图
    Mat img_eql;   // 定义直方图均衡化后的灰度图
    // 加载人脸的模型文件（-lopencv_objdetect）
    CascadeClassifier Classifier("/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
    vector<Rect> face_all;      // 存储所有脸的方框列表
    Mat img_face_shot;          // 存储人脸截图
    vector<uchar> img_face_jpg; // 存储.jpg格式的人脸截图
    string img_face_base64;     // 存储BASE64格式的人脸截图
    Json::Value result;         // 存储百度云返回的信息
    time_t sec;                 // 存储当前系统时间(总秒数)
    for(;;) {
        cap >> img_color;                                   // 1.获取一帧彩色图像
        cvtColor(img_color, img_gray, cv::COLOR_BGR2GRAY);  // 2.将彩图转换成灰度图（-lopencv_imgproc）
        equalizeHist(img_gray, img_eql);                    // 3.直方图均衡化（-lopencv_imgproc）
        Classifier.detectMultiScale(img_eql, face_all);     // 4.人脸检测
        if( face_all.size() ) {
            // 5.框出所有的人脸
            for(int i=0; i<face_all.size(); i++) {                  // 该for循环兼顾判断是否存在方框
                rectangle(img_eql, face_all[i], Scalar(0,255,255)); // 将所有脸的方框都显示出来
                // 注意添加颜色时，Scalar的顺序是BGR
            }
            // 6.将人脸截图保存成jpg格式
            img_face_shot = img_eql(face_all[0]);
            imencode(".jpg", img_face_shot, img_face_jpg);// 编译时加上-lopencv_imgcodecs

            // 7.将jpg格式转换成百度云支持的BASE64格式
            img_face_base64 = base64_encode((char *)img_face_jpg.data(), img_face_jpg.size());
            // 8.调用百度云1:N人脸搜索
            result = client.face_search_v3(img_face_base64, "BASE64", "my_test", aip::json_null);
            
            // 9.提取识别到的人名，并将信息添加到屏幕上
            if( !result["result"].isNull() && result["result"]["user_list"][0]["score"].asInt() > 80 ) {
                // 输出姓名和时间
                cout << result["result"]["user_list"][0]["user_id"] << " ";
                sec = time(NULL);
                cout << ctime(&sec) << endl; // 将总秒数转换成年月日时分秒输出

                // 在图像上添加名字、时间
                putText(img_eql,
                        result["result"]["user_list"][0]["user_id"].asString(),//要添加的内容
                        Point(0,50),          // 添加的位置
                        FONT_HERSHEY_SIMPLEX, // 字体
                        1.2,                  // 字号
                        Scalar(255,255,255)); // 颜色
                putText(img_eql,
                        ctime(&sec),//要添加的内容
                        Point(0,100),         // 添加的位置
                        FONT_HERSHEY_SIMPLEX, // 字体
                        1.2,                  // 字号
                        Scalar(255,255,255)); // 颜色
            }
        }

        imshow("video", img_eql); // 在“video”窗口中显示图片（编译时加上-lopencv_highgui）
        waitKey(50);              // 等待50ms(20fps)
    }
    /////////////////////显示视频-结束/////////////////////
    
    return 0;
}