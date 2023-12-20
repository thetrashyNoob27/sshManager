#include "hosttreeitem.h"

hostTreeItem::hostTreeItem(const QList<QVariant> &data, hostTreeItem *parent): itemData(data), parentItem(parent)
{

}

hostTreeItem::~hostTreeItem()
{
  qDeleteAll(childItems);
}

hostTreeItem *hostTreeItem::child(int number)
{
  if (number < 0 || number >= childItems.size())
      return nullptr;
  return childItems.at(number);
}

int hostTreeItem::childCount() const
{
  return childItems.count();
}

int hostTreeItem::columnCount() const
{
  return itemData.count();
}

QVariant hostTreeItem::data(int column) const
{
  if (column < 0 || column >= itemData.size())
      return QVariant();
  return itemData.at(column);
}

bool hostTreeItem::insertChildren(int position, int count, int columns)
{
  if (position < 0 || position > childItems.size())
      return false;

  for (int row = 0; row < count; ++row) {
      QList<QVariant> data(columns);
      hostTreeItem *item = new hostTreeItem(data, this);
      childItems.insert(position, item);
  }

  return true;
}

bool hostTreeItem::insertColumns(int position, int columns)
{
  if (position < 0 || position > itemData.size())
      return false;

  for (int column = 0; column < columns; ++column)
      itemData.insert(position, QVariant());

  for (auto *child : std::as_const(childItems))
      child->insertColumns(position, columns);

  return true;
}

hostTreeItem *hostTreeItem::parent()
{
  return parentItem;
}

bool hostTreeItem::removeChildren(int position, int count)
{
  if (position < 0 || position + count > childItems.size())
      return false;

  for (int row = 0; row < count; ++row)
      delete childItems.takeAt(position);

  return true;
}

bool hostTreeItem::removeColumns(int position, int columns)
{
  if (position < 0 || position + columns > itemData.size())
      return false;

  for (int column = 0; column < columns; ++column)
      itemData.remove(position);

  for (auto *child : std::as_const(childItems))
      child->removeColumns(position, columns);

  return true;
}

int hostTreeItem::childNumber() const
{
  if (parentItem)
      return parentItem->childItems.indexOf(const_cast<hostTreeItem*>(this));
  return 0;
}

bool hostTreeItem::setData(int column, const QVariant &value)
{
  if (column < 0 || column >= itemData.size())
      return false;

  itemData[column] = value;
  return true;
}
