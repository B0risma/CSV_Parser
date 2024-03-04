#pragma once

#include <QHeaderView>
#include <QComboBox>
/*!
 * \brief Данные для заголовка
 */
class HeaderModel : public QAbstractTableModel {
    Q_OBJECT
public:
    HeaderModel(QWidget* parent) : QAbstractTableModel(parent){};
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override{return {};}
    virtual int columnCount(const QModelIndex &parent = {}) const override{
        return count;
    }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{return {};}
    virtual int rowCount(const QModelIndex &parent) const override {return 0;}

    QList<QString> colNames;
    int count=0;
};


/*!
 * \brief Заголовок с возможностью выбора заголовка к столбцу из набора
 */
class ComboHeader : public QHeaderView
{
    Q_OBJECT
public:
    ComboHeader(QWidget *parent);
    void setHeaders(const QList<QString> &headers);
    void setColumnCount(const int colCount);
    void insertSection(const int newColumn);
    //!Заглушка для игнорирования модели таблицы
    virtual void setModel(QAbstractItemModel *model) override {
        Q_UNUSED(model)
        QHeaderView::setModel(data);
    }

public slots:
    virtual void reset() override;

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    QComboBox *createBox();


private:
    QList<QComboBox*> colWgts;
    HeaderModel *data;
};



