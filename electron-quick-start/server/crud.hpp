#ifndef INCLUDE_CRUD_
#define INCLUDE_CRUD_

#define COMMA false
#define AND true

#include <sqlite3.h>

#include <string>
#include <tuple>

using namespace std;

template<typename T>
struct field
{
    string name;
    T value;

    field(string _name, T _value) : name(_name), value(_value) 
    {}
};

enum query_type
{
    INSERT = 0,
    UPDATE = 0,
    DELETE = 0,
    VALIDATE = 0,
    SELECT_FULL,
    SELECT_SHORT
};

struct query_info
{
    int page;
    int items_page;
    string order_by;
    query_type type;
    json filter;
};

template <typename T>
void from_json(const json& j, query_info& obj)
{
    j.at("page").get_to(obj.page);
    j.at("items_page").get_to(obj.items_page);
    j.at("order_by").get_to(obj.order_by)
    j.at("filter").get_to(obj.filter);
}

string get_filter(json& j_filter)
{
    for (auto it = j_filter.begin(); it != j_filter.end(); ++it)
    {
        cout << "";
    }
}

template <int N, typename ...T>
decltype(auto) get_argument(const T&... t)
{
    return get<N>(forward_as_tuple(t...));
}

template<typename T>
void format_helper(string& base, string& query, bool separator_and, bool& first, field<T> arg)
{
    if (!first)
{
        base += separator_and ? " AND" : ",";
    }
    else     
        first = false;

    base += fmt::format(query, arg.name, arg.value);
}

template <typename... T>
void format_query(string& base, string& query, string& table, bool separator_and, tuple<T...> t)
{
    bool first = true;
    std::apply([&base, &query, separator_and, &first](auto&&... args) {
        (format_helper(base, query, separator_and, first, args), ...);
    }, t);
}

static int close_connection(void* _db, int count, char** data, char** columns)
{
    sqlite3 *db = (sqlite3*)_db;
    return sqlite3_close(db);
}

namespace crud
{
    template <typename T, typename... Args>
    int64_t create(Args... args)
    {
        sqlite3* db;
        sqlite3_open("C:/AEnAzume/database.db", &db); 
        string query = fmt::format("INSERT INTO {} ({}) VALUES({});", T::get_table(INSERT), T::get_fields(INSERT), T::get_template());

        string sql = 
            fmt::format(query,
                forward<Args>(args)...);

        sqlite3_exec(db, sql.c_str(), NULL, db, NULL);
        int64_t last_id = sqlite3_last_insert_rowid(db);

        sqlite3_close(db);
        return last_id;
    }

    template <typename T>
    string read(string* extra = NULL, query_type type = SELECT_FULL)
    {
        sqlite3* db;
        sqlite3_open("C:/AEnAzume/database.db", &db);

        auto q_array = json::array();
        sqlite3_stmt* stmt;
        string sql = 
            fmt::format("SELECT {} FROM {} {}", T::get_fields(type), T::get_table(type), extra != NULL ? *extra : "");

        int step = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        while ((step = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            q_array.push_back(T(type, stmt));
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

        string sql = fmt::format("UPDATE {} SET", T::get_table(UPDATE)), query = " {} = '{}',";
        formatQuery(sql, query, T::get_table(UPDATE), forward<Args>(args)...);
        sql += fmt::format(" WHERE id = {}", id);

        sqlite3_exec(db, sql.c_str(), close_connection, db, NULL);
    }

    template <typename T>
    void remove(field<size_t>& _field)
    {
        sqlite3* db;
        int err = sqlite3_open("C:/AEnAzume/database.db", &db);

        string sql = 
            fmt::format("DELETE FROM {} WHERE {} = {};",
                T::get_table(DELETE),
                _field.name,
                _field.value);

        err = sqlite3_exec(db, sql.c_str(), close_connection, db, NULL);
    }
}

#endif // INCLUDE_CRUD_