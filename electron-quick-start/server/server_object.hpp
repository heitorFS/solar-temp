#ifndef INCLUDE_SERVER_OBJECT_
#define INCLUDE_SERVER_OBJECT_

#define DUPLICATE true
#define REMOVE false

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