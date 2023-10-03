#include "pch.h"
#include "faceTool.h"

void faceInit(MHandle* handle) {
    //激活接口,需联网激活
    MRESULT res = ASFOnlineActivation((char*)APPID, (char*)SDKKey);
    if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
        printf("激活失败\n");

    //获取激活文件信息
    ASF_ActiveFileInfo  activeFileInfo;
    res = ASFGetActiveFileInfo(&activeFileInfo);
    if (res != MOK)
        printf("ASFGetActiveFileInfo fail: %d\n", res);

    //初始化接口
    MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
    res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 30, 10, mask, handle);
    if (res != MOK)
        printf("接口初始化失败\n");
}

float faceCompare(MHandle handle, cv::Mat& img1, cv::Mat& img2) {
    ASF_MultiFaceInfo detectedFaces1{ 0 };          // 定义多人脸信息
    ASF_SingleFaceInfo SingleDetectedFaces1{ 0 };   // 定义单人脸信息
    ASF_FaceFeature feature1{ 0 };                  // 定义人脸特征
    ASF_FaceFeature copyfeature1{ 0 };              // 定义人脸特征的拷贝
    cv::Rect roiRect1(0, 0, img1.cols - img1.cols % 4, img1.rows); // 定义要裁剪的区域（只是因为要求宽度是4的整数倍）
    cv::Mat cutImg1 = img1(roiRect1).clone();       // 得到裁剪好的图片

    // 检测是否存在人脸（注意这里虹软SDK文档要求图片宽度必须是4的倍数）
    MRESULT res;
    res = ASFDetectFaces(handle, cutImg1.cols, cutImg1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1.data, &detectedFaces1);

    if (MOK == res) {
        // 取出图片中的第一个人脸信息
        if (detectedFaces1.faceRect != NULL && detectedFaces1.faceOrient != NULL) {
            // 其实这个判断可以不写，因为只要res==MOK，detectedFaces1中就一定会有内容，所以直接赋值没问题。
            // 但是我想消除编译器警告，所以才加上这个判断。
            SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
            SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
            SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
            SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
            SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];
        }

        // 单人脸特征提取
        res = ASFFaceFeatureExtract(handle, cutImg1.cols, cutImg1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1.data, &SingleDetectedFaces1, &feature1);
        if (res == MOK) {
            // 若提取到了人脸特征信息，才将其进行拷贝。
            // 至于为什么要进行拷贝，暂时还没有搞懂？？
            copyfeature1.featureSize = feature1.featureSize;
            copyfeature1.feature = (MByte*)malloc(feature1.featureSize);
            memset(copyfeature1.feature, 0, feature1.featureSize);
            memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);
        }
        else {
            printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
        }
    }
    else {
        printf("ASFDetectFaces 1 fail: %d\n", res);
    }

    //第二张人脸处理
    ASF_MultiFaceInfo detectedFaces2{ 0 };          // 定义多人脸信息
    ASF_SingleFaceInfo SingleDetectedFaces2{ 0 };   // 定义单人脸信息
    ASF_FaceFeature feature2 = { 0 };               // 定义人脸特征
    cv::Rect roiRect2(0, 0, img2.cols - img2.cols % 4, img2.rows); // 定义要裁剪的区域（只是因为要求宽度是4的整数倍）
    cv::Mat cutImg2 = img2(roiRect2).clone();       // 得到裁剪好的图片

    // 检测图片中的人脸信息
    res = ASFDetectFaces(handle, cutImg2.cols, cutImg2.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2.data, &detectedFaces2);
    if (MOK == res) {
        if (detectedFaces2.faceRect != NULL && detectedFaces2.faceOrient != NULL) {
            // 其实这个判断可以不写，因为只要res==MOK，detectedFaces1中就一定会有内容，所以直接赋值没问题。
            // 但是我想消除编译器警告，所以才加上这个判断。
            SingleDetectedFaces2.faceRect.left = detectedFaces2.faceRect[0].left;
            SingleDetectedFaces2.faceRect.top = detectedFaces2.faceRect[0].top;
            SingleDetectedFaces2.faceRect.right = detectedFaces2.faceRect[0].right;
            SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
            SingleDetectedFaces2.faceOrient = detectedFaces2.faceOrient[0];
        }

        // 单人脸特征提取
        res = ASFFaceFeatureExtract(handle, cutImg2.cols, cutImg2.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2.data, &SingleDetectedFaces2, &feature2);
        if (MOK != res) {
            printf("ASFFaceFeatureExtract 2 fail: %d\n", res);
        }
    }
    else {
        printf("ASFDetectFaces 2 fail: %d\n", res);
    }

    // 单人脸特征比对
    MFloat confidenceLevel;
    res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
    if (res != MOK) {
        confidenceLevel = -1;
    }

    SafeFree(copyfeature1.feature); //释放内存

    return confidenceLevel;
}

// 返回图片中识别到的人脸位置
BOOL faceRegion(MHandle handle, cv::Mat img, cv::Rect& face_rect) {
    // 裁剪图片，使其宽度为4的整数倍（ASFDetectFaces要求）
    cv::Rect roiRect1(0, 0, img.cols - img.cols % 4, img.rows); // 定义要裁剪的区域（只是因为要求宽度是4的整数倍）
    cv::Mat cutImg1 = img(roiRect1).clone();       // 得到裁剪好的图片

    // 检测是否存在人脸
    ASF_MultiFaceInfo detectedFaces1{ 0 };          // 定义多人脸信息
    MRESULT res = ASFDetectFaces(handle, cutImg1.cols, cutImg1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1.data, &detectedFaces1);
    // 若存在人脸就返回第一个人脸信息
    if (MOK == res && detectedFaces1.faceRect != NULL && detectedFaces1.faceNum) {
        face_rect.x = detectedFaces1.faceRect[0].left;
        face_rect.y = detectedFaces1.faceRect[0].top;
        face_rect.width = detectedFaces1.faceRect[0].right - detectedFaces1.faceRect[0].left;
        face_rect.height = detectedFaces1.faceRect[0].bottom - detectedFaces1.faceRect[0].top;
        return true;
    }
    else {
        printf("ASFDetectFaces 1 fail: %d\n", res);
        return false;
    }
}
