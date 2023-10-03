#pragma once

#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <direct.h>
#include <iostream>  
#include <stdarg.h>
#include <string>
#include <opencv2\opencv.hpp>

using namespace std;
#pragma comment(lib, "libarcsoft_face_engine.lib")

#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; } 
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; } 

#define APPID "qAQ7JXMqChSZ5td1RJq1i16Lkew4WgZxXv92vnAWXqs"

// 32位
//#define SDKKey "8LJqeAmH6wsjcdBBMt6E1WRjt8aHyaWdsfUg7ELx8KPD"

//64位
#define SDKKey  "7kidGLKLxqz39fUgPFkzvQvZADdtYtMEMZX64iACPYZM"

// 虹软人脸识别SDK初始化
void faceInit(MHandle* handle);

// 人脸对比函数，返回相似度
float faceCompare(MHandle handle, cv::Mat& img1, cv::Mat& img2);

// 返回图片中识别到的人脸位置
BOOL faceRegion(MHandle handle, cv::Mat img, cv::Rect& face_rect);