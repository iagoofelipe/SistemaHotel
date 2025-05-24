#include "table.h"
#include "ui_table.h"

Table::Table(QWidget *parent, const QString& title, const QStringList& headers)
    : QWidget(parent)
    , ui(new Ui::Table)
    , headers(headers)
    , timer(new QTimer(this))
    , minsLastUpdate(0)
    , numMaxItems(0)
    , currentIndex(0)
    , currentRow(-1)
{
    ui->setupUi(this);
    ui->lbTitle->setText(title);
    timer->setInterval(60000);
    numMaxItems = ui->comboNumRange->currentText().toInt();
    setIntervalText(0, 0, 0);

    ui->widDetails->hide();
    ui->btnPrev->setEnabled(false);
    ui->btnNext->setEnabled(false);

    if(!headers.empty()) {
        ui->tableWid->setColumnCount(headers.size());
        ui->tableWid->setHorizontalHeaderLabels(headers);
    }

    connect(ui->btnUpdate, &QPushButton::clicked, this, &Table::updateRequired);
    connect(ui->btnPrev, &QPushButton::clicked, this, &Table::on_btnPrev_clicked);
    connect(ui->btnNext, &QPushButton::clicked, this, &Table::on_btnNext_clicked);
    connect(timer, &QTimer::timeout, this, &Table::on_timer_timeout);
    connect(ui->comboNumRange, &QComboBox::currentTextChanged, this, &Table::on_comboNumRange_currentTextChanged);
    connect(ui->tableWid, &QTableWidget::itemSelectionChanged, this, &Table::on_widTable_itemSelectionChanged);
    connect(ui->btnCloseDetails, &QPushButton::clicked, this, &Table::on_btnCloseDetails_clicked);
}

Table::~Table()
{
    delete ui;
}

void Table::reset() {
    if(timer->isActive())
        timer->stop();

    data.clear();
    ui->tableWid->clear();
    ui->tableWid->setRowCount(0);
    setIntervalText(0, 0, 0);
    ui->btnPrev->setEnabled(false);
    ui->btnNext->setEnabled(false);

    if(!headers.empty())
        ui->tableWid->setHorizontalHeaderLabels(headers);
}

void Table::clear(int numRows) {
    ui->tableWid->clear();
    ui->tableWid->setRowCount(numRows);

    if(!headers.empty())
        ui->tableWid->setHorizontalHeaderLabels(headers);
}

void Table::setTitle(const QString& text) { ui->lbTitle->setText(text); }

void Table::setIntervalText(int from, int to, int count) {
    QString txt;

    txt +=
        "Exibindo "
        + QString::number(from)
        + " - "
        + QString::number(to)
        + " de "
        + QString::number(count)
        + (count == 1? " item" : " itens")
        ;

    ui->lbRange->setText(txt);
}

void Table::setData(const QList<QList<QVariant>>& data) {
    this->data = data;

    clear(data.size());
    setIntervalIndex(0);
    resetTimer();
}

void Table::appendRow(const QList<QVariant>& row) {
    int
        numRow = ui->tableWid->rowCount(),
        numCol = 0;
    const auto items = createRow(row);
    ui->tableWid->setRowCount(numRow + 1);

    for(auto item : items)
        ui->tableWid->setItem(numRow, numCol++, item);

    resetTimer();
    data.append(row);
    setIntervalIndex(0);
}

void Table::blockUpdate(bool block) {
    ui->btnUpdate->setDisabled(block);
}

void Table::resetTimer() {
    minsLastUpdate = 0;

    if(timer->isActive())
        timer->stop();

    timer->start();
    ui->lbLastUpdate->setText("Atualizado há poucos segundos");
}

void Table::setIntervalIndex(int index) {
    int len = data.size();

    if(!len) {
        reset();
        return;
    }

    int
        numIntervals = getNumIntervals(),
        lastIndex = numIntervals - 1,
        start;

    currentIndex = index >= numIntervals? 0 : index;
    start = currentIndex * numMaxItems;
    bool isLastIndex = currentIndex == lastIndex;
    auto values = data.mid(start, isLastIndex? -1 : numMaxItems);

    ui->btnPrev->setEnabled((bool)currentIndex);
    ui->btnNext->setEnabled(!isLastIndex);

    setTableValues(values);
    setIntervalText(start + 1, start + values.size(), len);
}

void Table::showDetails() {
    ui->widDetails->show();
}

void Table::hideDetails() {
    ui->widDetails->hide();
}

void Table::setDetailsWidget(QWidget *widget) {
    QWidget *old = ui->widDetailsContent;
    ui->widDetailsContent = widget;

    ui->layoutDetails->replaceWidget(old, widget);
}

int Table::getCurrentRow() {
    if(currentRow == -1)
        return -1;

    return currentIndex * numMaxItems + currentRow;
}

QString Table::title() { return ui->lbTitle->text(); }

QTableWidget* Table::tableWidget() { return ui->tableWid; }

QList<QTableWidgetItem*> Table::createRow(const QList<QVariant>& values) {
    QList<QTableWidgetItem*> items;

    for(const auto& content : values) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole, content);
        items.append(item);
    }

    return items;
}

int Table::getNumIntervals() {
    if(!numMaxItems) // caso 0
        return 1;

    int len = data.size();
    return len / numMaxItems + bool(len % numMaxItems);
}

void Table::setTableValues(const QList<QList<QVariant>>& values) {
    clear(values.size());

    int
        numRow = 0,
        numCol;

    for(const auto& row : values) {
        numCol = 0;
        const auto items = createRow(row);

        for(auto item : items)
            ui->tableWid->setItem(numRow, numCol++, item);

        numRow++;
    }
}

void Table::on_timer_timeout() {
    ui->lbLastUpdate->setText("Atualizado há " + QString::number(++minsLastUpdate) + " minuto(s)");
}

void Table::on_comboNumRange_currentTextChanged(const QString& text) {
    numMaxItems = text.toInt();
    setIntervalIndex(0);
}

void Table::on_btnPrev_clicked() {
    setIntervalIndex(currentIndex - 1);
}

void Table::on_btnNext_clicked() {
    setIntervalIndex(currentIndex + 1);
}

void Table::on_widTable_itemSelectionChanged() {
    int selected = ui->tableWid->currentRow();
    currentRow = selected == currentRow? -1 : selected;

    emit currentRowChanged(getCurrentRow());
}

void Table::on_btnCloseDetails_clicked() {
    ui->tableWid->clearSelection();
}
