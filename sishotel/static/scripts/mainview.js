import Table from "./components/table.js";
import { TABLE_RESERVAS, TABLE_CLIENTES } from "./consts.js";

$(() => {
    let view = new MainView();
});

class MainView {

    #currentNavBtn = null;
    #currentContent = null;
    #defaultcontentName = 'reservas';
    #tableClientes;
    #tableReservas;

    constructor() {
        // configurando botões do menu
        document.querySelectorAll(".nav-btn").forEach(btn => {
            btn.addEventListener("click", () => { this.#on_navBtn_clicked(btn) });
        });

        this.#tableReservas = new Table(TABLE_RESERVAS);
        this.#tableClientes = new Table(TABLE_CLIENTES);

        // exibindo conteúdo inicial
        $(`#nav-btn-${this.#defaultcontentName}`).click();
    }

    #on_navBtn_clicked(element) {
        if(this.#currentNavBtn == element || !this.setContentByName(element.id.slice("nav-btn-".length))) {
            return;
        }

        if(this.#currentNavBtn != null) {
            this.#currentNavBtn.classList.remove("nav-btn-focus");
        }

        this.#currentNavBtn = element;
        element.classList.add("nav-btn-focus");

    }

    setContentByName(name) {
        let element = document.getElementById(`container-${name}`);

        if(!element)
            return false;

        if(this.#currentContent) {
            this.#currentContent.hide();
        }

        this.#currentContent = $(element);
        this.#currentContent.show();

        return true;
    }
};