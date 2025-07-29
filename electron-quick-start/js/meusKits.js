allRows = (data) => {
    $('.table-cards-container').html(`       
        ${data.map((kit) => {
            return `<div class="table-card" onclick="openView(event, ${kit.id})">
                <div id="id" style="display:none">${kit.id}</div>
                <div class="table-card-icon-container">
                    <div class="table-card-icon"><i class="fa-solid fa-solar-panel"></i></div>
                </div>
                <div class="table-card-title">
                    <div>${kit.fornecedor}</div>
                </div>
                <div class="table-card-info">
                    <div>${formatWp(kit.short_return.soma_potencia)} kWp</div>
                    <div>R$ ${kit.valor.toString().replace('.', ',')}</div>
                    <div>R$ ${kit.short_return.valor_kw.toFixed(2).replace('.', ',')} / kWp</div>
                    <div>${getTensao(kit.tensao)} | ${getFixacao(kit.fixacao)}</div>
                </div>
                <div class="table-card-actions" onclick="openActionsMenu(event)"><i class="fa-solid fa-ellipsis-vertical"></i></div>
            </div>`;
        }).join('')} 
    `);
};

invokeWorker('getAllKits', null, allRows);

openView = (e, id) => {
    if (e.target.parentElement.parentElement.className === 'table-card-actions' || e.target.parentElement.className === 'table-card-actions' || e.target.className === 'table-card-actions')
        return;
    
    getRow = (data) => {
        alterObj = data[0];
        openModal(`Kit - ${alterObj.fornecedor}`, 'verKit', true);
    };
    invokeWorker("getKit", id, getRow);
}

$(document).click((event) => {
    let menu = $('.table-card-actions-menu');    
    if (event.target.parentElement.className !== 'table-card-actions' &&
        event.target.parentElement.parentElement.className !== 'table-card-actions' &&
        event.target.className !== 'table-card-actions' &&
        menu.css('display') === 'block')
        menu.css('display', 'none');
});

$('#kit-edit').click((event) => {
    let id = parseInt(event.currentTarget.parentElement.parentElement.children[0].textContent);
    invokeWorker("getKit", id, (data) => {
        alterObj = data[0];

        alterObj['mod_remove'] = [];
        alterObj['inv_remove'] = [];
        alterObj['item_remove'] = [];

        alterObj['mod_change'] = [];
        alterObj['inv_change'] = [];
        alterObj['item_change'] = [];

        openModal(`Kit`, 'criarKit', false);
    });
    
});

$('#kit-delete').click((event) => {
    let id = parseInt(event.currentTarget.parentElement.parentElement.children[0].textContent);
    invokeWorker("deleteKit", id);
    invokeWorker("deleteInversoresByKit", id);
    invokeWorker("deleteItensByKit", id);
    invokeWorker("deleteModulosByKit", id);
    invokeWorker("getAllKits", null, allRows)
});
