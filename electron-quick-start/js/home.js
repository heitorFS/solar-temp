const setButton = document.getElementById('home-button');

setButton.addEventListener('click', () => {
  invokeWorker('createColaborador', {
    nome: "Fernando",
    id_cargo: 1,
    email: "fernando@gmail.com",
    telefone: "11920069376",
    cpf_cnpj: "46672399801"
  });
});