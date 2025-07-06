allRows = (data) => {
    $('.table-container').html(`       
        <input id="telefone-mask" hidden />
        <input id="cpf-mask" hidden />
        <input id="cnpj-mask" hidden />
        <div class="table-row col-5-1fr table-header"> 
            <div>Nome</div>
            <div>Cargo</div>
            <div>Email</div>
            <div>Telefone</div>
            <div>CPF/CNPJ</div>
            <div class="center">Ações</div>
        </div>
        ${data.map((colaborador) => {
            return `<div class="table-row col-5-1fr">
                <div>${colaborador.nome}</div>
                <div>${getCargo(colaborador.id_cargo)}</div>
                <div>${colaborador.email}</div>
                <div>${$('#telefone-mask').masked(colaborador.telefone)}</div>
                <div>${colaborador.cpf_cnpj.length == 11 ? $('#cpf-mask').masked(colaborador.cpf_cnpj) : $('#cnpj-mask').masked(colaborador.cpf_cnpj)}</div>
                <div class="actions">
                    <div class="table-action edit" onclick="editRow(event)"><i class="fa-solid fa-pen-to-square"></i></div>
                    <div class="table-action delete" onclick="deleteRow(${colaborador.id})"><i class="fa-solid fa-trash-can"></i></div>
                </div>
                <div style="display: none">${colaborador.id}</div>
            </div>`;
        }).join('')}
    `);
    $('#cpf-mask').mask('000.000.000-00');
    $('#cnpj-mask').mask('00.000.000/0000-00');
    $('#telefone-mask').mask(telephoneMasks[1]);
}

invokeWorker("getAllColaboradores", null, allRows);

editRow = (e) => {
    alterObj = {
        id: e.currentTarget.parentElement.parentElement.children[6].textContent,
        nome: e.currentTarget.parentElement.parentElement.children[0].textContent,
        id_cargo: getCargoId(e.currentTarget.parentElement.parentElement.children[1].textContent),
        email: e.currentTarget.parentElement.parentElement.children[2].textContent,
        telefone: e.currentTarget.parentElement.parentElement.children[3].textContent.replace(/\D/g, ""),
        cpf_cnpj: e.currentTarget.parentElement.parentElement.children[4].textContent
    }
    openModal('Editar colaborador', 'criarColaborador');
}

deleteRow = (id) => {
    invokeWorker(`deleteColaborador`, parseInt(id));
    invokeWorker("getAllColaboradores", null, allRows);
}

$('#cpf-mask').mask('000.000.000-00');
$('#cnpj-mask').mask('00.000.000/0000-00');
$('#telefone-mask').mask(telephoneMasks[1]);

