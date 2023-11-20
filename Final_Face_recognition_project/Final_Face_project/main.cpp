#include <iostream>
#include <cstdlib>
#include "face_recognition.h"
#include "mysqlite.h"

using namespace std;

//菜单界面
void menu()
{
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "-------------欢迎使用kk牌智能人脸识别系统------------------" << endl;
    cout << "---------------------------->功能菜单<-------------------------" << endl;
    cout << "1：人脸录入      2：人脸查询        3：删除信息       4：显示所有      0：退出系统" << endl;
    cout << endl;
}
int main()
{
    MySqlite mysql;
    FaceRecognition fr;
    int choice;
    //bool exit = false;
    while(1) {
        menu();
        cout << "请输入选项: ";
        cin >> choice;
        switch(choice) {
            case 1: {
                cout << "你选择的功能是--->1：人脸录入" << endl;
                // 检测识别人脸
                string loc_img_path;
                cout << "请输入要录入的图片信息或路径:" << endl;
                cin >> loc_img_path;
                fr.face_detect(loc_img_path.c_str());
                // 提取人脸特征值
                auto feature1 = fr.face_feature();
                cout << feature1.total() * feature1.elemSize() << endl;
                // 存储数据
                string lord_name;
                int lord_id;
                cout << "请输入要录入的以下信息：" << endl << "id:   name:" << endl;
                cin >> lord_id;
                cin >> lord_name;
                cout << "要录入的信息为：id= " << lord_id << "，name= " << lord_name << endl;
                if(true == mysql.save_face_feature(lord_id, lord_name, feature1.data, feature1.total() * feature1.elemSize())) {
                    cout << "录入成功" << endl;
                }
                //创建窗口显示人脸
                fr.face_mark();
                cv::waitKey();
                cv::destroyAllWindows();
                break;
            }
            case 2: {
                cout << "你选择的功能是--->2：人脸查询" << endl;
                // 检测识别人脸
                string find_img_path;
                cout << "请输入要查询的图片信息或路径:" << endl;
                cin >> find_img_path;
                fr.face_detect(find_img_path.c_str());
                // 提取人脸特征值
                auto feature2 = fr.face_feature();
                //cout << feature2.total() * feature2.elemSize() << endl;
                // // 根据特征值查询人名
                std::string name = mysql.query_name_by_feature(feature2.reshape(1, feature2.total()), fr);
                cout << name << endl;
                //创建窗口显示人脸
                fr.face_mark();
                cv::waitKey();
                cv::destroyAllWindows();
                break;
            }
            case 3: {
                cout << "你选择的功能是--->3：删除信息" << endl;
                int del_id;
                string del_name;
                cout << "请输入要删除的信息:" << endl << "id:   name:" << endl;
                cin >> del_id;
                cin >> del_name;
                cout << "要删除的信息为：id= " << del_id << "，name= " << del_name << endl;
                if(true == mysql.delete_face_feature(del_id, del_name)) {
                    cout << "删除成功" << endl;
                }
                break;
            }
            case 4: {
                cout << "你选择的功能是--->4：显示所有" << endl;
                cout << endl;
                cout << "当前库中已存在的信息有：" << endl;
                mysql.show_face_feature();
                break;
            }
            case 0: {
                cout << "你选择的功能是--->0：退出系统" << endl;
                //exit = true; // 退出循环
                exit(0);
                break;
            }
            default:
                cout << "无效的选项，请重新输入！" << endl;
                break;
        }
    }
    return 0;
}





