#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QStringList>
#include <QTableWidget>
#include <QTimer>

namespace Ui {
class Table;
}

class Table : public QWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr, const QString& title = "", const QStringList& headers = {});
    ~Table();

    void reset();
    void clear(int numRows = 0);
    void setTitle(const QString& text);
    void setData(const QList<QList<QVariant>>& data);
    void appendRow(const QList<QVariant>& row);
    void blockUpdate(bool block);
    void resetTimer();
    void setIntervalIndex(int index);
    void showDetails();
    void hideDetails();
    void setDetailsWidget(QWidget *widget);
    int getCurrentRow();

    QString title();
    QTableWidget* tableWidget();

private:
    Ui::Table *ui;
    const QStringList headers;
    QTimer *timer;
    int minsLastUpdate;
    int numMaxItems;
    int currentIndex;
    QList<QList<QVariant>> data;
    int currentRow;

    QList<QTableWidgetItem*> createRow(const QList<QVariant>& values);
    int getNumIntervals();
    void setTableValues(const QList<QList<QVariant>>& values);
    void setIntervalText(int from, int to, int count);

    void on_timer_timeout();
    void on_comboNumRange_currentTextChanged(const QString& text);
    void on_btnPrev_clicked();
    void on_btnNext_clicked();
    void on_widTable_itemSelectionChanged();
    void on_btnCloseDetails_clicked();

signals:
    void updateRequired();
    void currentRowChanged(int row);
};

#endif // TABLE_H
