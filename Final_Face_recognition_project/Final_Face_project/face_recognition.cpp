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
    // ����ͼ��
    src_img = cv::imread(filename); // �Ӹ������ļ�����ȡͼ��src_img
    cout << "ͼƬ��Ϣ��" << src_img.rows << ", " << src_img.cols << ", " << src_img.elemSize() << endl;
    if (src_img.empty()) {                            // ���ͼ���Ƿ�ɹ�����
        throw std::runtime_error("open file error!");    // ���ͼ��û�гɹ����أ��׳�����ʱ����
    }

    // �������
    face_detector->setInputSize(src_img.size()); // �������������������ͼ���СΪsrc_img�Ĵ�С

    face_detector->detect(src_img, face_pos); // ��src_img�м��������������⵽������λ�ñ�����face_pos

    if (face_pos.empty()) {                                             // ����Ƿ��⵽����
        throw std::runtime_error("There are no faces in the picture!");    // ���û�м�⵽�������׳�����ʱ����
    }
}

// ����ֵ��ȡ
cv::Mat FaceRecognition::face_feature()
{
    cv::Mat align_img;
    face_recognizer->alignCrop(src_img, face_pos, align_img); // ��src_img�е��������ж���Ͳü����������������align_img

    cv::Mat feature;
    face_recognizer->feature(align_img, feature); // ��ȡalign_img�е������������������������feature

    return feature.clone(); // ����feature��һ������
}

// �������
void FaceRecognition::face_mark()
{
    // �������
    int x = face_pos.at<float>(0, 0); // ��face_pos�л�ȡ������x����
    int y = face_pos.at<float>(0, 1); // ��face_pos�л�ȡ������y����
    int w = face_pos.at<float>(0, 2); // ��face_pos�л�ȡ�����Ŀ��
    int h = face_pos.at<float>(0, 3); // ��face_pos�л�ȡ�����ĸ߶�

    cout << "ͼƬ������Ϣ��x = " << x << ", y = " << y << ", w = " << w << ", h = " << h << endl;
    cv::rectangle(src_img, cv::Rect(x, y, w, h), cv::Scalar(0, 255, 255)); // ��src_img�ϻ���һ�����Σ��Ա��������λ��

    // �������壬��ʾָ����ͼ��
    cv::imshow("image", src_img); // ����һ����Ϊ"image"�Ĵ��ڣ����ڴ�������ʾsrc_img

}

// ����ֵƥ��
void FaceRecognition::face_match(const cv::Mat &feature1, const cv::Mat &feature2)
{
    // ������������ֵ֮��ľ���
    float dist = face_recognizer->match(feature1, feature2);    // ʹ������ʶ��ģ�ͼ���feature1��feature2֮��ľ���
    cout << "dist = " << dist << endl;                          // �������õ��ľ���
}

// ���غ������������
bool FaceRecognition::operator()(const std::vector<float> &feature1, const std::vector<float> &feature2) const
{
    // ������������ֵ֮��ľ���
    cv::Mat f1(1, feature1.size(), CV_32F, (void *)feature1.data());
    cv::Mat f2(1, feature2.size(), CV_32F, (void *)feature2.data());

    double dist = face_recognizer->match(f1, f2);
    cout << "���ƶȣ� " << dist << endl;
    if (dist > 0.363) { // �������õ��ľ���С��0.363������true�����򷵻�false
        return true;
    }
    return false;

}
