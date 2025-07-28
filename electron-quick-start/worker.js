const {
    setupDatabase,
    createCliente, getAllClientes, getAllClientesShort, getCliente, updateCliente, deleteCliente,
    createColaborador, getColaborador, getAllColaboradores, updateColaborador, deleteColaborador,
    createCompanhia, getCompanhia, getAllCompanhias, updateCompanhia, deleteCompanhia,
    createOrigem, getOrigem, getAllOrigens, deleteOrigem } = require('./build/Release/cppserver.node');

const htopdf = require('html-pdf-node');
const fs = require('fs');
const { shell } = require('electron');

onmessage = (e) => {
    try {
        switch (e.data[0]) {
            // region Setup
            case 'setup':
                if (!fs.existsSync('C:/AEnAzume'))
                    fs.mkdirSync('C:/AEnAzume');
                setupDatabase();
                postMessage(["Conexão com banco de dados bem sucedida.", e.data[0], "info"]);
                break;                
            // endregion Setup

            // region Arquivos
            case 'openFolder':
                shell.showItemInFolder(e.data[1]);
                break;
            case 'openFile':
                shell.openPath(e.data[1]);
                break;
            case 'deleteFile':
                fs.unlink(e.data[1], (err) => {
                    if (!!err) throw err;
                    postMessage(["Arquivo removido com sucesso", e.data[0], "files"]);
                });
                break;
            // endregion Arquivos

            // region Cliente
            case 'createCliente':
                createCliente(e.data[1]["nome"], e.data[1]["email"], e.data[1]["proprietario"], e.data[1]["cpf_cnpj"], e.data[1]["telefone"], e.data[1]["cep"], e.data[1]["endereco"], e.data[1]["numero"], e.data[1]["complemento"], e.data[1]["origem"], e.data[1]["data_origem"], e.data[1]["extra_nome"], e.data[1]["extra_cpf"], e.data[1]["extra_rg"], e.data[1]["extra_nacionalidade"], e.data[1]["extra_profissao"], e.data[1]["extra_renda"], e.data[1]["observacoes"]);
                postMessage(["Cliente criado com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllClientesShort':
                var ret = getAllClientesShort();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getAllClientes':
                var ret = getAllClientes();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getCliente':
                var ret = getCliente(e.data[1]);
                postMessage([ret, e.data[0], "json"])
                break;
            case 'updateCliente':
                updateCliente(e.data[1]["id"], e.data[1]["nome"], e.data[1]["email"], e.data[1]["proprietario"], e.data[1]["cpf_cnpj"], e.data[1]["telefone"], e.data[1]["cep"], e.data[1]["endereco"], e.data[1]["numero"], e.data[1]["complemento"], e.data[1]["origem"], e.data[1]["data_origem"], e.data[1]["extra_nome"], e.data[1]["extra_cpf"], e.data[1]["extra_rg"], e.data[1]["extra_nacionalidade"], e.data[1]["extra_profissao"], e.data[1]["extra_renda"], e.data[1]["observacoes"]);
                postMessage(["Cliente atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteCliente':
                deleteCliente(e.data[1]);
                postMessage(["Cliente removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Cliente

            // region Colaborador
            case 'createColaborador':
                createColaborador(e.data[1]["nome"], e.data[1]["id_cargo"], e.data[1]["email"], e.data[1]["telefone"], e.data[1]["cpf_cnpj"]);
                postMessage(["Colaborador criado com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllColaboradores':
                var ret = getAllColaboradores();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getColaborador':
                var ret = getColaborador(e.data[1]);
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
            // endregion Colaborador

            // region Companhia
            case 'createCompanhia':
                createCompanhia(e.data[1]);
                postMessage(["Companhia criado com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllCompanhias':
                var ret = getAllCompanhias();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getCompanhia':
                var ret = getCompanhia(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateCompanhia':
                updateCompanhia(e.data[1]["id"], e.data[1]["nome"]);
                postMessage(["Companhia atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteCompanhia':
                deleteCompanhia(e.data[1]);
                postMessage(["Companhia removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Companhia

            // region Origem
            case 'createOrigem':
                createOrigem(e.data[1]["nome"], e.data[1]["id_companhia"]);
                postMessage(["Origem criada com sucesso", e.data[0], "form_success"]);
                break;
            case 'getAllOrigens':
                var ret = getAllOrigens();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getOrigem':
                var ret = getOrigem(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'deleteOrigem':
                deleteOrigem(e.data[1]);
                postMessage(["Origem removida com sucesso", e.data[0], "form_success"])
                break;
            // endregion Origem

            // region Procuracao
            case 'generateProcuracao':
                htopdf.generatePdf({content: e.data[1].content}, {format: 'A4'})
                    .then((pdf_buffer) => {
                        if (!fs.existsSync('C:/AEnAzume/Procuracoes'))
                            fs.mkdirSync('C:/AEnAzume/Procuracoes');

                        fs.writeFileSync(`C:/AEnAzume/Procuracoes/${e.data[1].nome}.pdf`, pdf_buffer, 'binary');

                        postMessage([pdf_buffer, e.data[0], "void"]);
                    });
                break;
            case 'getProcuracao':
                fs.readdir('C:/AEnAzume/Procuracoes', (err, files) => {
                    let ret = [];
                    for (const file of files) {
                        if (file.split('_')[0] == e.data[1]) {
                            ret.push({path: 'C:/AEnAzume/Procuracoes/', name: file,
                                size: fs.statSync(`C:/AEnAzume/Procuracoes/${file}`).size});
                        }
                    }
                    postMessage([ret, e.data[0], "files"]);
                })
                break;
            // endregion Procuracao
        }
    }
    catch (err) {
        postMessage([err, e.data[0], 'error']);
    }
}