export default class TableReservas {

    static EVENTS_ID = {
        CurrentRowChanged: "TableReservas.CurrentRowChanged",
        RowSelected: "TableReservas.RowSelected",
    };

    #oldRowSelected = null;
    #currentRow = null;

    constructor() {
        this.#hideBtns();
        
        // vinculando eventos
        document.addEventListener(TableReservas.EVENTS_ID.CurrentRowChanged, (evt) => this.#onCurrentRowChanged(evt) );
        document.addEventListener(TableReservas.EVENTS_ID.RowSelected, (evt) => this.#onRowSelected(evt) );

        document.querySelectorAll("#table-reservas tbody tr").forEach((linha) => {
            linha.addEventListener("click", () => {
                let evtRowSelected = new CustomEvent(TableReservas.EVENTS_ID.RowSelected, { detail: linha });
                document.dispatchEvent(evtRowSelected);
            });
        });
    }

    #onRowSelected(evt) {
        let row = evt.detail;

        if(row == this.#currentRow)
            return;

        // alterando variáveis
        this.#oldRowSelected = this.#currentRow;
        this.#currentRow = row;

        // emitindo evento de que outra linha foi selecionada
        let evtCurrentRowChanged = new CustomEvent(TableReservas.EVENTS_ID.CurrentRowChanged, { detail: row });
        document.dispatchEvent(evtCurrentRowChanged);

        // exibindo componentes
        this.#showBtns();
    }

    #onCurrentRowChanged(evt) {
        if(this.#oldRowSelected != null)
            this.#oldRowSelected.classList.remove("table-active");

        if(this.#currentRow != null)
            this.#currentRow.classList.add("table-active");
    }

    #showBtns() {
        $("#btn-edit-reservas").show();
        $("#btn-details-reservas").show();
    }

    #hideBtns() {
        $("#btn-edit-reservas").hide();
        $("#btn-details-reservas").hide();
    }

    currentRowSelected() { return this.#currentRow; }
    oldRowSelected() { return this.#oldRowSelected; }

};