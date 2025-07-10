#ifndef INCLUDE_SERVER_OBJECT_
#define INCLUDE_SERVER_OBJECT_

#define DUPLICATE true
#define REMOVE false
#define SELECT true
#define INSERT false
#define DELETE false
#define VALIDATE false

#include "crud.hpp"
#include "validations.hpp"
#include "CpfCnpjValidacao.hpp"

#include <fmt/core.h>
#include <napi.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <sqlite3.h>

using namespace std;
using json = nlohmann::json;

#pragma region Helper Classes

template<typename T>
struct field
{
    string name;
    T value;
    const char* extra = NULL;

    field(string _name, T _value) : name(_name), value(_value) 
    {}

    field(string _name, T _value, const char* _extra) : name(_name), value(_value), extra(_extra)
    {}
};

enum field_error_code
{
    INVALID_FIELD,
    NOT_UNIQUE
};

struct field_error
{
    string name;
    field_error_code err;

    field_error(string _name, field_error_code _err) : name(_name), err(_err)
    {}
};
    
void to_json(nlohmann::json& j, field_error& obj)
{
    j = {{ "name", obj.name }, { "error_type", obj.err }};
}

struct date
{
    static string dash_to_slash(string _d)
    {
        return fmt::format("{}/{}/{}", _d.substr(8, 2), _d.substr(5, 2), _d.substr(0, 4));
    }

    static bool validate_date(string _d)
    {
        if (regex_search(_d, regex("^([0-9]){4}-([0-9]){2}-([0-9]){2}$", regex_constants::ECMAScript)))
            return true;
        return false;
    }
};

#pragma endregion Helper Classes

#pragma region Helper Methods

template <typename... Args>
bool validate_unique(json& ret, string table, Args... args)
{
    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    sqlite3_stmt* stmt;
    string sql = "SELECT", query = " COUNT(CASE WHEN {} = '{}'{} THEN 1 ELSE null END),";
    formatQuery(sql, query, table, forward<Args>(args)...);
    sql += fmt::format(" FROM {}", table);

    err = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (err != SQLITE_OK)
        return false;

    err = sqlite3_step(stmt);
    bool unique = true;
    const size_t count = sizeof...(Args);
    for (int i = 0; i < count; ++i)
    {
        if (sqlite3_column_int(stmt, i) > 0)
        {
            ret.push_back(field_error(get_argument<0>(args...).name, field_error_code::NOT_UNIQUE));
            unique = false;
        }
    }

    sqlite3_finalize(stmt);  
    sqlite3_close(db);
    return unique;
}

string remove_single_quotes(string txt, bool duplicate)
{
    size_t location = txt.find('\'');
    while (location != string::npos)
    {
        txt.replace(location, 1, duplicate ? "''" : "");
        location = txt.find('\'', location + 1);
    }

    return txt;
}

#pragma endregion Helper Methods

#endif // INCLUDE_SERVER_OBJECT_