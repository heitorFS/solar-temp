#include "server.h"

using namespace std;

#pragma region Main Methods

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
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(120) NOT NULL,"
        "   id_cargo UNSIGNED TINYINT NOT NULL,"
        "   email VARCHAR(254) NOT NULL,"
        "   telefone VARCHAR(11) NOT NULL,"
        "   cpf_cnpj VARCHAR(14) NOT NULL UNIQUE"
        ");"
        "INSERT INTO colaboradores (nome, id_cargo, email, telefone, cpf_cnpj) VALUES( 'uaidhwuiawd', '1', 'aiuwdh@iufahw', '19785198247', '11111111111');"
        "create table companhias ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(254) NOT NULL UNIQUE"
        ");"
        "create table origens ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(254) NOT NULL UNIQUE,"
        "   id_companhia INTEGER NOT NULL,"
        "   FOREIGN KEY (id_companhia)"
        "       REFERENCES companhias (id)"
        ");"
        "CREATE TABLE clientes ("
        "   id INTEGER PRIMARY KEY,"
        "   nome VARCHAR(120) NOT NULL,"
        "   email VARCHAR(254) NOT NULL,"
        "   id_proprietario INTEGER NOT NULL,"
        "   cpf_cnpj VARCHAR(14) NOT NULL,"
        "   telefone VARCHAR(11) NOT NULL,"
        "   cep CHAR(8) NOT NULL,"
        "   endereco VARCHAR(254) NOT NULL,"
        "   numero INTEGER NOT NULL,"
        "   complemento VARCHAR(120),"
        "   id_origem INTEGER NOT NULL,"
        "   data_origem DATE,"
        "   extra_nome VARCHAR(120),"
        "   extra_cpf CHAR(11),"
        "   extra_rg VARCHAR(13),"
        "   extra_nacionalidade VARCHAR(20),"
        "   extra_profissao VARCHAR(120),"
        "   extra_renda DOUBLE,"
        "   observacoes VARCHAR(700),"
        "   FOREIGN KEY (id_proprietario)"
        "       REFERENCES colaboradores (id),"
        "   FOREIGN KEY (id_origem)"
        "       REFERENCES origens (id)"
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

Napi::Boolean server::setupDatabaseWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env(); 
    if (!setupDatabase())
        Napi::Error::New(env, "Não foi possível criar o banco de dados. Erro: ").ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, true);
}

#pragma endregion Origens

#pragma endregion Wrapped Methods

Napi::Object server::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("setupDatabase", Napi::Function::New(env, server::setupDatabaseWrapped));

    exports.Set("getAllClientes", Napi::Function::New(env, clientes::get_all));
    exports.Set("getCliente", Napi::Function::New(env, clientes::get));
    exports.Set("createCliente", Napi::Function::New(env, clientes::create));
    exports.Set("updateCliente", Napi::Function::New(env, clientes::update));
    exports.Set("deleteCliente", Napi::Function::New(env, clientes::remove));

    exports.Set("getAllColaboradores", Napi::Function::New(env, colaboradores::get_all));
    exports.Set("getColaborador", Napi::Function::New(env, colaboradores::get));
    exports.Set("createColaborador", Napi::Function::New(env, colaboradores::create));
    exports.Set("updateColaborador", Napi::Function::New(env, colaboradores::update));
    exports.Set("deleteColaborador", Napi::Function::New(env, colaboradores::remove));

    exports.Set("getAllCompanhias", Napi::Function::New(env, companhias::get_all));
    exports.Set("getCompanhia", Napi::Function::New(env, companhias::get));
    exports.Set("createCompanhia", Napi::Function::New(env, companhias::create));
    exports.Set("updateCompanhia", Napi::Function::New(env, companhias::update));
    exports.Set("deleteCompanhia", Napi::Function::New(env, companhias::remove));

    exports.Set("getAllOrigens", Napi::Function::New(env, origens::get_all));
    exports.Set("getOrigem", Napi::Function::New(env, origens::get));
    exports.Set("createOrigem", Napi::Function::New(env, origens::create));
    exports.Set("deleteOrigem", Napi::Function::New(env, origens::remove));
    return exports;
}