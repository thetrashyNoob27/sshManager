#ifndef HOSTTREEMODEL_H
#define HOSTTREEMODEL_H

#include <QAbstractItemModel>
#include <QWidget>
#include "hosttreeitem.h"

class hostTreeModel : public QAbstractItemModel {
  Q_OBJECT
public:
  hostTreeModel(QObject *parent);
  ~hostTreeModel();

  // just basic
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  // allow it to be rw
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role = Qt::EditRole) override;

  bool insertColumns(int position, int columns,
                     const QModelIndex &parent = QModelIndex()) override;
  bool removeColumns(int position, int columns,
                     const QModelIndex &parent = QModelIndex()) override;
  bool insertRows(int rows, int position,
                  const QModelIndex &parent = QModelIndex()) override;
  bool removeRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex()) override;

private:
  void setupModelData(const QStringList &lines, hostTreeItem *parent);
  hostTreeItem *getItem(const QModelIndex &index) const;

  hostTreeItem *rootItem;
};

#endif // HOSTTREEMODEL_H
