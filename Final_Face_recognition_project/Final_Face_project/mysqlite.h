#ifndef MYSQLITE_H // 如果没有定义MYSQLITE_H宏
#define MYSQLITE_H // 定义MYSQLITE_H宏

#include <iostream>           // 包含iostream库，用于输入/输出操作
#include <string.h>           // 包含string.h库，用于字符串操作
#include <sqlite3.h>          // 包含sqlite3库，用于SQLite数据库操作
#include <vector>             // 包含vector库，用于使用向量数据结构
#include "face_recognition.h" // 包含face_recognition头文件

class FaceRecognition; // 前向声明FaceRecognition类

class MySqlite // 定义一个名为MySqlite的类
{
public:          // 下面的成员是公有的，可以在类的外部访问
    MySqlite();  // 类的构造函数
    ~MySqlite(); // 类的析构函数

    // 人脸特征保存到SQLite数据库中
    /**
     * @brief save_face_feature
     * @param id    个人编号
     * @param name  人的姓名
     * @param addr  人脸特征值的起始地址
     * @param size  人脸特征值的字节数
     * @return 保存成功返回 true, 否则返回false
     */
    bool save_face_feature(int id, std::string name,
                           const void *pfeature,
                           int feature_size); // 一个成员函数，用于保存人脸特征到SQLite数据库

    // 查询SQLite数据库中的人脸特征,并进行匹配
    std::string query_name_by_feature(const std::vector<float> &feature1,
                                      const FaceRecognition &mfr); // 一个成员函数，用于通过人脸特征查询姓名
    //删除一条数据库中已存在的信息
    bool delete_face_feature(int id, std::string name);

    //显示数据库中存在的所有信息
    void show_face_feature();

private:                 // 下面的成员是私有的，只能在类的成员函数中访问
    sqlite3 *db;         // SQLite数据库的指针
    sqlite3_stmt *pstmt; // SQLite预处理语句的指针
};

#endif // 结束对MYSQLITE_H宏的判断