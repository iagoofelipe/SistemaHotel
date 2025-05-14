export const HTML_TABLE = `
<div class="table-container">

    <div class="table-header">
        <p>%TITLE%</p>
        <div>
            <i id="btn-reload-%NAME%" class="btn fa-solid fa-rotate" title="recarregar %NAME%"></i>
            <i id="btn-add-%NAME%" class="btn fa-solid fa-plus" title="adicionar %NAME%"></i>
            <i id="btn-edit-%NAME%" class="fa-solid fa-pen btn"></i>
            <i id="btn-details-%NAME%" class="fa-solid fa-eye btn"></i>
        </div>
    </div>

    <div class="table-responsive">
        <table class="table table-hover" id="table-%NAME%">
            <thead id="table-head-%NAME%"></thead>
            <tbody id="table-body-%NAME%"></tbody>
        </table>
    </div>


    <div class="table-foot">
        <p id="table-nav-text-%NAME%">Exibindo <strong>0</strong>-<strong>0</strong> de <strong>0</strong> valor(es)</p>
        <div class="table-nav">
            <button class="btn disabled" id="btn-table-prev-%NAME%">< Anterior</button>
            <button class="btn" id="btn-table-next-%NAME%">Próximo ></button>
        </div>
    </div>

</div>
`;