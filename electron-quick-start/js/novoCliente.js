$('.cpfcnpj-switch').change(function () {
    let ret = [];
    if (this.checked) {
        ret[0] = "CNPJ";
        ret[1] = "00.000.000/0000-00";
        ret[2] = "RG do responsável";
        ret[3] = "Nacionalidade do responsável";
        ret[4] = "Profissão do responsável";
        ret[5] = "Faturamento mensal";
        ret[6] = "Data de abertura";
        $('#extra_properties').css('display', 'grid');
    }
    else {
        ret[0] = "CPF";
        ret[1] = "000.000.000-00"
        ret[2] = "RG";
        ret[3] = "Nacionalidade";
        ret[4] = "Profissão";
        ret[5] = "Renda mensal";
        ret[6] = "Data de nascimento";
        $('#extra_properties').css('display', 'none');
    }
  
    $('.cpfcnpj-label').text(ret[0]);
    $('#cpf_cnpj').mask(ret[1]);
    $('.extra_rg-label').text(ret[2]);
    $('.extra_nacionalidade-label').text(ret[3]);
    $('.extra_profissao-label').text(ret[4]);
    $('.extra_renda-label').text(ret[5]);
    $('.data-label').text(ret[6]);
});
$('#cpf_cnpj').mask('000.000.000-00');

telephoneOptions = {
    translation: {
        'Z': {
            pattern: /[0-9]/, optional: true
        }
    },
    onKeyPress: function (tel, e, field, options) {
        var mask = (tel.length == 15) ? telephoneMasks[1] : telephoneMasks[0];
        $('#telefone').mask(mask, options);
    }
};
$('#telefone').mask(telephoneMasks[1], telephoneOptions);

$('#cep').mask('00000-000');
$('#extra_renda').mask("#.##0,00", {reverse: true});

allRows = (data) => {
    for (const colaborador of data) {
        $('#proprietario').append($('<option>', {
            value: colaborador.id,
            text: `${colaborador.nome} - ${getCargo(colaborador.id_cargo)}`
        }));
    }
};
invokeWorker("getAllColaboradores", null, allRows);

allRows = (data) => {
    for (const origem of data) {
        $('#origem').append($('<option>', {
            value: origem.id,
            text: origem.nome
        }));
    }
};
invokeWorker("getAllOrigens", null, allRows);

$('form').submit((e) => {
    e.preventDefault();
    if (e.target.id.value == 0) {
        formCallback = (data, type) => {
            if (type === 'error') {
                let message = "";
                let errs = 0;
                for (const i in data) {
                    if (data[i].error_type == 1)
                        showPopup(`O ${(data[i].name === 'cpf_cnpj' ? $(`#cpf_cnpj`).cleanVal().length === 11 ? 'CPF' : 'CNPJ' : data[i].name)} inserido já foi utilizado`, 'error');
                    else
                    {
                        message += `${i == 0 ? '' : '; '}${(data[i].name === 'cpf_cnpj' ? $(`#cpf_cnpj`).cleanVal().length === 11 ? 'CPF' : 'CNPJ' : data[i].name)}`;
                        errs++;
                    }
                    $(`#${data[i].name}`).css('border-color', '#F00');
                }
                if (message.length > 0)
                    showPopup(`O${errs > 1 ? 's' : ''} seguinte${errs > 1 ? 's' : ''} campo${errs > 1 ? 's' : ''} ${errs > 1 ? 'estão' : 'está'} incorreto${errs > 1 ? 's' : ''}: ${message}.`, 'error');
            }
            else
                $('form').trigger('reset');
        };
        
        invokeWorker('createCliente', {
            nome: e.target.nome.value,
            email: e.target.email.value,
            proprietario: parseInt(e.target.proprietario.value),
            cpf_cnpj: $('#cpf_cnpj').cleanVal(),
            telefone: $('#telefone').cleanVal(),
            cep: $('#cep').cleanVal(),
            endereco: e.target.endereco.value,
            numero: parseInt(e.target.numero.value),
            complemento: e.target.complemento.value,
            origem: parseInt(e.target.origem.value),
            data_origem: e.target.data_origem.value,
            extra_nome: e.target.extra_nome.value,
            extra_cpf: e.target.extra_cpf.value,
            extra_rg: e.target.extra_rg.value,
            extra_nacionalidade: e.target.extra_nacionalidade.value,
            extra_profissao: e.target.extra_profissao.value,
            extra_renda: parseFloat($('#extra_renda').cleanVal() / 100),
            observacoes: e.target.observacoes.value
        }, formCallback);

        if (!(e.originalEvent.submitter.classList[1] === 'secondary-submit'))
            console.log('dsabdsaaihb');
    }
    else {
        formCallback = (data, type) => {
            if (type === 'error') {
                let message = `O${data.length > 1 ? 's' : ''} seguinte${data.length > 1 ? 's' : ''} campo${data.length > 1 ? 's' : ''} ${data.length > 1 ? 'estão' : 'está'} incorreto${data.length > 1 ? 's' : ''}: `;
                for (const i in data) {
                    message += `${(data[i] === 'cpf_cnpj' ? $(`#cpf_cnpj`).cleanVal().length === 11 ? 'CPF' : 'CNPJ' : data[i])}${i == data.length - 1 ? '' : '; '}`;
                    $(`#${data[i]}`).css('border-color', '#F00');
                }
                showPopup(`${message}.`, 'error');
            }
            else {
                $('form').trigger('reset');
                closeModal();
                invokeWorker("getAllColaboradores", null, allRows);
            }
        };

        invokeWorker('updateCliente', {
            id: parseInt(e.target.id.value),
            nome: e.target.nome.value,
            email: e.target.email.value,
            proprietario: parseInt(e.target.proprietario.value),
            cpf_cnpj: $('#cpf_cnpj').cleanVal(),
            telefone: $('#telefone').cleanVal(),
            cep: $('#cep').cleanVal(),
            endereco: e.target.endereco.value,
            numero: parseInt(e.target.numero.value),
            complemento: e.target.complemento.value,
            origem: parseInt(e.target.origem.value),
            data_origem: e.target.data_origem.value,
            extra_nome: e.target.extra_nome.value,
            extra_cpf: e.target.extra_cpf.value,
            extra_rg: e.target.extra_rg.value,
            extra_nacionalidade: e.target.extra_nacionalidade.value,
            extra_profissao: e.target.extra_profissao.value,
            extra_renda: parseFloat($('#extra_renda').cleanVal() / 100),
            observacoes: e.target.observacoes.value
        }, formCallback);
    }
});

if (!!alterObj) {
    $('#id').attr('value', alterObj.id);
    $('#nome').attr('value', alterObj.nome);
    $('#email').attr('value', alterObj.email);
    $('#proprietario')[0].value = alterObj.proprietario;
    $('#cpf_cnpj').attr('value', alterObj.cpf_cnpj);
    $('#telefone').attr('value', $('#telefone-mask').masked(alterObj.telefone));
    $('#cep').attr('value', alterObj.cep);
    $('#numero').attr('value', alterObj.numero);
    $('#complemento').attr('value', alterObj.complemento);
    $('#origem')[0].value = alterObj.origem;
    $('#data_origem')[0].value = alterObj.data_origem;
    $('#extra_nome')[0].value = alterObj.extra_nome;
    $('#extra_cpf')[0].value = alterObj.extra_cpf;
    $('#extra_rg')[0].value = alterObj.extra_rg;
    $('#extra_nacionalidade')[0].value = alterObj.extra_nacionalidade;
    $('#extra_profissao')[0].value = alterObj.extra_profissao;
    $('#extra_renda')[0].value = alterObj.extra_renda;
    $('#observacoes')[0].value = alterObj.observacoes;
}

$('#cep').on('keyup', function () {
    if (this.value.length !== 9)
        return;

    let cep = $(this).cleanVal();
    fetch(`https://viacep.com.br/ws/${cep}/json/`)
        .then((response) => {
            response.json()
                .then((data) => {
                    let endereco = $('#endereco');
                    if (!data.logradouro || !data.bairro || !data.localidade || !data.estado){
                        endereco.prop('readonly', false);
                    }
                    else {
                        endereco.prop('readonly', true);
                    }

                    endereco[0].value = `${data.logradouro} - ${data.bairro}, ${data.localidade} - ${data.estado}`;
                });
        });
})
