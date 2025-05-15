import { HTML_TABLE } from "../consts.js";

export const EVT_TABLE_ROW_CHANGED = 'RowChanged';
export const EVT_TABLE_ROW_SELECTED = 'RowSelected';

export default class Table {

    #parentId;
    #name;
    #title;
    #columns;

    #headId;
    #bodyId;
    #navTextId;

    #data = [];
    #oldRowSelected = null;
    #currentRow = null;

    constructor(props) {
        this.#parentId = '#' + props.parentId;
        this.#name = props.name;
        this.#columns = props.columns;
        this.#title = props.title ?? '';

        this.#headId = '#table-head-' + props.name;
        this.#bodyId = '#table-body-' + props.name;
        this.#navTextId = '#table-nav-text-' + props.name;

        this.#generateElement();
        this.#hideBtns();

        // vinculando eventos
        let evtIdRowChanged = this.getEventId(EVT_TABLE_ROW_CHANGED);
        let evtIdRowSelected = this.getEventId(EVT_TABLE_ROW_SELECTED);

        document.addEventListener(evtIdRowChanged, (evt) => this.#onRowChanged(evt) );
        document.addEventListener(evtIdRowSelected, (evt) => this.#onRowSelected(evt) );
    }

    setData(data, from, to, total) {
        let rowNum = 0;
        let body = $(this.#bodyId);
        let len = data.length;
        let evtIdRowSelected = this.getEventId(EVT_TABLE_ROW_SELECTED);
        this.#data = data;

        body.html('');

        data.forEach(row => {
            let id = `table-${this.#name}-row-${rowNum++}`;
            let tr = $('<tr>')
                .prop('id', id)
                .appendTo(body)
                .click(() => {
                    let evtRowSelected = new CustomEvent(evtIdRowSelected, { detail: id });
                    document.dispatchEvent(evtRowSelected);
                })
                ;

            row.forEach(col => {
                $('<td>')
                    .text(col)
                    .appendTo(tr);
            });
        });

        // atualizando texto
        $(this.#navTextId).html(
            `Exibindo <strong>${from ?? (len? 1 : 0)}</strong>-<strong>${to ?? len}</strong> de <strong>${total ?? len}</strong> valores`
        );
    }

    clearData() {
        this.setData([]);
        this.#hideBtns();

        this.#oldRowSelected = null;
        this.#currentRow = null;
    }
    
    getEventId(evtTable) {
        return `Table.${this.#name}-${evtTable}`;
    }

    #generateElement() {
        let parent = $(this.#parentId);
        let html = HTML_TABLE
            .replaceAll('%TITLE%', this.#title)
            .replaceAll('%NAME%', this.#name);

        $(html).appendTo(parent);

        // adicionando colunas
        let row_thead = $('<tr>');
        row_thead.appendTo(this.#headId);

        this.#columns.forEach(col => {
            $('<th>')
                .prop('scope', 'col')
                .text(col)
                .appendTo(row_thead);
        });
    }

    #onRowSelected(evt) {
        let row = document.getElementById(evt.detail);

        if(row == this.#currentRow)
            return;

        // alterando variáveis
        this.#oldRowSelected = this.#currentRow;
        this.#currentRow = row;

        // emitindo evento de que outra linha foi selecionada
        let evtCurrentRowChanged = new CustomEvent(this.getEventId(EVT_TABLE_ROW_CHANGED), { detail: row });
        document.dispatchEvent(evtCurrentRowChanged);

        // exibindo componentes
        this.#showBtns();
    }

    #onRowChanged(evt) {
        if(this.#oldRowSelected != null)
            this.#oldRowSelected.classList.remove("table-active");

        if(this.#currentRow != null)
            this.#currentRow.classList.add("table-active");
    }

    #showBtns() {
        $(`#btn-edit-${this.#name}`).show();
        $(`#btn-details-${this.#name}`).show();
    }

    #hideBtns() {
        $(`#btn-edit-${this.#name}`).hide();
        $(`#btn-details-${this.#name}`).hide();
    }
};