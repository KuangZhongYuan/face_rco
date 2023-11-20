#ifndef FACE_RECOGNITION_H // ���û�ж���FACE_RECOGNITION_H��
#define FACE_RECOGNITION_H // ����FACE_RECOGNITION_H��

#include <iostream>           // ����iostream�⣬��������/�������
#include <opencv2/opencv.hpp> // ����OpenCV�⣬����ͼ����ͼ�����Ӿ�����

class FaceRecognition { // ����һ����ΪFaceRecognition����
        
    public:   
        FaceRecognition(); // ��Ĺ��캯��

        // �������
        void face_detect(std::string filename); // һ����Ա���������ڼ��ͼ���е�����

        // ����ֵ��ȡ
        cv::Mat face_feature(); // һ����Ա������������ȡ����������ֵ

        // �������
        void face_mark(); // һ����Ա������������ͼ���ϱ������λ��

        // �Ա�����������ֵƥ��
        void face_match(const cv::Mat &feature1, const cv::Mat &feature2); // һ����Ա���������ڱȽ�������������ֵ

        // ���غ������������
        bool operator()(const std::vector<float> &feature1, const std::vector<float> &feature2) const; // һ����Ա�����������˺�����������������ڱȽ�������������ֵ

    private:                       // ����ĳ�Ա��˽�еģ�ֻ������ĳ�Ա�����з���
        std::string fd_model_path; // �������ģ�͵�·��
        std::string fr_model_path; // ����ʶ��ģ�͵�·��

        cv::Mat src_img;  // �洢ͼ��ľ���
        cv::Mat face_pos; // �洢����λ����Ϣ�ľ���

        cv::Ptr<cv::FaceDetectorYN> face_detector;     // �����������ָ��
        cv::Ptr<cv::FaceRecognizerSF> face_recognizer; // ����ʶ������ָ��
};

#endif // ������FACE_RECOGNITION_H����ж�
