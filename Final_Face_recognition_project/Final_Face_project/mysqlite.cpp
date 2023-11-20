#include "mysqlite.h"
using namespace std;

MySqlite::MySqlite() // 构造函数
{
    int r = sqlite3_open("../Face.db", &db);
    if (r != SQLITE_OK) {
        cerr << "open db error: " << sqlite3_errmsg(db) << endl;
        throw std::runtime_error(string("open db error: ") + sqlite3_errmsg(db));
    }
    cout << "init db ok!!" << endl;
}

MySqlite::~MySqlite() // 析构函数
{
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
}

bool MySqlite::save_face_feature(int id, std::string name, const void *pfeature, int feature_size)
{
    // 配置SQL语句
    sqlite3_stmt *pstmt;
    string sql_insert = "insert into t_face(id,name,feature) values(?,?,?);";
    // 准备SQL语句（数据库链接对象，SQL语句，SQL语句长度，指向预处理语句的指针，指向剩余未使用语句的指针）
    int r = sqlite3_prepare(db, sql_insert.c_str(), sql_insert.size(), &pstmt, NULL);
    if (r != SQLITE_OK) {
        cerr << "prepare insert error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // 将id（int类型）绑定到预处理语句的参数上
    r = sqlite3_bind_int(pstmt, 1, id);
    if (r != SQLITE_OK) {
        cerr << "bind int error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // 将name（string字符串类型）绑定到预处理语句的参数上
    r = sqlite3_bind_text(pstmt, 2, name.c_str(), name.size(), NULL);
    if (r != SQLITE_OK) {
        cerr << "bind text error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // 将pfeature（blob二进制类型）绑定到预处理语句的参数上
    r = sqlite3_bind_blob(pstmt, 3, pfeature, feature_size, NULL);
    if (r != SQLITE_OK) {
        cerr << "bind blob error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // 执行预处理语句--->将数据插入到数据库中
    r = sqlite3_step(pstmt);
    cout << "r= " << r << endl;
    if (r != SQLITE_DONE) {
        cout << "step int error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

// 查询SQLite数据库中的人脸特征,并进行匹配
std::string MySqlite::query_name_by_feature(const std::vector<float> &feature1, const FaceRecognition &fr)
{
    string rname = "验证失败-->name not found";
    string sql_select = "select * from t_face;";
    sqlite3_prepare_v2(db, sql_select.c_str(), sql_select.size(), &pstmt, NULL);

    while(1) {
        int r = sqlite3_step(pstmt);
        if (r == SQLITE_DONE) {
            break;
        }
        if (r == SQLITE_ROW) {
            // int id = sqlite3_column_int(pstmt, 0);
            const unsigned char *name = sqlite3_column_text(pstmt, 1);
            const float *pf2 = static_cast<const float *>(sqlite3_column_blob(pstmt, 2));
            int size2 = sqlite3_column_bytes(pstmt, 2);

            bool ok = fr(feature1, std::vector<float>(pf2, pf2 + size2 / sizeof(float)));
            if (ok) {
                cout << "验证成功：" << endl << "name = ";
                rname = reinterpret_cast<const char *>(name);
                break;
            }
        }
    }
    return rname;
}

// 删除一条数据库中已存在的信息
bool MySqlite::delete_face_feature(int id, std::string name)
{
    sqlite3_stmt *pstmt; // 定义一个sqlite3_stmt类型的指针pstmt，用于执行SQL语句

    // 定义一个SQL删除语句，用于删除t_face表中id和name都匹配的记录
    string sql_delete = "delete from t_face where id = ? and name = ?;";

    // 使用sqlite3_prepare_v2函数预处理SQL删除语句，结果存储在pstmt中
    sqlite3_prepare_v2(db, sql_delete.c_str(), sql_delete.size(), &pstmt, NULL);

    // 使用sqlite3_bind_int函数将id绑定到SQL删除语句的第一个参数位置
    int r = sqlite3_bind_int(pstmt, 1, id);
    if (r != SQLITE_OK)
    {                                                             // 如果绑定失败
        cerr << "bind int error: " << sqlite3_errmsg(db) << endl; // 输出错误信息
        return false;                                             // 返回false表示删除失败
    }

    // 使用sqlite3_bind_text函数将name绑定到SQL删除语句的第二个参数位置
    r = sqlite3_bind_text(pstmt, 2, name.c_str(), name.size(), NULL);
    if (r != SQLITE_OK)
    {                                                              // 如果绑定失败
        cerr << "bind text error: " << sqlite3_errmsg(db) << endl; // 输出错误信息
        return false;                                              // 返回false表示删除失败
    }

    // 使用sqlite3_step函数执行预处理过的SQL删除语句
    r = sqlite3_step(pstmt);
    if (r != SQLITE_DONE)
    {                                                             // 如果执行失败
        cout << "step int error: " << sqlite3_errmsg(db) << endl; // 输出错误信息
        return false;                                             // 返回false表示删除失败
    }

    return true; // 如果所有操作都成功，返回true表示删除成功
}

// 显示数据库中存在的所有信息
void MySqlite::show_face_feature()
{
    sqlite3_stmt *pstmt; // 定义一个sqlite3_stmt类型的指针pstmt，用于执行SQL语句

    // 定义一个SQL查询语句，用于查询t_face表中所有记录
    string sql_select = "select id,name from t_face;";

    // 使用sqlite3_prepare_v2函数预处理SQL查询语句，结果存储在pstmt中
    sqlite3_prepare_v2(db, sql_select.c_str(), sql_select.size(), &pstmt, NULL);

    // 使用sqlite3_step函数执行预处理过的SQL查询语句
    while (sqlite3_step(pstmt) == SQLITE_ROW)
    {
        // 获取id列的值，并输出
        int id = sqlite3_column_int(pstmt, 0);
        cout << "id = " << id << ", ";

        // 获取name列的值，并输出
        const unsigned char *name = sqlite3_column_text(pstmt, 1);
        cout << "name = " << name << ", ";

        // // 获取feature列的值，并输出
        // const void *p = sqlite3_column_blob(pstmt, 2);
        // int n = sqlite3_column_bytes(pstmt, 2);
        // cout << "feature = ";
        // for (int i = 0; i < n; i++)
        // {
        //     cout << hex << (int)((unsigned char *)p)[i] << " ";
        // }
        cout << endl;
    }
}