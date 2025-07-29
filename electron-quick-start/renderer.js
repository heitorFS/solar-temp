// region Variable declarations

let popupCount = 0;

let formCallback;
let alterObj = null;

let formButton;
let addRow;
let allRows;
let getRow;
let editRow;
let deleteRow;
let cpfcnpjSwitch;

let openView;
let switchTab;

let allFiles;
let openFile;
let openFolder;
let deleteFile;

let onChange;

const telephoneMasks = ['(00) 0000-0000Z', '(00) 00000-0000'];
let telephoneOptions;

// endregion Variable declarations

// region Function declaration

const openModal = (title, id, wide = false) => {
    $('.modal-title').text(title)
    $('.modal-content').load(`pages/${id}.html`);
    $('#modal-container').css('display', 'flex');

    if (wide)
        $('.modal').addClass('modal-wide');
    else
        $('.modal').removeClass('modal-wide')
};

const openConfig = (title, id) => {
    let mod = "";
    switch (id) {
        case 'Origens':
            mod = 'simple_table';

            addRow = () => {
                formCallback = (_, stat) => {
                    if (stat == 'form_success')
                        $('#nome')[0].value = "";
                    else {
                        showPopup("Origem já existente", "error");
                    }
                }

                invokeWorker(`createOrigem`, { nome: $('#nome')[0].value, id_companhia: parseInt($('#companhia')[0].value) }, formCallback);
                invokeWorker(`getAllOrigens`, null, allRows);
            };

            allRows = (data) => {
                invokeWorker('getAllCompanhias', null, (extra) => {      
                    debugger;          
                    $('.table-container').html(`
                        <div class="table-row col-1fr-1fr-60px table-header"> 
                            <div>Nome</div>
                            <div>Companhia</div>
                            <div class="center">Ações</div>
                        </div>
                        ${data.map((origem) => {
                            return `<div class="table-row col-1fr-1fr-60px">
                                <div>${origem.nome}</div>
                                <div>${origem.companhia.nome}</div>
                                <div class="actions center">
                                    <div class="table-action delete" onclick="deleteRow(${origem.id})"><i class="fa-solid fa-trash-can"></i></div>
                                </div>
                            </div>`;
                        }).join('')}
                        <form id="form">
                            <div class="table-row col-1fr-1fr-60px">
                                <input class="dynamic-input" id="nome" required/>
                                <select class="dynamic-input" id="companhia" required>${extra.map((companhia) => {
                                    return `<option value="${companhia.id}">${companhia.nome}</option>`
                                })}</select>
                                <div class="actions center">
                                    <button class="empty" type="submit"><div class="table-action add"><i class="fa-solid fa-square-plus"></i></div></button>
                                </div>
                            </div>
                        </form>
                        <script>$('#form').submit((e) => {e.preventDefault(); addRow();})</script>
                    `);
                });
            }
            
            deleteRow = (id) => {
                formCallback = (_, stat) => {}
                invokeWorker(`deleteOrigem`, parseInt(id), formCallback);
                invokeWorker(`getAllOrigens`, null, allRows);
            }
            break;
        case 'Companhias':
            mod = 'simple_table';

            addRow = () => {
                formCallback = (_, stat) => {
                    if (stat == 'form_success')
                        $('#nome')[0].value = "";
                    else {
                        showPopup("Companhia já existente", "error");
                    }
                }

                invokeWorker(`createCompanhia`, $('#nome')[0].value, formCallback);
                invokeWorker(`getAllCompanhias`, null, allRows);
            };

            allRows = (data) => {           
                $('.table-container').html(`
                    <div class="table-row col-1fr-60px table-header"> 
                        <div>Nome</div>
                        <div class="center">Ações</div>
                    </div>
                    ${data.map((origem) => {
                        return `<div class="table-row col-1fr-60px">
                            <div>${origem.nome}</div>
                            <div class="actions center">
                                <div class="table-action delete" onclick="deleteRow(${origem.id})"><i class="fa-solid fa-trash-can"></i></div>
                            </div>
                        </div>`;
                    }).join('')}
                    <form id="form">
                        <div class="table-row col-1fr-60px">
                            <input class="dynamic-input" id="nome" required/>
                            <div class="actions center">
                                <button class="empty" type="submit"><div class="table-action add"><i class="fa-solid fa-square-plus"></i></div></button>
                            </div>
                        </div>
                    </form>
                    <script>$('#form').submit((e) => {e.preventDefault(); addRow();})</script>
                `);
            }
            
            deleteRow = (id) => {
                formCallback = (_, stat) => {}
                invokeWorker(`deleteCompanhia`, parseInt(id), formCallback);
                invokeWorker(`getAllCompanhias`, null, allRows);
            }

            break;
    }

    openModal(title, mod);
    invokeWorker(`getAll${id}`, null, allRows);
};

const formatDate = (date) => {
    let arr = date.split('-');
    return `${arr[2]}/${arr[1]}/${arr[0]}`;
};

const formatMoney = (money) => {
    if (money % 1 === 0)
        return money + ',00';
    else if ((money * 10) % 1 === 0)
        return money + '0';
    else
        return money;
};

const formatData = (data) => {
    let i = 0;
    while (data / 1024 > 1) {
        data /= 1024;
        i++;
    }

    switch (i) {
        case 0:
            return `${Math.floor(data)}B`;
        case 1:
            return `${Math.floor(data)}KB`;
        case 2:
            return `${Math.floor(data)}MB`;
        case 3:
            return `${Math.floor(data)}GB`;
        default:
            return 'TOO_BIG';
    }
};

const formatWp = (wp) => {
    return ((Math.round(wp / 10) / 100).toFixed(2)).replace('.', ',');
};

const getCargo = (id) => {
    switch (id) {
        case 0:
            return 'Vendedor';
        case 1:
            return 'Projetista';
        case 2:
            return 'Instalador';
        case 3:
            return 'Financeiro';
        case 4:
            return 'Administrativo';
    }
};

const getCargoId = (cargo) => {
    switch (cargo) {
        case 'Vendedor':
            return 0;
        case 'Projetista':
            return 1;
        case 'Instalador':
            return 2;
        case 'Financeiro':
            return 3;
        case 'Administrativo':
            return 4;
    }
};

const getTensao = (tensao) => {
    switch (tensao) {
        case 0:
            return '127V';
        case 1:
            return '220V';
        case 2:
            return '380V';
    }
};

const getFixacao = (fixacao) => {
    switch (fixacao) {
        case 0:
            return 'Cerâmica';
        case 1:
            return 'Metálica Trapezoidal';
        case 2:
            return 'Metálica Ondulada';
        case 3:
            return 'Metálica 55cm';
        case 4:
            return 'Fibrocimento Est. Madeira';
        case 5:
            return 'Fibrocimento Est. Metálica';
        case 6:
            return 'Laje';
        case 7:
            return 'Solo';
        case 8:
            return 'Mini Trilho';
        case 9:
            return 'Kalhetão';
        case 10:
            return 'Americano';
        case 11:
            return 'Carport';
        case 12:
            return 'Zipado';
        case 13:
            return 'Sem Estrutura';
    }
};

const getMonitoramento = (monitoramento) => {
    switch (monitoramento) {
        case 0:
            return 'Wi-Fi';
        case 1:
            return '4G';
        case 2:
            return 'Sem Monitoramento';
    }
};

const getPopup = (text, type) => {    
    let popup = `<div class="popup popup-${type}" data-id="${popupCount}">
        <div class="popup-close" onclick="closePopup(event)"><i class="fa-solid fa-xmark"></i></div>
        <div class="popup-text">
            ${text}
        </div>
        <div class="popup-feedback">
            <i class="fa-solid fa-circle-${type == 'error' ? 'exclamation' : type == 'info' ? 'info' : 'check'}"></i>
        </div>
        </div>
    </div>`;

    return popup;
};

const showPopup = (text, type) => {
    let popups = $('.popups');
    popups.html(popups.html() + getPopup(text, type));
    let now = popupCount;
    setTimeout(() => {
        popups.children(`div[data-id="${now}"]`).remove();
    }, 5000);
    popupCount++;
};

const closePopup = (e) => {
    e.currentTarget.parentElement.remove();
};

const getMasked = (type, val) => {
    switch (type) {
        case 'telefone':
            var ret = `(${val[0] + val[1]}) `;
            var len = val.length == 11 ? 7 : 6;
            for (var i = 2; i < len; i++) {
                ret += val[i];
            }
            ret += '-';
            for (var i = val.length - 4; i < val.length; i++) {
                ret += val[i];
            }
            return ret;
        case 'cpfcnpj':
            var ret = "";
            if (val.length == 11) {
                for (var i = 0; i < 9; i++) {
                    if (i % 3 == 0 && i != 0)
                        ret += '.';
                    ret += val[j];
                }
                ret += `-${val[9] + val[10]}`;
            }
            else {
                
                for (var i = 0; i < 9; i++) {
                    if (i % 3 == 0 && i != 0)
                        ret += '.';
                    ret += val[j];
                }
            }
            return ret; 
    }
};

const openActionsMenu = (event) => {
    let menu = $('.table-card-actions-menu');
    menu.css('display', 'block');
    menu.css('top', event.pageY);
    menu.css('left', event.pageX);
    menu[0].children[0].textContent = event.currentTarget.parentElement.children[0].textContent;
};

// endregion Function declaration

// region Worker

let worker = new Worker('./worker.js');
let callback = {};

worker.onmessage = function(event) {    
    switch (event.data[2])
    {
        case 'json':
            callback[event.data[1]](JSON.parse(event.data[0]));
            break;
        case 'error':
            debugger;
            callback[event.data[1]](JSON.parse(event.data[0].message), 'error');
            break;
        case 'form_success':
            callback[event.data[1]](null, 'form_success');
            showPopup(event.data[0], 'success');
            break;
        case 'create_id':
            callback[event.data[1]](event.data[0]);
            break;
        case 'files':
            callback[event.data[1]](event.data[0]);
            break;
        case 'void':
            callback[event.data[1]]();
            break;
        case 'none':
            return;
        default:
            showPopup(event.data[0], event.data[2]);
            return;
    }
    delete callback[event.data[1]];
}

function invokeWorker(operation, data, _callback = null) {
    if (!!_callback)
        callback[operation] = _callback;
    worker.postMessage([operation, data]);
}

worker.postMessage(['setup']);

//endregion Worker

