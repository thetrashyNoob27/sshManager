#ifndef HOSTTREEITEM_H
#define HOSTTREEITEM_H

#include <QVariant>

class hostTreeItem
{
public:
  hostTreeItem(const QList<QVariant> &data, hostTreeItem *parent = nullptr);
  ~hostTreeItem();

  hostTreeItem *child(int number);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  bool insertChildren(int position, int count, int columns);
  bool insertColumns(int position, int columns);
  hostTreeItem *parent();
  bool removeChildren(int position, int count);
  bool removeColumns(int position, int columns);
  int childNumber() const;
  bool setData(int column, const QVariant &value);

private:
  QList<hostTreeItem *> childItems;
  QList<QVariant> itemData;
  hostTreeItem *parentItem;
};

#endif // HOSTTREEITEM_H
