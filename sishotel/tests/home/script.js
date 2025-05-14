import TableReservas from "./tableReservas.js";

var currentNavBtn = null;
var currentContent = null;
// var currentTableRow = null;
const defaultcontentName = 'reservas';

$(() => {
    let nav_btns = document.getElementsByClassName("nav-btn");
    let table = new TableReservas();

    // document.addEventListener(TableReservas.EVENTS_ID.CurrentRowChanged, (e) => console.log("id da linha atual:", e.detail.id));

    // configurando botões de navegação
    for(let i = 0; i < nav_btns.length; i++) {
        let element = nav_btns[i];

        element.addEventListener("click", () => { on_navBtn_clicked(element) });
    }

    // exibindo conteúdo inicial
    $(`#nav-btn-${defaultcontentName}`).click();
});


// Nav
function on_navBtn_clicked(element) {
    if(currentNavBtn == element || !setContentByName(element.id.slice("nav-btn-".length))) {
        return;
    }

    if(currentNavBtn != null) {
        currentNavBtn.classList.remove("nav-btn-focus");
    }

    currentNavBtn = element;
    element.classList.add("nav-btn-focus");

}

function setContentByName(name) {
    let element = document.getElementById(`container-${name}`);

    if(!element)
        return false;

    if(currentContent) {
        currentContent.hide();
    }

    currentContent = $(element);
    currentContent.show();

    return true;
}