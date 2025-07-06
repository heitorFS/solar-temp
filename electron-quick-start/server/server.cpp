#include "server.h"

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

#pragma endregion Helper Classes

#pragma region Helper Methods

template <typename... Args>
bool validate_unique(json& ret, string table, Args... args)
{
    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    sqlite3_stmt* stmt;
    string sql = "SELECT", query = " COUNT(CASE WHEN {} = '{}'{} THEN 1 ELSE null END),";
    server::formatQuery(sql, query, table, forward<Args>(args)...);
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
            ret.push_back(server::field_error(server::get_argument<0>(args...).name, server::field_error_code::NOT_UNIQUE));
            unique = false;
        }
    }

    sqlite3_finalize(stmt);  
    sqlite3_close(db);
    return unique;
}

#pragma region CRUD operations

template <typename T, typename... Args>
void create(Args... args)
{
    sqlite3* db;
    sqlite3_open("C:/AEnAzume/database.db", &db); 
    string query = fmt::format("INSERT INTO {} ({}) VALUES({});", T::get_table(), T::get_fields(), T::get_template());

    const char* sql = 
        fmt::format(query,
            forward<Args>(args)...).c_str();

    sqlite3_exec(db, sql, server::close_connection, db, NULL);
}

template <typename T>
string read()
{
    sqlite3* db;
    sqlite3_open("C:/AEnAzume/database.db", &db);

    auto q_array = json::array();
    sqlite3_stmt* stmt;
    const char* sql = 
        fmt::format("SELECT * FROM {}", T::get_table()).c_str();

    int step = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
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

    string sql = fmt::format("UPDATE {} SET", T::get_table()), query = " {} = '{}',";
    server::formatQuery(sql, query, T::get_table(), forward<Args>(args)...);
    sql += fmt::format(" WHERE id = {}", id);

    sqlite3_exec(db, sql.c_str(), server::close_connection, db, NULL);
}

template <typename T>
void remove(size_t id)
{
    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    const char* sql = 
        fmt::format("DELETE FROM {} WHERE id = {};",
            T::get_table(),
            id).c_str();

    err = sqlite3_exec(db, sql, server::close_connection, db, NULL);
}

#pragma endregion CRUD operations

#pragma region Char Validations

bool validate_letters(string& txt)
{
    size_t length = txt.length();
    for (int i = 0; i < length; ++i)
    {
        if (
            (txt[i] < 65 ||
            (txt[i] > 90 && txt[i] < 97) ||
            (txt[i] > 122 && txt[i] < 192) ||
            txt[i] > 255) && txt[i] != 32 && txt[i] != 39 && txt[i] != 45
        )
            return false;
    }

    return true;
}

bool validate_numbers(string& txt)
{
    size_t length = txt.length();
    for (int i = 0; i < length; ++i)
    {
        if (
            txt[i] < 48 ||
            txt[i] > 57
        )
            return false;
    }

    return true;
}

bool validate_email(string& txt)
{
    size_t length = txt.length();
    size_t atLoc = length - 1;

    for (; txt[atLoc] != '@'; --atLoc);

    string str = txt.substr(0, atLoc);

    if (str[0] == '.' || str[str.length() - 1] == '.')
        return false;

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
            return false;
    }

    return true;
}

#pragma endregion Char Validations

#pragma endregion Helper Methods

#pragma region Main Methods

int server::add(int x, int y)
{
    return (x + y);
}

bool server::setupDatabase()
{
    ifstream f("C:/AEnAzume/database.db");
    if (f.good())
        return true;

    ofstream { "C:/AEnAzume/database.db" };

    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    string sql = 
        "create table colaboradores ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nome VARCHAR(120) NOT NULL,"
        "id_cargo UNSIGNED TINYINT NOT NULL,"
        "email VARCHAR(254) NOT NULL,"
        "telefone VARCHAR(11) NOT NULL,"
        "cpf_cnpj VARCHAR(14) NOT NULL UNIQUE"
        ");"
        "INSERT INTO colaboradores (nome, id_cargo, email, telefone, cpf_cnpj) VALUES( 'uaidhwuiawd', '1', 'aiuwdh@iufahw', '19785198247', '11111111111');"
        "create table origens ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nome VARCHAR(254) NOT NULL UNIQUE"
        ");"
        "CREATE TABLE clientes ("
        "id INTEGER PRIMARY KEY,"
        "nome VARCHAR(120) NOT NULL,"
        "email VARCHAR(254) NOT NULL,"
        "proprietario INTEGER NOT NULL,"
        "cpf_cnpj VARCHAR(14) NOT NULL,"
        "telefone VARCHAR(11) NOT NULL,"
        "cep CHAR(8) NOT NULL,"
        "numero INTEGER NOT NULL,"
        "complemento VARCHAR(120),"
        "origem INTEGER NOT NULL,"
        "data_origem DATE,"
        "extra_nome VARCHAR(120),"
        "extra_cpf CHAR(11),"
        "extra_rg VARCHAR(13),"
        "extra_nacionalidade VARCHAR(20),"
        "extra_profissao VARCHAR(120),"
        "extra_renda DOUBLE,"
        "observacoes VARCHAR(700),"
        "FOREIGN KEY (proprietario)"
        "    REFERENCES colaboradores (id),"
        "FOREIGN KEY (origem)"
        "    REFERENCES origens (id)"
        ");";

    char* errmsg;
    err = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);
    sqlite3_close(db);
    if (err != SQLITE_OK)
        return false;

    return true;
}

#pragma endregion Main Methods

#pragma region Wrapped Methods

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

Napi::Boolean server::setupDatabaseWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env(); 
    if (!setupDatabase())
        Napi::Error::New(env, "Não foi possível criar o banco de dados. Erro: ").ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, true);
}

Napi::String server::getAllClientesWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    return Napi::String::New(env, read<cliente>());
}

Napi::Boolean server::createClienteWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 17 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsString() || !info[4].IsString() || !info[5].IsString() || !info[6].IsNumber() || !info[7].IsString() || !info[8].IsNumber() || !info[9].IsString() ||  !info[10].IsString() || !info[11].IsString() || !info[12].IsString() || !info[13].IsString() || !info[14].IsString() || !info[15].IsNumber() || !info[16].IsString())
        Napi::TypeError::New(env, "nome::String, email::String, proprietario::Number, cpf_cnpj::String, telefone::String, cep::String, numero::Number, complemento::String, origem::Number, data_origem::String, extra_nome::String, extra_cpf::String, extra_rg::String, extra_nacionalidade::String, extra_profissao::String, extra_renda::Number, observacoes::String expected")
            .ThrowAsJavaScriptException();

    server::cliente _cliente(
        remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[1].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[2].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[5].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[6].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[7].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[8].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[9].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[10].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[11].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[12].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[13].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[14].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[15].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[16].As<Napi::String>().Utf8Value(), DUPLICATE)
    ); 

    json errs = json::array({});
    if (!validate_letters(_cliente.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
    if (!validate_email(_cliente.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.cep)) errs.push_back(field_error("cep", field_error_code::INVALID_FIELD));
    if (!date::validate_date(_cliente.data_origem)) errs.push_back(field_error("data_origem", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.nome)) errs.push_back(field_error("extra_nome", field_error_code::INVALID_FIELD));
    if (_cliente.extra_cpf.length() > 0 && (!validate_numbers(_cliente.extra_cpf) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.extra_cpf.c_str()))) errs.push_back(field_error("extra_cpf", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.extra_rg)) errs.push_back(field_error("extra_rg", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.extra_nacionalidade)) errs.push_back(field_error("extra_nacionalidade", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.extra_profissao)) errs.push_back(field_error("extra_profissao", field_error_code::INVALID_FIELD));

    validate_unique(errs, cliente::get_table(), field<string>("cpf_cnpj", _cliente.cpf_cnpj));
    
    if (errs.size() > 0)
    {
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    create<cliente>(_cliente.nome, _cliente.email, _cliente.proprietario, _cliente.cpf_cnpj, _cliente.telefone, _cliente.cep, _cliente.numero, _cliente.complemento);
    return Napi::Boolean::New(env, true);
}

Napi::Boolean server::updateClienteWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 17 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsString() || !info[4].IsString() || !info[5].IsString() || !info[6].IsNumber() || !info[7].IsString() || !info[8].IsNumber() || !info[9].IsString() ||  !info[10].IsString() || !info[11].IsString() || !info[12].IsString() || !info[13].IsString() || !info[14].IsString() || !info[15].IsNumber() || !info[16].IsString())
        Napi::TypeError::New(env, "nome::String, email::String, proprietario::Number, cpf_cnpj::String, telefone::String, cep::String, numero::Number, complemento::String, origem::Number, data_origem::String, extra_nome::String, extra_cpf::String, extra_rg::String, extra_nacionalidade::String, extra_profissao::String, extra_renda::Number, observacoes::String expected")
            .ThrowAsJavaScriptException();

    server::cliente _cliente(
        remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[1].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[2].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[5].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[6].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[7].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[8].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[9].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[10].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[11].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[12].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[13].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[14].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[15].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[16].As<Napi::String>().Utf8Value(), DUPLICATE)
    ); 

    json errs = json::array({});
    if (!validate_letters(_cliente.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
    if (!validate_email(_cliente.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.cep)) errs.push_back(field_error("cep", field_error_code::INVALID_FIELD));
    if (!date::validate_date(_cliente.data_origem)) errs.push_back(field_error("data_origem", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.nome)) errs.push_back(field_error("extra_nome", field_error_code::INVALID_FIELD));
    if (_cliente.extra_cpf.length() > 0 && (!validate_numbers(_cliente.extra_cpf) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.extra_cpf.c_str()))) errs.push_back(field_error("extra_cpf", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_cliente.extra_rg)) errs.push_back(field_error("extra_rg", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.extra_nacionalidade)) errs.push_back(field_error("extra_nacionalidade", field_error_code::INVALID_FIELD));
    if (!validate_letters(_cliente.extra_profissao)) errs.push_back(field_error("extra_profissao", field_error_code::INVALID_FIELD));

    validate_unique(errs, cliente::get_table(), field<string>("cpf_cnpj", _cliente.cpf_cnpj));
    
    if (errs.size() > 0)
    {
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
        
    if (errs.size() > 0)
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();

    update<cliente>(_cliente.id, field<string>("nome", _cliente.nome), field<string>("email",  _cliente.email),
        field<int>("proprietario", _cliente.proprietario), field<string>("cpf_cnpj", _cliente.cpf_cnpj),
        field<string>("telefone", _cliente.telefone), field<string>("cep", _cliente.cep),
        field<int>("origem", _cliente.origem), field<string>("data_origem", _cliente.data_origem),
        field<string>("extra_nome", _cliente.extra_nome), field<string>("extra_rg", _cliente.extra_rg),
        field<string>("extra_nacionalidade", _cliente.extra_nacionalidade), field<string>("extra_profissao", _cliente.extra_profissao),
        field<double>("extra_renda", _cliente.extra_renda), field<string>("observacoes", _cliente.observacoes));
    return Napi::Boolean::New(env, true);
}

Napi::Boolean server::deleteClienteWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
        Napi::TypeError::New(env, "id::number")
            .ThrowAsJavaScriptException();

    json errs = json::array({});
    size_t id = info[0].As<Napi::Number>().Int32Value();
    if (validate_unique(errs, cliente::get_table(), field<size_t>("id", id)))
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
    remove<colaborador>(id);

    return Napi::Boolean::New(env, true);
}

Napi::String server::getAllColaboradoresWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    return Napi::String::New(env, read<colaborador>());
}

Napi::Boolean server::createColaboradorWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 5 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsString() || !info[3].IsString() || !info[4].IsString())
        Napi::TypeError::New(env, "nome::String, id_cargo::Number, email::String, telefone::String, cpf_cnpj::String expected")
            .ThrowAsJavaScriptException();

    server::colaborador _colaborador(
        remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[1].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[2].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE)
    ); 

    json errs = json::array({});
    if (!validate_letters(_colaborador.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
    if (_colaborador.id_cargo < 1 || _colaborador.id_cargo > 5) errs.push_back(field_error("cargo", field_error_code::INVALID_FIELD));
    if (!validate_email(_colaborador.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_colaborador.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_colaborador.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_colaborador.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));

    validate_unique(errs, colaborador::get_table(), field<string>("cpf_cnpj", _colaborador.cpf_cnpj));

    if (errs.size() > 0)
    {
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    create<colaborador>(_colaborador.nome, _colaborador.id_cargo, _colaborador.email, _colaborador.telefone, _colaborador.cpf_cnpj);
    
    return Napi::Boolean::New(env, true);
}

Napi::Boolean server::updateColaboradorWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 6 || !info[0].IsNumber() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsString() || !info[4].IsString() || !info[5].IsString())
        Napi::TypeError::New(env, "id::Number, nome::String, id_cargo::Number, email::String, telefone::String, cpf_cnpj::String expected")
            .ThrowAsJavaScriptException();
            
    server::colaborador _colaborador(
        info[0].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[1].As<Napi::String>().Utf8Value(), DUPLICATE),
        info[2].As<Napi::Number>().Int32Value(),
        remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE),
        remove_single_quotes(info[5].As<Napi::String>().Utf8Value(), DUPLICATE)
    ); 

    json errs = json::array({});
    if (!validate_letters(_colaborador.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
    if (_colaborador.id_cargo < 1 || _colaborador.id_cargo > 5) errs.push_back(field_error("cargo", field_error_code::INVALID_FIELD));
    if (!validate_email(_colaborador.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_colaborador.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
    if (!validate_numbers(_colaborador.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_colaborador.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));

    validate_unique(errs, colaborador::get_table(), field<string>("cpf_cnpj", _colaborador.cpf_cnpj, fmt::format("id != {}", _colaborador.id).c_str()));
        
    if (errs.size() > 0)
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();

    update<colaborador>(_colaborador.id, field<string>("nome", _colaborador.nome), field<int>("id_cargo",  _colaborador.id_cargo),
        field<string>("email", _colaborador.email), field<string>("telefone", _colaborador.telefone),
        field<string>("cpf_cnpj", _colaborador.cpf_cnpj));

    return Napi::Boolean::New(env, true);
}

Napi::Boolean server::deleteColaboradorWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
        Napi::TypeError::New(env, "id::number")
            .ThrowAsJavaScriptException();

    json errs = json::array({});
    size_t id = info[0].As<Napi::Number>().Int32Value();
    if (validate_unique(errs, colaborador::get_table(), field<size_t>("id", id)))
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
    remove<colaborador>(id);

    return Napi::Boolean::New(env, true);
}

Napi::String server::getAllOrigensWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    return Napi::String::New(env, read<origem>());
}

Napi::Boolean server::createOrigemWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
        Napi::TypeError::New(env, "nome::String")
            .ThrowAsJavaScriptException();

    server::origem _origem(
        remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE)
    ); 

    json errs = json::array({});
    if (!validate_letters(_origem.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
    validate_unique(errs, "origens", field<string>("nome", _origem.nome));

    if (errs.size() > 0)
    {
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
        return Napi::Boolean::New(env, false);
    }

    create<origem>(_origem.nome);
    return Napi::Boolean::New(env, true);
}

Napi::Boolean server::deleteOrigemWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
        Napi::TypeError::New(env, "id::number")
            .ThrowAsJavaScriptException();

    json errs = json::array({});
    size_t id = info[0].As<Napi::Number>().Int32Value();
    if (validate_unique(errs, "origens", field<size_t>("id", id)))
        Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();

    remove<origem>(id);

    return Napi::Boolean::New(env, true);
}

#pragma endregion Wrapped Methods

Napi::Object server::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("add", Napi::Function::New(env, server::addWrapped));
    exports.Set("setupDatabase", Napi::Function::New(env, server::setupDatabaseWrapped));

    exports.Set("getAllClientes", Napi::Function::New(env, server::getAllClientesWrapped));
    exports.Set("createCliente", Napi::Function::New(env, server::createClienteWrapped));
    exports.Set("updateCliente", Napi::Function::New(env, server::updateClienteWrapped));
    exports.Set("deleteCliente", Napi::Function::New(env, server::deleteClienteWrapped));

    exports.Set("getAllColaboradores", Napi::Function::New(env, server::getAllColaboradoresWrapped));
    exports.Set("createColaborador", Napi::Function::New(env, server::createColaboradorWrapped));
    exports.Set("updateColaborador", Napi::Function::New(env, server::updateColaboradorWrapped));
    exports.Set("deleteColaborador", Napi::Function::New(env, server::deleteColaboradorWrapped));

    exports.Set("getAllOrigens", Napi::Function::New(env, server::getAllOrigensWrapped));
    exports.Set("createOrigem", Napi::Function::New(env, server::createOrigemWrapped));
    exports.Set("deleteOrigem", Napi::Function::New(env, server::deleteOrigemWrapped));
    return exports;
}