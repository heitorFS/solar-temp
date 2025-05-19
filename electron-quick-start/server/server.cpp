#include "server.h"

using namespace std;

#pragma region HelperClasses

class InvalidFieldException : exception
{
    string id;

    public:
        InvalidFieldException(string id_param)
        {
            id = id_param;
        }

        string what()
        {
            return "O campo " + id + " possui caracteres inválidos";
        }
};

#pragma endregion HelperClasses

#pragma region HelperMethods

bool validateLetters(string txt, string id)
{
    size_t length = txt.length();
    for (int i = 0; i < length; ++i)
    {
        if (
            (txt[i] < 65 ||
            (txt[i] > 90 && txt[i] < 97) ||
            txt[i] > 122) && txt[i] != 32
        )
            throw InvalidFieldException(id);
    }

    return true;
}

bool validateEmail(string txt)
{
    size_t length = txt.length();
    size_t atLoc = length - 1;

    for (; txt[atLoc] != '@'; --atLoc);

    string str = txt.substr(0, atLoc);

    if (str[0] == '.' || str[str.length() - 1] == '.')
        throw InvalidFieldException("email");

    for (int i = 0; i < length; ++i)
    {
        if (
            (str[i] == '(' ||
            str[i] == ')' ||
            str[i] == '<' ||
            str[i] == '>' ||
            str[i] == ',' ||
            str[i] == ';' ||
            str[i] == ':' ||
            str[i] == '\\' ||            
            str[i] == '[' ||
            str[i] == ']' ||
            str[i] == '@') && (str[i - 1] != '"' || str[i + 1] != '"')
        )
            throw InvalidFieldException("email");
    }

    return true;
}

string removeSingleQuotes(string txt)
{
    size_t location = txt.find('\'');
    while (location != string::npos)
    {
        txt.replace(location, 1, "''");
        location = txt.find('\'', location + 1);
    }

    return txt;
}

#pragma endregion HelperMethods

#pragma region MainMethods

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

bool server::createColaborador(Colaborador& colaborador)
{    
    MYSQL *conn = mysql_init(NULL);
    
    if(conn == NULL)
    {
        throw exception("Connection is null");
    }

    if (!mysql_real_connect(conn, SERVER, USERNAME, PASSWORD, DATABASE, PORT, NULL, NULL))
    {
        throw exception("Could not connect");
    }

    string query = fmt::format("INSERT INTO colaboradores VALUES('{}', '{}', '{}', '{}', '{}')", colaborador.Nome, colaborador.Id_cargo, colaborador.Email, colaborador.Telefone, colaborador.Cpf_cnpj);

    if (mysql_query(conn, query.c_str()))
        throw exception("Could not run query");

    
    mysql_close(conn);    
    return true;
}

#pragma endregion MainMethods

#pragma region WrappedMethods

Napi::Number server::addWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber())
        Napi::TypeError::New(env, "arg1::Number, arg2::Number expected")
            .ThrowAsJavaScriptException();

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

Napi::Boolean server::createColaboradorWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 5 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsString() || !info[3].IsString() || !info[4].IsString())
        Napi::TypeError::New(env, "nome::String, id_cargo::Number, email::String, telefone::String, cpf_cnpj::String expected")
            .ThrowAsJavaScriptException();

    try
    {
        server::Colaborador colaborador(
            removeSingleQuotes(info[0].As<Napi::String>().Utf8Value()),
            info[1].As<Napi::Number>().Int32Value(),
            removeSingleQuotes(info[2].As<Napi::String>().Utf8Value()),
            removeSingleQuotes(info[3].As<Napi::String>().Utf8Value()),
            removeSingleQuotes(info[4].As<Napi::String>().Utf8Value())
        ); 

        validateEmail(colaborador.Email);

        server::createColaborador(colaborador);

        Napi::Boolean result = Napi::Boolean::New(env,
            createColaborador(colaborador));

        return result;
    }
    catch (InvalidFieldException exc)
    {
        Napi::Error::New(env, exc.what()).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    catch (exception exc)
    {
        Napi::Error::New(env, exc.what()).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
}

#pragma endregion WrappedMethods

Napi::Object server::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("add", Napi::Function::New(env, server::addWrapped));
    exports.Set("getAllClientes", Napi::Function::New(env, server::getAllClientesWrapped));
    exports.Set("createColaborador", Napi::Function::New(env, server::createColaboradorWrapped));
    return exports;
}