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

// 32λ
//#define SDKKey "8LJqeAmH6wsjcdBBMt6E1WRjt8aHyaWdsfUg7ELx8KPD"

//64λ
#define SDKKey  "7kidGLKLxqz39fUgPFkzvQvZADdtYtMEMZX64iACPYZM"

// ��������ʶ��SDK��ʼ��
void faceInit(MHandle* handle);

// �����ԱȺ������������ƶ�
float faceCompare(MHandle handle, cv::Mat& img1, cv::Mat& img2);

// ����ͼƬ��ʶ�𵽵�����λ��
BOOL faceRegion(MHandle handle, cv::Mat img, cv::Rect& face_rect);