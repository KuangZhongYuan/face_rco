#ifndef MYSQLITE_H // ���û�ж���MYSQLITE_H��
#define MYSQLITE_H // ����MYSQLITE_H��

#include <iostream>           // ����iostream�⣬��������/�������
#include <string.h>           // ����string.h�⣬�����ַ�������
#include <sqlite3.h>          // ����sqlite3�⣬����SQLite���ݿ����
#include <vector>             // ����vector�⣬����ʹ���������ݽṹ
#include "face_recognition.h" // ����face_recognitionͷ�ļ�

class FaceRecognition; // ǰ������FaceRecognition��

class MySqlite // ����һ����ΪMySqlite����
{
public:          // ����ĳ�Ա�ǹ��еģ�����������ⲿ����
    MySqlite();  // ��Ĺ��캯��
    ~MySqlite(); // �����������

    // �����������浽SQLite���ݿ���
    /**
     * @brief save_face_feature
     * @param id    ���˱��
     * @param name  �˵�����
     * @param addr  ��������ֵ����ʼ��ַ
     * @param size  ��������ֵ���ֽ���
     * @return ����ɹ����� true, ���򷵻�false
     */
    bool save_face_feature(int id, std::string name,
                           const void *pfeature,
                           int feature_size); // һ����Ա���������ڱ�������������SQLite���ݿ�

    // ��ѯSQLite���ݿ��е���������,������ƥ��
    std::string query_name_by_feature(const std::vector<float> &feature1,
                                      const FaceRecognition &mfr); // һ����Ա����������ͨ������������ѯ����
    //ɾ��һ�����ݿ����Ѵ��ڵ���Ϣ
    bool delete_face_feature(int id, std::string name);

    //��ʾ���ݿ��д��ڵ�������Ϣ
    void show_face_feature();

private:                 // ����ĳ�Ա��˽�еģ�ֻ������ĳ�Ա�����з���
    sqlite3 *db;         // SQLite���ݿ��ָ��
    sqlite3_stmt *pstmt; // SQLiteԤ��������ָ��
};

#endif // ������MYSQLITE_H����ж�