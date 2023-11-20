#include "face_recognition.h"
using namespace std;

FaceRecognition::FaceRecognition()
{
    fd_model_path = "c:/opencv455-win32/face_detection_yunet_2022mar.onnx";
    fr_model_path = "C:/opencv455-win32/face_recognizer_fast.onnx";

    face_detector = cv::FaceDetectorYN::create(fd_model_path, "", cv::Size());
    face_recognizer = cv::FaceRecognizerSF::create(fr_model_path, "");
}

void FaceRecognition::face_detect(std::string filename)
{
    // 加载图像
    src_img = cv::imread(filename); // 从给定的文件名读取图像到src_img
    cout << "图片信息：" << src_img.rows << ", " << src_img.cols << ", " << src_img.elemSize() << endl;
    if (src_img.empty()) {                            // 检查图像是否成功加载
        throw std::runtime_error("open file error!");    // 如果图像没有成功加载，抛出运行时错误
    }

    // 人脸检测
    face_detector->setInputSize(src_img.size()); // 设置人脸检测器的输入图像大小为src_img的大小

    face_detector->detect(src_img, face_pos); // 在src_img中检测人脸，并将检测到的人脸位置保存在face_pos

    if (face_pos.empty()) {                                             // 检查是否检测到人脸
        throw std::runtime_error("There are no faces in the picture!");    // 如果没有检测到人脸，抛出运行时错误
    }
}

// 特征值提取
cv::Mat FaceRecognition::face_feature()
{
    cv::Mat align_img;
    face_recognizer->alignCrop(src_img, face_pos, align_img); // 对src_img中的人脸进行对齐和裁剪，并将结果保存在align_img

    cv::Mat feature;
    face_recognizer->feature(align_img, feature); // 提取align_img中的人脸特征，并将结果保存在feature

    return feature.clone(); // 返回feature的一个副本
}

// 人脸标记
void FaceRecognition::face_mark()
{
    // 标记人脸
    int x = face_pos.at<float>(0, 0); // 从face_pos中获取人脸的x坐标
    int y = face_pos.at<float>(0, 1); // 从face_pos中获取人脸的y坐标
    int w = face_pos.at<float>(0, 2); // 从face_pos中获取人脸的宽度
    int h = face_pos.at<float>(0, 3); // 从face_pos中获取人脸的高度

    cout << "图片坐标信息：x = " << x << ", y = " << y << ", w = " << w << ", h = " << h << endl;
    cv::rectangle(src_img, cv::Rect(x, y, w, h), cv::Scalar(0, 255, 255)); // 在src_img上绘制一个矩形，以标记人脸的位置

    // 创建窗体，显示指定的图像
    cv::imshow("image", src_img); // 创建一个名为"image"的窗口，并在窗口中显示src_img

}

// 特征值匹配
void FaceRecognition::face_match(const cv::Mat &feature1, const cv::Mat &feature2)
{
    // 计算两个特征值之间的距离
    float dist = face_recognizer->match(feature1, feature2);    // 使用人脸识别模型计算feature1和feature2之间的距离
    cout << "dist = " << dist << endl;                          // 输出计算得到的距离
}

// 重载函数调用运算符
bool FaceRecognition::operator()(const std::vector<float> &feature1, const std::vector<float> &feature2) const
{
    // 计算两个特征值之间的距离
    cv::Mat f1(1, feature1.size(), CV_32F, (void *)feature1.data());
    cv::Mat f2(1, feature2.size(), CV_32F, (void *)feature2.data());

    double dist = face_recognizer->match(f1, f2);
    cout << "相似度： " << dist << endl;
    if (dist > 0.363) { // 如果计算得到的距离小于0.363，返回true，否则返回false
        return true;
    }
    return false;

}
