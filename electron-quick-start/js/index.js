function openMenu(id) {
    let div = document.querySelector(`div[data-id="${id}"]`);

    if (div.classList.contains('sidebar-open')) {
        div.classList.remove('sidebar-open');
    }
    else {        
        div.classList.add('sidebar-open');
    }
}

function switchScreen(id) {
    let iframe = document.querySelector('iframe');
    iframe.src = `../pages/${id}.html`;
}