$('#cpf-mask').mask('000.000.000-00');
$('#cnpj-mask').mask('00.000.000/0000-00');
$('#telefone-mask').mask(telephoneMasks[1]);
$('#cep-mask').mask('00000-000');
$('#extra_renda-mask').mask("#.##0,00", {reverse: true});

$('#nome').html(alterObj.nome);
$('#email').html(alterObj.email);
$('#proprietario').html(alterObj.proprietario.nome);
$('#cpf_cnpj-label').html(alterObj.cpf_cnpj.length == 11 ? 'CPF' : 'CNPJ');
$('#cpf_cnpj').html(alterObj.cpf_cnpj.length == 11 ? $('#cpf-mask').masked(alterObj.cpf_cnpj) : $('#cnpj-mask').masked(alterObj.cpf_cnpj));
$('#telefone').html($('#telefone-mask').masked(alterObj.telefone));
$('#cep').html($('#cep-mask').masked(alterObj.cep));
$('#endereco').html(alterObj.endereco);
$('#numero').html(alterObj.numero);
$('#complemento').html(alterObj.complemento.length == 0 ? '-' : alterObj.complemento);
$('#origem').html(alterObj.origem.nome);
$('#data_origem-label').html(alterObj.cpf_cnpj.length == 11 ? 'Data de Nascimento' : 'Data de Fundação');
$('#data_origem').html(formatDate(alterObj.data_origem));
$('#extra_rg').html(alterObj.extra_rg.length == 0 ? '-' : alterObj.extra_rg);
$('#extra_nacionalidade').html(alterObj.extra_nacionalidade.length == 0 ? '-' : alterObj.extra_nacionalidade);
$('#extra_profissao').html(alterObj.extra_profissao.length == 0 ? '-' : alterObj.extra_profissao);
$('#extra_renda').html($('#extra_renda-mask').masked(formatMoney(alterObj.extra_renda)));
$('#observacoes').html(alterObj.observacoes.length == 0 ? '-' : alterObj.observacoes);

if (alterObj.cpf_cnpj.length == 11) {
    $('#extra_nome-label').css('display', 'none');
    $('#extra_nome').css('display', 'none');
    $('#extra_cpf-label').css('display', 'none');
    $('#extra_cpf').css('display', 'none');
}
else {
    $('#extra_nome').html(alterObj.extra_nome);
    $('#extra_cpf').html($('#cpf-mask').masked(alterObj.extra_cpf));
    $('#extra_rg-label').html('RG do Responsável');
    $('#extra_nacionalidade-label').html('Nacionalidade do Responsável');
    $('#extra_profissao-label').html('Profissão do Responsável');
    $('#extra_renda-label').html('Faturamento Mensal');
    $('#extra_rg-label').html('RG do Responsável');
}

switchTab = (tab) => {
    switch (tab) {
        case 'propostas':
            break;
        case 'procuracao':
            formButton = (e) => {
                e.preventDefault();
                $('#loader-container').css('display', 'flex');

                invokeWorker('getCompanhia', alterObj.origem.id_companhia, (data) => {
                    generateProcuracao(alterObj.id, alterObj.nome, $('#cpf-mask').masked(alterObj.cpf_cnpj), $('#cep-mask').masked(alterObj.cep),
                        alterObj.endereco, alterObj.numero, data[0].nome, e.target.codigo.value);
                });
            }

            $('.view-content').html(`
                <div class="procuracao-container">
                    <form id="form">
                        <div class="col-1fr-1fr">                        
                            <div class="field view-field">
                                <label>Código do cliente</label>
                                <input id="codigo" type="text" required />
                            </div>
                            <div class="field view-field">
                                <button class="submit view-button" type="submit">Gerar procuração</button>
                            </div>
                        </div>
                    </form>
                    <script>$('#form').submit(formButton)</script>
                </div>    
            `);
            break;
        case 'arquivos':
            allFiles = (files) => {     
                $('.arquivos-container').html(
                    files.map((file) => {
                        return `
                            <div class="arquivos-container" onclick="openFile(event, '${file.path + file.name}')">
                                <div class="arquivo-card">
                                    <div class="arquivo-icon"><i class="fa-solid fa-file-pdf" style="color: #aa3c3c;"></i></div>
                                    <div class="arquivo-info">
                                        <div class="arquivo-title">Procuração</div>
                                        <div class="arquivo-subtitle">${formatData(file.size)}</div>
                                    </div>
                                    <div class="arquivo-extra">
                                        <i class="fa-solid fa-folder open-folder" onclick="openFolder('${file.path + file.name}')"></i>
                                        <div class="file-action trash-container" onclick="deleteFile(event, '${file.path + file.name}')"><i class="fa-solid fa-trash"></i><i class="fa-solid fa-check"></i></div>
                                    </div>
                                </div>
                            </div>`;
                    })
                );
            }

            openFile = (e, address) => {
                if (e.target.localName !== 'path' && e.target.localName !== 'svg' && e.target.classList[0] !== 'file-action')
                    invokeWorker("openFile", address);
            }

            openFolder = (address) => {
                invokeWorker("openFolder", address);
            }

            deleteFile = (e, address) => {
                if (!e.currentTarget.classList.contains('confirm-delete'))
                {
                    let obj = e.currentTarget;
                    obj.classList.add('confirm-delete');
                    setTimeout(() => {
                        obj.classList.remove('confirm-delete');
                    }, 3000);
                }
                else
                    invokeWorker("deleteFile", address, (msg) => {
                        invokeWorker('getProcuracao', alterObj.id, allFiles);
                        showPopup(msg, 'success');
                    });
            }

            invokeWorker('getProcuracao', alterObj.id, allFiles);

            $('.view-content').html(`
                <div class="arquivos-container">
                </div>    
            `);
            break;
    }

    $('#propostas').removeClass('tab-selected');
    $('#procuracao').removeClass('tab-selected');
    $('#arquivos').removeClass('tab-selected');
    
    $(`#${tab}`).addClass('tab-selected');
}
