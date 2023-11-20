#ifndef FACE_RECOGNITION_H // 如果没有定义FACE_RECOGNITION_H宏
#define FACE_RECOGNITION_H // 定义FACE_RECOGNITION_H宏

#include <iostream>           // 包含iostream库，用于输入/输出操作
#include <opencv2/opencv.hpp> // 包含OpenCV库，用于图像处理和计算机视觉操作

class FaceRecognition { // 定义一个名为FaceRecognition的类
        
    public:   
        FaceRecognition(); // 类的构造函数

        // 人脸检测
        void face_detect(std::string filename); // 一个成员函数，用于检测图像中的人脸

        // 特征值提取
        cv::Mat face_feature(); // 一个成员函数，用于提取人脸的特征值

        // 人脸标记
        void face_mark(); // 一个成员函数，用于在图像上标记人脸位置

        // 对比人脸，特征值匹配
        void face_match(const cv::Mat &feature1, const cv::Mat &feature2); // 一个成员函数，用于比较两个人脸特征值

        // 重载函数调用运算符
        bool operator()(const std::vector<float> &feature1, const std::vector<float> &feature2) const; // 一个成员函数，重载了函数调用运算符，用于比较两个人脸特征值

    private:                       // 下面的成员是私有的，只能在类的成员函数中访问
        std::string fd_model_path; // 人脸检测模型的路径
        std::string fr_model_path; // 人脸识别模型的路径

        cv::Mat src_img;  // 存储图像的矩阵
        cv::Mat face_pos; // 存储人脸位置信息的矩阵

        cv::Ptr<cv::FaceDetectorYN> face_detector;     // 人脸检测器的指针
        cv::Ptr<cv::FaceRecognizerSF> face_recognizer; // 人脸识别器的指针
};

#endif // 结束对FACE_RECOGNITION_H宏的判断
