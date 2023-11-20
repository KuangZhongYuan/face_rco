#include "mysqlite.h"
using namespace std;

MySqlite::MySqlite() // ���캯��
{
    int r = sqlite3_open("../Face.db", &db);
    if (r != SQLITE_OK) {
        cerr << "open db error: " << sqlite3_errmsg(db) << endl;
        throw std::runtime_error(string("open db error: ") + sqlite3_errmsg(db));
    }
    cout << "init db ok!!" << endl;
}

MySqlite::~MySqlite() // ��������
{
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
}

bool MySqlite::save_face_feature(int id, std::string name, const void *pfeature, int feature_size)
{
    // ����SQL���
    sqlite3_stmt *pstmt;
    string sql_insert = "insert into t_face(id,name,feature) values(?,?,?);";
    // ׼��SQL��䣨���ݿ����Ӷ���SQL��䣬SQL��䳤�ȣ�ָ��Ԥ��������ָ�룬ָ��ʣ��δʹ������ָ�룩
    int r = sqlite3_prepare(db, sql_insert.c_str(), sql_insert.size(), &pstmt, NULL);
    if (r != SQLITE_OK) {
        cerr << "prepare insert error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // ��id��int���ͣ��󶨵�Ԥ�������Ĳ�����
    r = sqlite3_bind_int(pstmt, 1, id);
    if (r != SQLITE_OK) {
        cerr << "bind int error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // ��name��string�ַ������ͣ��󶨵�Ԥ�������Ĳ�����
    r = sqlite3_bind_text(pstmt, 2, name.c_str(), name.size(), NULL);
    if (r != SQLITE_OK) {
        cerr << "bind text error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // ��pfeature��blob���������ͣ��󶨵�Ԥ�������Ĳ�����
    r = sqlite3_bind_blob(pstmt, 3, pfeature, feature_size, NULL);
    if (r != SQLITE_OK) {
        cerr << "bind blob error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // ִ��Ԥ�������--->�����ݲ��뵽���ݿ���
    r = sqlite3_step(pstmt);
    cout << "r= " << r << endl;
    if (r != SQLITE_DONE) {
        cout << "step int error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

// ��ѯSQLite���ݿ��е���������,������ƥ��
std::string MySqlite::query_name_by_feature(const std::vector<float> &feature1, const FaceRecognition &fr)
{
    string rname = "��֤ʧ��-->name not found";
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
                cout << "��֤�ɹ���" << endl << "name = ";
                rname = reinterpret_cast<const char *>(name);
                break;
            }
        }
    }
    return rname;
}

// ɾ��һ�����ݿ����Ѵ��ڵ���Ϣ
bool MySqlite::delete_face_feature(int id, std::string name)
{
    sqlite3_stmt *pstmt; // ����һ��sqlite3_stmt���͵�ָ��pstmt������ִ��SQL���

    // ����һ��SQLɾ����䣬����ɾ��t_face����id��name��ƥ��ļ�¼
    string sql_delete = "delete from t_face where id = ? and name = ?;";

    // ʹ��sqlite3_prepare_v2����Ԥ����SQLɾ����䣬����洢��pstmt��
    sqlite3_prepare_v2(db, sql_delete.c_str(), sql_delete.size(), &pstmt, NULL);

    // ʹ��sqlite3_bind_int������id�󶨵�SQLɾ�����ĵ�һ������λ��
    int r = sqlite3_bind_int(pstmt, 1, id);
    if (r != SQLITE_OK)
    {                                                             // �����ʧ��
        cerr << "bind int error: " << sqlite3_errmsg(db) << endl; // ���������Ϣ
        return false;                                             // ����false��ʾɾ��ʧ��
    }

    // ʹ��sqlite3_bind_text������name�󶨵�SQLɾ�����ĵڶ�������λ��
    r = sqlite3_bind_text(pstmt, 2, name.c_str(), name.size(), NULL);
    if (r != SQLITE_OK)
    {                                                              // �����ʧ��
        cerr << "bind text error: " << sqlite3_errmsg(db) << endl; // ���������Ϣ
        return false;                                              // ����false��ʾɾ��ʧ��
    }

    // ʹ��sqlite3_step����ִ��Ԥ�������SQLɾ�����
    r = sqlite3_step(pstmt);
    if (r != SQLITE_DONE)
    {                                                             // ���ִ��ʧ��
        cout << "step int error: " << sqlite3_errmsg(db) << endl; // ���������Ϣ
        return false;                                             // ����false��ʾɾ��ʧ��
    }

    return true; // ������в������ɹ�������true��ʾɾ���ɹ�
}

// ��ʾ���ݿ��д��ڵ�������Ϣ
void MySqlite::show_face_feature()
{
    sqlite3_stmt *pstmt; // ����һ��sqlite3_stmt���͵�ָ��pstmt������ִ��SQL���

    // ����һ��SQL��ѯ��䣬���ڲ�ѯt_face�������м�¼
    string sql_select = "select id,name from t_face;";

    // ʹ��sqlite3_prepare_v2����Ԥ����SQL��ѯ��䣬����洢��pstmt��
    sqlite3_prepare_v2(db, sql_select.c_str(), sql_select.size(), &pstmt, NULL);

    // ʹ��sqlite3_step����ִ��Ԥ�������SQL��ѯ���
    while (sqlite3_step(pstmt) == SQLITE_ROW)
    {
        // ��ȡid�е�ֵ�������
        int id = sqlite3_column_int(pstmt, 0);
        cout << "id = " << id << ", ";

        // ��ȡname�е�ֵ�������
        const unsigned char *name = sqlite3_column_text(pstmt, 1);
        cout << "name = " << name << ", ";

        // // ��ȡfeature�е�ֵ�������
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