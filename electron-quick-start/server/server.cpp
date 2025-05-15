#include "server.h"

using namespace std;

int server::add(int x, int y)
{
    return (x + y);
}

int server::getAllClientes()
{
    MYSQL *conn = mysql_init(NULL);
    
    if(conn == NULL)
        return 1;

    if (!mysql_real_connect(conn, SERVER, USERNAME, PASSWORD, DATABASE, PORT, NULL, NULL))
        return 2;

    if (mysql_query(conn, "SELECT * FROM colaboradores"))
        return 3;

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL)
        return 4;
    
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; ++i)
        {

        }
    }

    mysql_free_result(result);
    mysql_close(conn);

    return 0;
}

Napi::Number server::addWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "arg1::Number, arg2::Number expected").ThrowAsJavaScriptException();
    }

    Napi::Number result = Napi::Number::New(env,
        add(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value()));

    return result;
}

Napi::Number server::getAllClientesWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    Napi::Number ret = Napi::Number::New(env, getAllClientes());
    return ret;
}

Napi::Object server::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("add", Napi::Function::New(env, server::addWrapped));
    exports.Set("getAllClientes", Napi::Function::New(env, server::getAllClientesWrapped));
    return exports;
}