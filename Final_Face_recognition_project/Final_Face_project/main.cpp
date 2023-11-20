#include <iostream>
#include <cstdlib>
#include "face_recognition.h"
#include "mysqlite.h"

using namespace std;

//�˵�����
void menu()
{
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "-------------��ӭʹ��kk����������ʶ��ϵͳ------------------" << endl;
    cout << "---------------------------->���ܲ˵�<-------------------------" << endl;
    cout << "1������¼��      2��������ѯ        3��ɾ����Ϣ       4����ʾ����      0���˳�ϵͳ" << endl;
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
        cout << "������ѡ��: ";
        cin >> choice;
        switch(choice) {
            case 1: {
                cout << "��ѡ��Ĺ�����--->1������¼��" << endl;
                // ���ʶ������
                string loc_img_path;
                cout << "������Ҫ¼���ͼƬ��Ϣ��·��:" << endl;
                cin >> loc_img_path;
                fr.face_detect(loc_img_path.c_str());
                // ��ȡ��������ֵ
                auto feature1 = fr.face_feature();
                cout << feature1.total() * feature1.elemSize() << endl;
                // �洢����
                string lord_name;
                int lord_id;
                cout << "������Ҫ¼���������Ϣ��" << endl << "id:   name:" << endl;
                cin >> lord_id;
                cin >> lord_name;
                cout << "Ҫ¼�����ϢΪ��id= " << lord_id << "��name= " << lord_name << endl;
                if(true == mysql.save_face_feature(lord_id, lord_name, feature1.data, feature1.total() * feature1.elemSize())) {
                    cout << "¼��ɹ�" << endl;
                }
                //����������ʾ����
                fr.face_mark();
                cv::waitKey();
                cv::destroyAllWindows();
                break;
            }
            case 2: {
                cout << "��ѡ��Ĺ�����--->2��������ѯ" << endl;
                // ���ʶ������
                string find_img_path;
                cout << "������Ҫ��ѯ��ͼƬ��Ϣ��·��:" << endl;
                cin >> find_img_path;
                fr.face_detect(find_img_path.c_str());
                // ��ȡ��������ֵ
                auto feature2 = fr.face_feature();
                //cout << feature2.total() * feature2.elemSize() << endl;
                // // ��������ֵ��ѯ����
                std::string name = mysql.query_name_by_feature(feature2.reshape(1, feature2.total()), fr);
                cout << name << endl;
                //����������ʾ����
                fr.face_mark();
                cv::waitKey();
                cv::destroyAllWindows();
                break;
            }
            case 3: {
                cout << "��ѡ��Ĺ�����--->3��ɾ����Ϣ" << endl;
                int del_id;
                string del_name;
                cout << "������Ҫɾ������Ϣ:" << endl << "id:   name:" << endl;
                cin >> del_id;
                cin >> del_name;
                cout << "Ҫɾ������ϢΪ��id= " << del_id << "��name= " << del_name << endl;
                if(true == mysql.delete_face_feature(del_id, del_name)) {
                    cout << "ɾ���ɹ�" << endl;
                }
                break;
            }
            case 4: {
                cout << "��ѡ��Ĺ�����--->4����ʾ����" << endl;
                cout << endl;
                cout << "��ǰ�����Ѵ��ڵ���Ϣ�У�" << endl;
                mysql.show_face_feature();
                break;
            }
            case 0: {
                cout << "��ѡ��Ĺ�����--->0���˳�ϵͳ" << endl;
                //exit = true; // �˳�ѭ��
                exit(0);
                break;
            }
            default:
                cout << "��Ч��ѡ����������룡" << endl;
                break;
        }
    }
    return 0;
}





