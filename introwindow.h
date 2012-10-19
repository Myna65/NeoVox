#ifndef INTROWINDOW_H
#define INTROWINDOW_H
#include <QWidget>
#include <QStyledItemDelegate>
#include <QAbstractTableModel>

class IntroTableModel : public QAbstractTableModel
{
Q_OBJECT
public:
    enum Columns { Fr = 0, Nl,id, Count = id + 1 };

    explicit IntroTableModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

    bool insertRow(int row=0, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

private:
    QList<QVector<QVariant> > mElements;

};

class IntroTableDelegate : public QStyledItemDelegate
{
       Q_OBJECT
public:
    explicit IntroTableDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



namespace Ui {
class introwindow;
}

class Introwindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit Introwindow(QWidget *parent = 0, bool modif=false,int ser=0);
    ~Introwindow();
private:
    Ui::introwindow *ui;
    IntroTableModel model;
    IntroTableDelegate delegate;
    int id;
    bool modi;
    virtual void keyPressEvent(QKeyEvent *event);
public slots:
    void clickMot();
    void clickSerie();
    void valNum();
};

#endif // INTROWINDOW_H