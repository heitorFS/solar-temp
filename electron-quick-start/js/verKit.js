$('#fornecedor').html(alterObj.fornecedor);
$('#valor').html(alterObj.valor.toFixed(2).replace('.', ','));
$('#tensao').html(getTensao(alterObj.tensao));
$('#fixacao').html(getFixacao(alterObj.fixacao));
$('#observacoes').html(alterObj.observacoes.length == 0 ? '-' : alterObj.observacoes);

switchTab = (tab) => {
    switch (tab) {
        case 'modulos':
            $('.view-content').html(`
                <div class="kit-card-container">
                        ${alterObj.modulos.map((modulo) => {
                            return `<div class="kit-card">
                                <div class="table-card-icon-container">
                                    <div class="table-card-icon"><i class="fa-solid fa-solar-panel"></i></div>
                                </div>
                                <div class="kit-card-values">
                                <div class="kit-card-title">Potência</div>
                                <div class="kit-card-info">${modulo.potencia} Wp</div>
                                <div class="kit-card-title">Marca</div>
                                <div class="kit-card-info">${modulo.marca}</div>
                                <div class="kit-card-title">Quantidade</div>
                                <div class="kit-card-info">${modulo.quantidade}</div>
                                <div class="kit-card-title">Peso</div>
                                <div class="kit-card-info">${modulo.peso} kg</div>
                                <div class="kit-card-title">Comprimento</div>
                                <div class="kit-card-info">${modulo.comprimento} mm</div>
                                <div class="kit-card-title">Largura</div>
                                <div class="kit-card-info">${modulo.largura} mm</div>
                                <div class="kit-card-title">Garantia Defeito</div>
                                <div class="kit-card-info">${modulo.garantia_defeito} ano${modulo.garantia_defeito === 1 ? '' : 's'}</div>
                                <div class="kit-card-title">Garantia Eficiência</div>
                                <div class="kit-card-info">${modulo.garantia_eficiencia} ano${modulo.garantia_eficiencia === 1 ? '' : 's'}</div>
                                </div>                                
                            </div>`;
                        }).join('')}
                </div> 
            `);
            break;
        case 'inversores':
            $('.view-content').html(`
                <div class="kit-card-container">
                        ${alterObj.inversores.map((inversor) => {
                            return `<div class="kit-card">
                                <div class="table-card-icon-container">
                                    <div class="table-card-icon"><i class="fa-solid fa-bolt"></i></div>
                                </div>
                                <div class="kit-card-values">
                                <div class="kit-card-title">Marca</div>
                                <div class="kit-card-info">${inversor.marca} Wp</div>
                                <div class="kit-card-title">Modelo</div>
                                <div class="kit-card-info">R$ ${inversor.modelo}</div>
                                <div class="kit-card-title">Monitoramento</div>
                                <div class="kit-card-info">${getMonitoramento(inversor.monitoramento)}</div>
                                <div class="kit-card-title">Potência</div>
                                <div class="kit-card-info">${inversor.potencia} Wp</div>
                                <div class="kit-card-title">Quantidade</div>
                                <div class="kit-card-info">${inversor.quantidade} mm</div>
                                <div class="kit-card-title">Garantia</div>
                                <div class="kit-card-info">${inversor.garantia} mm</div>
                                <div class="kit-card-title">Microinversor</div>
                                <div class="kit-card-info">${inversor.microinversor === 1 ? 'Sim' : 'Não'}</div>
                                </div>                                
                            </div>`;
                        }).join('')}
                </div> 
            `);            
            break;
        case 'itens':
            $('.view-content').html(`
                <div class="kit-card-container">
                        ${alterObj.itens.map((item) => {
                            return `<div class="kit-card">
                                <div class="table-card-icon-container">
                                    <div class="table-card-icon"><i class="fa-solid fa-box-open"></i></div>
                                </div>
                                <div class="kit-card-values">
                                <div class="kit-card-title">Nome</div>
                                <div class="kit-card-info">${item.nome}</div>
                                <div class="kit-card-title">Quantidade</div>
                                <div class="kit-card-info">${item.quantidade}</div>
                                <div class="kit-card-title">Fabricante</div>
                                <div class="kit-card-info">${item.fabricante}</div>
                                <div class="kit-card-title">Detalhes</div>
                                <div class="kit-card-info">${item.detalhes}</div>
                                </div>                                
                            </div>`;
                        }).join('')}
                </div> 
            `);     
            break;
    }

    $('#modulos').removeClass('tab-selected');
    $('#inversores').removeClass('tab-selected');
    $('#itens').removeClass('tab-selected');
    
    $(`#${tab}`).addClass('tab-selected');
}

switchTab('modulos');

