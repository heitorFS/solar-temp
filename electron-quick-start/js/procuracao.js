let numberToMonth = (num) => {
    switch (num) {
        case 0:
            return 'Janeiro';
        case 1:
            return 'Fevereiro';
        case 2:
            return 'Março';
        case 3:
            return 'Abril';
        case 4:
            return 'Maio';
        case 5:
            return 'Junho';
        case 6:
            return 'Julho';
        case 7:
            return 'Agosto';
        case 8:
            return 'Setembro';
        case 9:
            return 'Outubro';
        case 10:
            return 'Novembro';
        case 11:
            return 'Dezembro';
    }
}

let generateProcuracao = (nome, cpf, cep, endereco, numero, companhia, codigo) => {
    let endereco_arr = endereco.split(' - ');
    let date = new Date();
    let ret =
        `<style>
            body {
                padding: 2cm 2cm 2cm 3cm;
                font-size: 11pt;
                font-family: 'Arial';
            }

            .caps {
                text-transform: uppercase;
            }
        </style>
        <body>
            <center><b>PROCURAÇÃO</b></center>
            <br /><br />
            <b>OUTORGANTE:</b>&nbsp;<span class="caps">${nome}</span>, inscrito no CPF nº&nbsp;
            ${cpf}, localizado na <span class="caps">${endereco_arr[0]}</span>,&nbsp;
            ${numero}&nbsp;- BAIRRO:&nbsp;<span class="caps">${endereco_arr[1]}</span>&nbsp;- CIDADE:&nbsp;
            <span class="caps">${endereco_arr[2]}</span>&nbsp;-&nbsp;<span class="caps">${endereco_arr[3]}</span>&nbsp;- CEP&nbsp;
            ${cep}, cliente da ${companhia}, com o seguinte código de cliente nº ${codigo}.
            <br /><br />
            <b>OUTORGADO:</b>&nbsp;JOAQUIM PEREIRA DA SILVA FILHO, eletrotécnico, brasileiro, empresário, portador
            CFT 13530081850, CPF 135.300.818-50 estabelecido profissionalmente na Rua Pampulha, 668, Condomínio
            Laguna - Bairro Vale dos Lagos, CEP: 13223-766, na cidade de Várzea Paulista/SP.
            <br /><br />
            <b>PODERES:</b>&nbsp;Atuar junto a Concessionária de energia ${companhia}, podendo ter vistas e procedimentos de conexão,
            assinar requerimentos, dar entrada a procedimentos de acesso a minigeração e
            microgeração de energia solar fotovoltaica, consulta de acesso, solicitação de parecer de
            acesso, aumento de carga, adequações no ramal de entrada e por fim representar o
            outorgante em todos os atos inerentes ao ofício do outorgado, bem como autorizo a dar
            vistas e assinaturas nos respectivos documentos listados abaixo:
            <br /><br />
            1. Formulário de Registro de Geração Distribuída;<br />
            2. Formulário de Solicitação de Acesso;<br />
            3. Projeto Completo de Energia Fotovoltaica;<br />
            4. TRT - Termo de Responsabilidade Técnica - Projeto e Execução;<br />
            5. Documentação de adequação de Ramal de Entrada;<br />
            6. Aumento de carga;<br />
            7. Solicitação de Troca de Titularidade;<br />
            8. Documento de Rateio de Créditos;<br />
            9. Solicitação de Transferencia de Creditos;<br />
            10. Memorial Técnico Descritivo;<br />
            11. Planta de Situação e Localização;<br />
            12. Diagrama Unifilar e Trifilar da instalação;
            <br /><br /><br />
            <center>Local/Data:&nbsp;${endereco_arr[2]},&nbsp;<span class="date">${date.getDate()} de ${numberToMonth(date.getMonth())} de ${date.getFullYear()}</span></center>
            <br /><br /><br /><hr /><br />
            <center><b><span class="caps">${nome}</span></b><br /><br />CPF:&nbsp;${cpf}</center>
        </body>`

    formCallback = (buffer) => {
        $('#loader-container').css('display', 'none');
        showPopup('Procuração criada com sucesso', 'success');
    }
        
    invokeWorker('generateProcuracao', {content: ret, nome: nome}, formCallback);
}