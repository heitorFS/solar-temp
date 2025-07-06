const {
    setupDatabase,
    createColaborador, getAllColaboradores, updateColaborador, deleteColaborador,
    createOrigem, getAllOrigens, deleteOrigem } = require('./build/Release/cppserver.node');

onmessage = (e) => {
    try {
        switch (e.data[0]) {
            case 'setup':
                setupDatabase();
                postMessage(["Conexão com banco de dados bem sucedida.", e.data[0], "info"]);
                break;
            case 'createColaborador':
                createColaborador(e.data[1]["nome"], e.data[1]["id_cargo"], e.data[1]["email"], e.data[1]["telefone"], e.data[1]["cpf_cnpj"]);
                postMessage(["Colaborador criado com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllColaboradores':
                var ret = getAllColaboradores();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateColaborador':
                updateColaborador(e.data[1]["id"], e.data[1]["nome"], e.data[1]["id_cargo"], e.data[1]["email"], e.data[1]["telefone"], e.data[1]["cpf_cnpj"]);
                postMessage(["Colaborador atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteColaborador':
                deleteColaborador(e.data[1]);
                postMessage(["Colaborador removido com sucesso", e.data[0], "success"]);
                break;
            case 'createOrigem':
                createOrigem(e.data[1]);
                postMessage(["Origem criada com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllOrigens':
                var ret = getAllOrigens();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'deleteOrigem':
                deleteOrigem(e.data[1]);
                postMessage(["Origem removida com sucesso", e.data[0], "form_success"])
                break;
        }
    }
    catch (err) {
        postMessage([err, e.data[0], 'error']);
    }
}