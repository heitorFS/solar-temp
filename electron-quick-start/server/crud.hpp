#ifndef INCLUDE_CRUD_
#define INCLUDE_CRUD_

#include <sqlite3.h>

#include <string>

using namespace std;

template <int N, typename ...T>
decltype(auto) get_argument(const T&... t)
{
    return get<N>(forward_as_tuple(t...));
}

void formatQuery(string& base, string& query, string& table)
{
    base.erase(base.length() - 1);
}

template <typename T, typename... Args>
void formatQuery(string& base, string& query, string& table, T& arg, Args&... args)
{
    base += fmt::format(typeid(arg.value) == typeid(string) ? query : remove_single_quotes(query, REMOVE), arg.name, arg.value, arg.extra != NULL ? fmt::format(" AND {}", arg.extra) : "");
    formatQuery(base, query, table, args...);
}

static int close_connection(void* _db, int count, char** data, char** columns)
{
    sqlite3 *db = (sqlite3*)_db;
    return sqlite3_close(db);
}

namespace crud
{
    template <typename T, typename... Args>
    void create(Args... args)
    {
        sqlite3* db;
        sqlite3_open("C:/AEnAzume/database.db", &db); 
        string query = fmt::format("INSERT INTO {} ({}) VALUES({});", T::get_table(INSERT), T::get_fields(INSERT), T::get_template());

        string sql = 
            fmt::format(query,
                forward<Args>(args)...);

        sqlite3_exec(db, sql.c_str(), close_connection, db, NULL);
    }

    template <typename T>
    string read(string* extra = NULL)
    {
        sqlite3* db;
        sqlite3_open("C:/AEnAzume/database.db", &db);

        auto q_array = json::array();
        sqlite3_stmt* stmt;
        string sql = 
            fmt::format("SELECT {} FROM {} {}", T::get_fields(SELECT), T::get_table(SELECT), extra != NULL ? *extra : "");

        int step = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        while ((step = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            q_array.push_back(T(stmt));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return to_string(q_array);
    }

    template <typename T, typename... Args>
    void update(size_t id, Args... args)
    {
        sqlite3* db;
        sqlite3_open("C:/AEnAzume/database.db", &db);

        string sql = fmt::format("UPDATE {} SET", T::get_table(INSERT)), query = " {} = '{}',";
        formatQuery(sql, query, T::get_table(INSERT), forward<Args>(args)...);
        sql += fmt::format(" WHERE id = {}", id);

        sqlite3_exec(db, sql.c_str(), close_connection, db, NULL);
    }

    template <typename T>
    void remove(size_t id)
    {
        sqlite3* db;
        int err = sqlite3_open("C:/AEnAzume/database.db", &db);

        string sql = 
            fmt::format("DELETE FROM {} WHERE id = {};",
                T::get_table(DELETE),
                id);

        err = sqlite3_exec(db, sql.c_str(), close_connection, db, NULL);
    }
}

#endif // INCLUDE_CRUD_