$('.cpfcnpj-switch').change(function () {
  let ret, mask;
  if (this.checked) {
      ret = "CNPJ";
      mask = "00.000.000/0000-00"
  }
  else {
      ret = "CPF";
      mask = "000.000.000-00"
  }
  $('.cpfcnpj-label').text(ret);
  $('#cpf_cnpj').mask(mask);
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

$('form').submit((e) => {
  e.preventDefault();
  if (e.target.id.value == 0) {
    formCallback = (data, type) => {
      if (type === 'error') {
        let message = "";
        let errs = 0;
        for (const i in data)
        {
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
      else {
        $('form').trigger('reset');
      }
    };
    
    invokeWorker('createColaborador', {
      nome: e.target.nome.value,
      id_cargo: parseInt(e.target.id_cargo.value),
      email: e.target.email.value,
      telefone: $('#telefone').cleanVal(),
      cpf_cnpj: $('#cpf_cnpj').cleanVal()
    }, formCallback);
  }
  else {
    formCallback = (data, type) => {
      if (type === 'error') {
        let message = `O${data.length > 1 ? 's' : ''} seguinte${data.length > 1 ? 's' : ''} campo${data.length > 1 ? 's' : ''} ${data.length > 1 ? 'estão' : 'está'} incorreto${data.length > 1 ? 's' : ''}: `;
        for (const i in data)
        {
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

    invokeWorker('updateColaborador', {
      id: parseInt(e.target.id.value),
      nome: e.target.nome.value,
      id_cargo: parseInt(e.target.id_cargo.value),
      email: e.target.email.value,
      telefone: $('#telefone').cleanVal(),
      cpf_cnpj: $('#cpf_cnpj').cleanVal()
    }, formCallback);
  }
});

if (!!alterObj) {
  $('#id').attr('value', alterObj.id);
  $('#nome').attr('value', alterObj.nome);
  $('#id_cargo')[0].value = alterObj.id_cargo;
  $('#email').attr('value', alterObj.email);
  $('#telefone').attr('value', $('#telefone-mask').masked(alterObj.telefone));
  $('#cpf_cnpj').attr('value', alterObj.cpf_cnpj);
}
