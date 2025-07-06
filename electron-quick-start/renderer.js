// region Variable declarations

let popupCount = 0;

let formCallback;
let alterObj = null;

let formButton;
let addRow;
let allRows;
let editRow;
let deleteRow;
let cpfcnpjSwitch;

const telephoneMasks = ['(00) 0000-0000Z', '(00) 00000-0000'];
let telephoneOptions;

const openModal = (title, id) => {
    $('.modal-title').text(title)
    $('.modal-content').load(`pages/${id}.html`);
    $('.modal-container').css('display', 'flex');
};

const openConfig = (title, id) => {
    switch (id) {
        case 'Origens':
            addRow = () => {
                formCallback = (_, stat) => {
                    if (stat == 'form_success')
                        $('#nome')[0].value = "";
                    else {
                        showPopup("Origem já existente", "error");
                    }
                }

                invokeWorker(`createOrigem`, $('#nome')[0].value, formCallback);
                invokeWorker(`getAllOrigens`, null, allRows);
            };

            allRows = (data) => {
                $('.table-container').html(`
                    <div class="table-row col-1-1fr table-header"> 
                        <div>Nome</div>
                        <div class="center">Ações</div>
                    </div>
                    ${data.map((origem) => {
                        return `<div class="table-row col-1-1fr">
                            <div>${origem.nome}</div>
                            <div class="actions center">
                                <div class="table-action delete" onclick="deleteRow(${origem.id})"><i class="fa-solid fa-trash-can"></i></div>
                            </div>
                        </div>`;
                    }).join('')}
                    <form id="form">
                        <div class="table-row col-1-1fr">
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
                invokeWorker(`deleteOrigem`, parseInt(id), formCallback);
                invokeWorker(`getAllOrigens`, null, allRows);
            }
            break;
    }

    openModal(title, id);
    invokeWorker(`getAll${id}`, null, allRows);
}

// endregion Variable declarations

// region Function declaration

const getCargo = (id) => {
    switch (id) {
        case 1:
            return 'Vendedor';
        case 2:
            return 'Projetista';
        case 3:
            return 'Instalador';
        case 4:
            return 'Financeiro';
        case 5:
            return 'Administrativo';
    }
};

const getCargoId = (cargo) => {
    switch (cargo) {
        case 'Vendedor':
            return 1;
        case 'Projetista':
            return 2;
        case 'Instalador':
            return 3;
        case 'Financeiro':
            return 4;
        case 'Administrativo':
            return 5;
    }
}

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
}

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
        default:
            showPopup(event.data[0], event.data[2]);
            break;
    }
}

function invokeWorker(operation, data, _callback = null) {
    if (!!_callback)
        callback[operation] = _callback;

    worker.postMessage([operation, data]);
}

worker.postMessage(['setup']);

//endregion Worker

