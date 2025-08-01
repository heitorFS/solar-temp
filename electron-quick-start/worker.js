const {
    setupDatabase,
    createCliente, getAllClientes, getAllClientesShort, getCliente, updateCliente, deleteCliente,
    createColaborador, getColaborador, getAllColaboradores, updateColaborador, deleteColaborador,
    createCompanhia, getCompanhia, getAllCompanhias, updateCompanhia, deleteCompanhia,
    createInversor, getInversor, getInversoresByKit, getAllInversores, updateInversor, deleteInversor, deleteInversoresByKit,
    createItem, getItem, getItensByKit, getAllItens, updateItem, deleteItem, deleteItensByKit,
    createKit, getKit, getAllKits, updateKit, deleteKit,
    createModulo, getModulo, getModulosByKit, getAllModulos, updateModulo, deleteModulo, deleteModulosByKit,
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
                createCliente(JSON.stringify(e.data[1]));
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
                updateCliente(JSON.stringify(e.data[1]));
                postMessage(["Cliente atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteCliente':
                deleteCliente(e.data[1]);
                postMessage(["Cliente removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Cliente

            // region Colaborador
            case 'createColaborador':
                createColaborador(JSON.stringify(e.data[1]));
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
                updateColaborador(JSON.stringify(e.data[1]));
                postMessage(["Colaborador atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteColaborador':
                deleteColaborador(e.data[1]);
                postMessage(["Colaborador removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Colaborador

            // region Inversor
            case 'createInversor':
                createInversor(JSON.stringify(e.data[1]));
                postMessage(["Inversor criado com sucesso", e.data[0], "void"]);
                break;
            case 'getInversoresByKit':
                getInversoresByKit(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getAllInversores':
                var ret = getAllInversores();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getInversor':
                var ret = getInversor(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateInversor':
                updateInversor(JSON.stringify(e.data[1]));
                postMessage(["Inversor atualizado com sucesso", e.data[0], "void"]);
                break;
            case 'deleteInversor':
                deleteInversor(e.data[1]);
                postMessage(["Inversor removido com sucesso", e.data[0], "success"]);
                break;
            case 'deleteInversoresByKit':
                deleteInversoresByKit(e.data[1]);
                postMessage(["Inversor removido com sucesso", e.data[0], "none"]);
                break;
            // endregion Inversor

            // region Item
            case 'createItem':
                createItem(JSON.stringify(e.data[1]));
                postMessage(["Item criado com sucesso", e.data[0], "void"]);
                break;
            case 'getItensByKit':
                getItensByKit(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getAllItens':
                var ret = getAllItens();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getItem':
                var ret = getItem(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateItem':
                updateItem(JSON.stringify(e.data[1]));
                postMessage(["Item atualizado com sucesso", e.data[0], "void"]);
                break;
            case 'deleteItem':
                deleteItem(e.data[1]);
                postMessage(["Item removido com sucesso", e.data[0], "success"]);
                break;
            case 'deleteItensByKit':
                deleteItensByKit(e.data[1]);
                postMessage(["Item removido com sucesso", e.data[0], "none"]);
                break;
            // endregion Item

            // region Companhia
            case 'createCompanhia':
                createCompanhia(JSON.stringify(e.data[1]));
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
                updateCompanhia(JSON.stringify(e.data[1]));
                postMessage(["Companhia atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteCompanhia':
                deleteCompanhia(e.data[1]);
                postMessage(["Companhia removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Companhia

            // region Kit
            case 'createKit':
                var id = createKit(JSON.stringify(e.data[1]));
                postMessage([id, e.data[0], "create_id"]);
                break;
            case 'getAllKits':
                var ret = getAllKits();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getKit':
                var ret = getKit(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateKit':
                updateKit(JSON.stringify(e.data[1]));
                postMessage(["Kit atualizado com sucesso", e.data[0], "form_success"]);
                break;
            case 'deleteKit':
                deleteKit(e.data[1]);
                postMessage(["Kit removido com sucesso", e.data[0], "success"]);
                break;
            // endregion Kit

            // region Modulo
            case 'createModulo':
                createModulo(JSON.stringify(e.data[1]));
                postMessage(["Modulo criado com sucesso", e.data[0], "void"]);
                break;
            case 'getModulosByKit':
                getModulosByKit(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getAllModulos':
                var ret = getAllModulos();
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'getModulo':
                var ret = getModulo(e.data[1]);
                postMessage([ret, e.data[0], "json"]);
                break;
            case 'updateModulo':
                updateModulo(JSON.stringify(e.data[1]));
                postMessage(["Modulo atualizado com sucesso", e.data[0], "void"]);
                break;
            case 'deleteModulo':
                deleteModulo(e.data[1]);
                postMessage(["Modulo removido com sucesso", e.data[0], "success"]);
                break;
            case 'deleteModulosByKit':
                deleteModulosByKit(e.data[1]);
                postMessage(["Modulo removido com sucesso", e.data[0], "none"]);
                break;
            // endregion Modulo

            // region Origem
            case 'createOrigem':
                createOrigem(JSON.stringify(e.data[1]));
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