$('#valor').mask("#.##0,00", {reverse: true});

if(!alterObj)
{
    $('#modulos').html(`        
        <div class="modulo">
            <div class="form-subtitle"><i class="fa-solid fa-solar-panel"></i>Módulo 1</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Potência</label>
                        <div class="input-measure">
                            <input id="mod-1_potencia" type="number" required min="1" />
                            <div class="measure"><span>Wp</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Marca</label>
                        <input id="mod-1_marca" type="text" required />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="mod-1_quantidade" type="number" required min="1" />
                    </div>
                    <div class="field">
                        <label>Peso</label>
                        <div class="input-measure">
                            <input id="mod-1_peso" type="number" required min="1" />
                            <div class="measure"><span>kg</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Comprimento</label>
                        <div class="input-measure">
                            <input id="mod-1_comprimento" type="number" required min="1" />
                            <div class="measure"><span>mm</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Largura</label>
                        <div class="input-measure">
                            <input id="mod-1_largura" type="number" required min="1" />
                            <div class="measure"><span>mm</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Garantia Defeito</label>
                        <div class="input-measure">
                            <input id="mod-1_gar-def" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Garantia Eficiência</label>
                        <div class="input-measure">
                            <input id="mod-1_gar-efic" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    `);

    $('#inversores').html(`        
        <div class="inversor">
            <div class="form-subtitle"><i class="fa-solid fa-bolt"></i>Inversor 1</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Marca</label>
                        <input id="inv-1_marca" type="text" required />
                    </div>
                    <div class="field">
                        <label>Modelo</label>
                        <input id="inv-1_modelo" type="text" required />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Monitoramento</label>
                        <select id="inv-1_monitoramento" required>
                            <option value="0">Wi-Fi</option>
                            <option value="1">4G</option>
                            <option value="2">Nenhum</option>
                        </select>
                    </div>
                    <div class="field">
                        <label>Potência</label>
                        <div class="input-measure">
                            <input id="inv-1_potencia" type="number" required min="1" />
                            <div class="measure"><span>W</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="inv-1_quantidade" type="number" required min="1" />
                    </div>
                    <div class="field">
                        <label>Garantia</label>
                        <div class="input-measure">
                            <input id="inv-1_garantia" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                </div>
                <div class="switch-container lone-switch">
                    <label class="switch">
                        <input id="inv-1_microinversor" type="checkbox">
                        <span class="slider"></span>
                    </label>                
                    <span>Microinversor</span>
                </div>
            </div>
        </div>
    `);

    $('#itens').html(`        
        <div class="item">
            <div class="form-subtitle"><i class="fa-solid fa-box-open"></i>Item 1</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Item</label>
                        <input id="item-1_nome" type="text" required />
                    </div>
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="item-1_quantidade" type="number" required />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Fabricante</label>
                        <input id="item-1_fabricante" type="text" required />
                    </div>
                    <div class="field">
                        <label>Detalhes</label>
                        <input id="item-1_detalhes" type="text" required />
                    </div>
                </div>
            </div>
        </div>
    `);
}

$('#add-modulo').on('click', () => {
    let modulos = $('#modulos'), num = modulos[0].children.length + 1;
    modulos.append(`
        <div class="modulo">
            <div class="form-subtitle"><i class="fa-solid fa-solar-panel"></i>Módulo ${num}</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Potência</label>
                        <div class="input-measure">
                            <input id="mod-${num}_potencia" type="number" required min="1" />
                            <div class="measure"><span>Wp</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Marca</label>
                        <input id="mod-${num}_marca" type="text" required />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="mod-${num}_quantidade" type="number" required min="1" />
                    </div>
                    <div class="field">
                        <label>Peso</label>
                        <div class="input-measure">
                            <input id="mod-${num}_peso" type="number" required min="1" />
                            <div class="measure"><span>kg</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Comprimento</label>
                        <div class="input-measure">
                            <input id="mod-${num}_comprimento" type="number" required min="1" />
                            <div class="measure"><span>mm</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Largura</label>
                        <div class="input-measure">
                            <input id="mod-${num}_largura" type="number" required min="1" />
                            <div class="measure"><span>mm</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Garantia Defeito</label>
                        <div class="input-measure">
                            <input id="mod-${num}_gar-def" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                    <div class="field">
                        <label>Garantia Eficiência</label>
                        <div class="input-measure">
                            <input id="mod-${num}_gar-efic" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    `);
    let button = $('#remove-modulo');
    if (button[0].classList.contains('op-disabled'))
        button.removeClass('op-disabled')
});

$('#remove-modulo').on('click', () => {
    let modulos = $('#modulos');
    switch(modulos[0].children.length) {
        case 1:
            return;
        case 2:
            var button = $('#remove-modulo');
            button.addClass('op-disabled');
            break;
    }
    if (!!alterObj)
        alterObj.mod_remove.push(parseInt(modulos.find('.modulo:last').data('id')));

    modulos.find('.modulo:last').remove();
});

$('#add-inversor').on('click', () => {
    let inversores = $('#inversores'), num = inversores[0].children.length + 1;
    inversores.append(`
        <div class="inversor">
            <div class="form-subtitle"><i class="fa-solid fa-bolt"></i>Inversor ${num}</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Marca</label>
                        <input id="inv-${num}_marca" type="text" required />
                    </div>
                    <div class="field">
                        <label>Modelo</label>
                        <input id="inv-${num}_modelo" type="text" required />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Monitoramento</label>
                        <select id="inv-${num}_monitoramento" required>
                            <option value="0">Wi-Fi</option>
                            <option value="1">4G</option>
                            <option value="2">Nenhum</option>
                        </select>
                    </div>
                    <div class="field">
                        <label>Potência</label>
                        <div class="input-measure">
                            <input id="inv-${num}_potencia" type="number" required min="1" />
                            <div class="measure"><span>W</span></div>
                        </div>
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="inv-${num}_quantidade" type="number" required min="1" />
                    </div>
                    <div class="field">
                        <label>Garantia</label>
                        <div class="input-measure">
                            <input id="inv-${num}_gar" type="number" required min="0" />
                            <div class="measure"><span>Anos</span></div>
                        </div>
                    </div>
                </div>
                <div class="switch-container lone-switch">
                    <label class="switch">
                        <input id="inv-${num}_microinversor" type="checkbox">
                        <span class="slider"></span>
                    </label>                
                    <span>Microinversor</span>
                </div>
            </div>
        </div>
    `);
    let button = $('#remove-inversor');
    if (button[0].classList.contains('op-disabled'))
        button.removeClass('op-disabled')
});

$('#remove-inversor').on('click', () => {
    let inversores = $('#inversores');
    switch(inversores[0].children.length) {
        case 1:
            return;
        case 2:
            var button = $('#remove-inversor');
            button.addClass('op-disabled');
            break;
    }
    if (!!alterObj)
        alterObj.inv_remove.push(parseInt(inversores.find('.inversor:last').data('id')));

    inversores.find('.inversor:last').remove();
});

$('#add-item').on('click', () => {
    let itens = $('#itens'), num = itens[0].children.length + 1;
    itens.append(`
        <div class="item">
            <div class="form-subtitle"><i class="fa-solid fa-box-open"></i>Item ${num}</div>
            <div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Item</label>
                        <input id="item-${num}_nome" type="text" required />
                    </div>
                    <div class="field">
                        <label>Quantidade</label>
                        <input id="item-${num}_quantidade" type="number" required min="1" />
                    </div>
                </div>
                <div class="col-1fr-1fr">
                    <div class="field">
                        <label>Fabricante</label>
                        <input id="item-${num}_fabricante" type="text" required />
                    </div>
                    <div class="field">
                        <label>Detalhes</label>
                        <input id="item-${num}_detalhes" type="text" required />
                    </div>
                </div>
            </div>
        </div>
    `);
    let button = $('#remove-item');
    if (button[0].classList.contains('op-disabled'))
        button.removeClass('op-disabled')
});

$('#remove-item').on('click', () => {
    let itens = $('#itens');
    switch(itens[0].children.length) {
        case 1:
            return;
        case 2:
            var button = $('#remove-item');
            button.addClass('op-disabled');
            break;
    }
    if (!!alterObj)[]
        alterObj.item_remove.push(parseInt(itens.find('.item:last').data('id')));

    itens.find('.item:last').remove();
});

$('form').submit((e) => {
    e.preventDefault();

    var form = $('form')[0];
    for (let i = 1; i < form.length - 1; i++) {
        form[i].style.borderColor = '#000';
    }

    if ($('#id').attr('value') == '0') {
        formCallback = (id) => {
            let len = $('#modulos')[0].children.length, errs = [false, false, false];
            for (let i = 1; i <= len; ++i) {
                let modulo = {
                    id_kit: id,
                    potencia: parseInt(e.target[`mod-${i}_potencia`].value),
                    marca: e.target[`mod-${i}_marca`].value,
                    quantidade: parseInt(e.target[`mod-${i}_quantidade`].value),
                    peso: parseFloat(e.target[`mod-${i}_peso`].value),
                    comprimento: parseInt(e.target[`mod-${i}_comprimento`].value),
                    largura: parseInt(e.target[`mod-${i}_largura`].value),
                    garantia_defeito: parseInt(e.target[`mod-${i}_gar-def`].value),
                    garantia_eficiencia: parseInt(e.target[`mod-${i}_gar-efic`].value)
                };
                
                invokeWorker('createModulo', modulo, (_ = null, err = null) => {
                    if (!!err)
                        errs[0] = true;
                });
            }

            len = $('#inversores')[0].children.length;
            for (let i = 1; i <= len; ++i) {
                let inversor = {
                    id_kit: id,
                    marca: e.target[`inv-${i}_marca`].value,
                    modelo: e.target[`inv-${i}_modelo`].value,
                    id_monitoramento: parseInt(e.target[`inv-${i}_monitoramento`].value),
                    potencia: parseInt(e.target[`inv-${i}_potencia`].value),
                    quantidade: parseInt(e.target[`inv-${i}_quantidade`].value),
                    garantia: parseInt(e.target[`inv-${i}_garantia`].value),
                    microinversor: e.target[`inv-${i}_microinversor`].checked ? 1 : 0
                };
                
                invokeWorker('createInversor', inversor, (_ = null, err = null) => {
                    if (!!err)
                        errs[1] = true;
                });
            }

            len = $('#itens')[0].children.length;
            for (let i = 1; i <= len; ++i) {
                let item = {
                    id_kit: id,
                    nome: e.target[`item-${i}_nome`].value,
                    quantidade: parseInt(e.target[`item-${i}_quantidade`].value),
                    fabricante: e.target[`item-${i}_fabricante`].value,
                    detalhe: e.target[`item-${i}_detalhes`].value
                };
                
                invokeWorker('createItem', item, (_ = null, err = null) => {
                    if (!!err)
                        errs[2] = true;
                });
            }

            showPopup('Kit criado com sucesso', 'success');
            $('form').trigger('reset');
        };
        
        invokeWorker('createKit', {
            fornecedor: e.target.fornecedor.value,
            valor: $('#valor').cleanVal() === '' ? 0 : parseFloat($('#valor').cleanVal() / 100),
            tensao: parseInt(e.target.id_tensao.value),
            fixacao: parseInt(e.target.id_fixacao.value),
            observacoes: e.target.observacoes.value
        }, formCallback);
    }
    else {
        formCallback = () => {
            let id = parseInt($('#id').attr('value'));

            for (const modulo of alterObj.mod_remove) {
                invokeWorker("deleteModulo", modulo);
            }

            for (const inversor of alterObj.inv_remove) {
                invokeWorker("deleteInversor", inversor);
            }

            for (const item of alterObj.item_remove) {
                invokeWorker("deleteItem", item);
            }

            for (const item of alterObj.mod_change) {
                let div = $(`.modulo[data-id=${item}] input`);
                console.log(item)
                let modulo = {
                    id: item,
                    id_kit: id,
                    potencia: parseInt(div[0].value),
                    marca: div[1].value,
                    quantidade: parseInt(div[2].value),
                    peso: parseFloat(div[3].value),
                    comprimento: parseInt(div[4].value),
                    largura: parseInt(div[5].value),
                    garantia_defeito: parseInt(div[6].value),
                    garantia_eficiencia: parseInt(div[7].value)
                };

                invokeWorker('updateModulo', modulo, () => {});
            }

            for (const item of alterObj.inv_change) {
                let div = $(`.inversor[data-id=${item}] input, .inversor[data-id=${item}] select`);                
                let inversor = {
                    id: item,
                    id_kit: id,
                    marca: div[0].value,
                    modelo: div[1].value,
                    id_monitoramento: parseInt(div[2].value),
                    potencia: parseInt(div[3].value),
                    quantidade: parseInt(div[4].value),
                    garantia: parseInt(div[5].value),
                    microinversor: div[6].checked ? 1 : 0
                };

                invokeWorker('updateInversor', inversor, () => {});
            }

            for (const i of alterObj.item_change) {
                let div = $(`.item[data-id=${i}] input`);
                let item = {
                    id: i,
                    id_kit: id,
                    nome: div[0].value,
                    quantidade: parseInt(div[1].value),
                    fabricante: div[2].value,
                    detalhe: div[3].value
                };

                invokeWorker('updateItem', item, () => {})                
            }

            let len = $('#modulos')[0].children.length;
            for (let i = alterObj.modulos.length - alterObj.mod_remove.length + 1; i <= len; ++i) {
                let modulo = {
                    id_kit: id,
                    potencia: parseInt(e.target[`mod-${i}_potencia`].value),
                    marca: e.target[`mod-${i}_marca`].value,
                    quantidade: parseInt(e.target[`mod-${i}_quantidade`].value),
                    peso: parseFloat(e.target[`mod-${i}_peso`].value),
                    comprimento: parseInt(e.target[`mod-${i}_comprimento`].value),
                    largura: parseInt(e.target[`mod-${i}_largura`].value),
                    garantia_defeito: parseInt(e.target[`mod-${i}_gar-def`].value),
                    garantia_eficiencia: parseInt(e.target[`mod-${i}_gar-efic`].value)
                };
                
                invokeWorker('createModulo', modulo, (_ = null, err = null) => {
                    if (!!err)
                        errs[0] = true;
                });
            }

            len = $('#inversores')[0].children.length;
            for (let i = alterObj.inversores.length - alterObj.inv_remove.length + 1; i <= len; ++i) {
                let inversor = {
                    id_kit: id,
                    marca: e.target[`inv-${i}_marca`].value,
                    modelo: e.target[`inv-${i}_modelo`].value,
                    id_monitoramento: parseInt(e.target[`inv-${i}_monitoramento`].value),
                    potencia: parseInt(e.target[`inv-${i}_potencia`].value),
                    quantidade: parseInt(e.target[`inv-${i}_quantidade`].value),
                    garantia: parseInt(e.target[`inv-${i}_garantia`].value),
                    microinversor: e.target[`inv-${i}_microinversor`].checked ? 1 : 0
                };
                
                invokeWorker('createInversor', inversor, (_ = null, err = null) => {
                    if (!!err)
                        errs[1] = true;
                });
            }

            len = $('#itens')[0].children.length;
            for (let i = alterObj.itens.length - alterObj.item_remove.length + 1; i <= len; ++i) {
                let item = {
                    id_kit: id,
                    nome: e.target[`item-${i}_nome`].value,
                    quantidade: parseInt(e.target[`item-${i}_quantidade`].value),
                    fabricante: e.target[`item-${i}_fabricante`].value,
                    detalhe: e.target[`item-${i}_detalhes`].value
                };
                
                invokeWorker('createItem', item, (_ = null, err = null) => {
                    if (!!err)
                        errs[2] = true;
                });
            }
            
            $('form').trigger('reset');
            closeModal();
            invokeWorker("getAllKits", null, allRows);
        };

        invokeWorker('updateKit', {
            id: parseInt($('#id').attr('value')),
            fornecedor: e.target.fornecedor.value,
            valor: $('#valor').cleanVal() === '' ? 0 : parseFloat($('#valor').cleanVal() / 100),
            tensao: parseInt(e.target.id_tensao.value),
            fixacao: parseInt(e.target.id_fixacao.value),
            observacoes: e.target.observacoes.value
        }, formCallback);
    }
});

if (!!alterObj) {
    $('.form-title').html('Alterar Kit');
    if (alterObj.modulos.length > 1)
        $('#remove-modulo').removeClass('op-disabled')
    if (alterObj.inversores.length > 1)
        $('#remove-inversor').removeClass('op-disabled')
    if (alterObj.itens.length > 1)
        $('#remove-item').removeClass('op-disabled')

    $('#id').attr('value', alterObj.id);
    $('#fornecedor').attr('value', alterObj.fornecedor);
    $('#valor').attr('value', $('#valor').masked(alterObj.valor));
    $('#id_tensao')[0].value = alterObj.tensao;
    $('#id_fixacao')[0].value = alterObj.tensao;
    $('#observacoes')[0].value = alterObj.observacoes;

    onChange = (obj, id) => {
        switch (obj) {
            case 'modulo':
                if (!alterObj.mod_change.includes(id))
                    alterObj.mod_change.push(id);
                break;
            case 'inversor':
                if (!alterObj.inv_change.includes(id))
                    alterObj.inv_change.push(id);
                break;
            case 'item':
                if (!alterObj.item_change.includes(id))
                    alterObj.item_change.push(id);
                break;
        }
    }

    let count = 1;
    let obj = $('#modulos');
    alterObj.modulos.forEach((modulo) => {
        obj.append(`
            <div class="modulo" data-id="${modulo.id}">
                <div class="form-subtitle"><i class="fa-solid fa-solar-panel"></i>Módulo ${count}</div>
                <div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Potência</label>
                            <div class="input-measure">
                                <input id="mod-${count}_potencia" type="number" required min="1" value="${modulo.potencia}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>Wp</span></div>
                            </div>
                        </div>
                        <div class="field">
                            <label>Marca</label>
                            <input id="mod-${count}_marca" type="text" required value="${modulo.marca}" onchange="onChange('modulo', ${modulo.id})" />
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Quantidade</label>
                            <input id="mod-${count}_quantidade" type="number" required min="1" value="${modulo.quantidade}" onchange="onChange('modulo', ${modulo.id})" />
                        </div>
                        <div class="field">
                            <label>Peso</label>
                            <div class="input-measure">
                                <input id="mod-${count}_peso" type="number" required min="1" value="${modulo.peso}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>kg</span></div>
                            </div>
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Comprimento</label>
                            <div class="input-measure">
                                <input id="mod-${count}_comprimento" type="number" required min="1" value="${modulo.comprimento}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>mm</span></div>
                            </div>
                        </div>
                        <div class="field">
                            <label>Largura</label>
                            <div class="input-measure">
                                <input id="mod-${count}_largura" type="number" required min="1" value="${modulo.largura}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>mm</span></div>
                            </div>
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Garantia Defeito</label>
                            <div class="input-measure">
                                <input id="mod-${count}_gar-def" type="number" required min="0" value="${modulo.garantia_defeito}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>Anos</span></div>
                            </div>
                        </div>
                        <div class="field">
                            <label>Garantia Eficiência</label>
                            <div class="input-measure">
                                <input id="mod-${count}_gar-efic" type="number" required min="0" value="${modulo.garantia_eficiencia}" onchange="onChange('modulo', ${modulo.id})" />
                                <div class="measure"><span>Anos</span></div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        `);
        count++;
    });

    count = 1;
    obj = $('#inversores');
    alterObj.inversores.forEach((inversor) => {
        obj.append(`   
            <div class="inversor" data-id="${inversor.id}">
                <div class="form-subtitle"><i class="fa-solid fa-bolt"></i>Inversor ${count}</div>
                <div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Marca</label>
                            <input id="inv-${count}_marca" type="text" required value="${inversor.marca}" onchange="onChange('inversor', ${inversor.id})" />
                        </div>
                        <div class="field">
                            <label>Modelo</label>
                            <input id="inv-${count}_modelo" type="text" required value="${inversor.modelo}" onchange="onChange('inversor', ${inversor.id})" />
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Monitoramento</label>
                            <select id="inv-${count}_monitoramento" required value="${inversor.monitoramento}" onchange="onChange('inversor', ${inversor.id})">
                                <option value="0">Wi-Fi</option>
                                <option value="1">4G</option>
                                <option value="2">Nenhum</option>
                            </select>
                        </div>
                        <div class="field">
                            <label>Potência</label>
                            <div class="input-measure">
                                <input id="inv-${count}_potencia" type="number" required min="1" value="${inversor.potencia}" onchange="onChange('inversor', ${inversor.id})" />
                                <div class="measure"><span>W</span></div>
                            </div>
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Quantidade</label>
                            <input id="inv-${count}_quantidade" type="number" required min="1" value="${inversor.quantidade}" onchange="onChange('inversor', ${inversor.id})" />
                        </div>
                        <div class="field">
                            <label>Garantia</label>
                            <div class="input-measure">
                                <input id="inv-${count}_garantia" type="number" required min="0" value="${inversor.garantia}" onchange="onChange('inversor', ${inversor.id})" />
                                <div class="measure"><span>Anos</span></div>
                            </div>
                        </div>
                    </div>
                    <div class="switch-container lone-switch">
                        <label class="switch">
                            <input id="inv-${count}_microinversor" type="checkbox" ${inversor.microinversor === 1 ? 'checked' : ''} onchange="onChange('inversor', ${inversor.id})">
                            <span class="slider"></span>
                        </label>                
                        <span>Microinversor</span>
                    </div>
                </div>
            </div>
        `);
        count++;
    });

    count = 1;
    obj = $('#itens');
    alterObj.itens.forEach((item) => {
        obj.append(`
            <div class="item" data-id="${item.id}">
                <div class="form-subtitle"><i class="fa-solid fa-box-open"></i>Item ${count}</div>
                <div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Item</label>
                            <input id="item-${count}_nome" type="text" required value="${item.nome}" onchange="onChange('item', ${item.id})" />
                        </div>
                        <div class="field">
                            <label>Quantidade</label>
                            <input id="item-${count}_quantidade" type="number" required min="1" value="${item.quantidade}" onchange="onChange('item', ${item.id})" />
                        </div>
                    </div>
                    <div class="col-1fr-1fr">
                        <div class="field">
                            <label>Fabricante</label>
                            <input id="item-${count}_fabricante" type="text" required value="${item.fabricante}" onchange="onChange('item', ${item.id})" />
                        </div>
                        <div class="field">
                            <label>Detalhes</label>
                            <input id="item-${count}_detalhes" type="text" required value="${item.detalhes}" onchange="onChange('item', ${item.id})" />
                        </div>
                    </div>
                </div>
            </div>
        `);
        count++;
    });
}
