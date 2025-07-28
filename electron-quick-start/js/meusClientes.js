allRows = (data) => {
    $('.table-container').html(`       
        <input id="telefone-mask" hidden />
        <div class="table-row col-5-1fr table-header"> 
            <div>Nome</div>
            <div>Telefone</div>
            <div>Proprietario</div>
            <div>E-mail</div>
            <div>Origem</div>
            <div class="center">Ações</div>
        </div>
        ${data.map((cliente) => {
            return `<div class="table-row col-5-1fr clickable-row" onclick="openView(event, ${cliente.id})">
                <div>${cliente.nome}</div>
                <div>${$('#telefone-mask').masked(cliente.telefone)}</div>
                <div>${cliente.proprietario.nome}</div>
                <div>${cliente.email}</div>
                <div>${cliente.origem.nome}</div>
                <div class="actions">
                    <div class="table-action edit" onclick="editRow(event)"><i class="fa-solid fa-pen-to-square"></i></div>
                    <div class="table-action delete" onclick="deleteRow(${cliente.id})"><i class="fa-solid fa-trash-can"></i></div>
                </div>
                <div style="display: none">${cliente.id}</div>
            </div>`;
        }).join('')}
    `);
    $('#telefone-mask').mask(telephoneMasks[1]);
}

invokeWorker("getAllClientesShort", null, allRows);

editRow = (e) => {
    getRow = (data) => {
        alterObj = data[0];
        alterObj.extra_renda = formatMoney(alterObj.extra_renda);
    };
    invokeWorker("getCliente", parseInt(e.currentTarget.parentElement.parentElement.children[6].textContent), getRow);
    openModal('Editar Cliente', 'criarCliente');
}

deleteRow = (id) => {
    allRows = (data) => {
        $('.table-container').html(`       
            <input id="telefone-mask" hidden />
            <div class="table-row col-5-1fr table-header"> 
                <div>Nome</div>
                <div>Telefone</div>
                <div>Proprietario</div>
                <div>E-mail</div>
                <div>Origem</div>
                <div class="center">Ações</div>
            </div>
            ${data.map((cliente) => {
                return `<div class="table-row col-5-1fr clickable-row" onclick="openView(event, ${cliente.id})">
                    <div>${cliente.nome}</div>
                    <div>${$('#telefone-mask').masked(cliente.telefone)}</div>
                    <div>${cliente.proprietario.nome}</div>
                    <div>${cliente.email}</div>
                    <div>${cliente.origem.nome}</div>
                    <div class="actions">
                        <div class="table-action edit" onclick="editRow(event)"><i class="fa-solid fa-pen-to-square"></i></div>
                        <div class="table-action delete" onclick="deleteRow(${cliente.id})"><i class="fa-solid fa-trash-can"></i></div>
                    </div>
                    <div style="display: none">${cliente.id}</div>
                </div>`;
            }).join('')}
        `);
        $('#telefone-mask').mask(telephoneMasks[1]);
    }
    
    invokeWorker(`deleteCliente`, parseInt(id));
    invokeWorker("getAllClientesShort", null, allRows);
}

openView = (e, id) => {
    if (e.target.localName === 'path' && e.target.localName === 'svg' && e.target.classList[0] === 'table-action')
        return;

    getRow = (data) => {
        alterObj = data[0];
        openModal(`Cliente - ${alterObj.nome}`, 'verCliente', true);
    };
    invokeWorker("getCliente", parseInt(id), getRow);
}

$('#telefone-mask').mask(telephoneMasks[1]);
