#include "hosttreemodel.h"
#include <QFile>

hostTreeModel::hostTreeModel(QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  QStringList headers;
  headers<<"host\'s name"<<"comment";
  for (const QString &header : headers)
      rootData << header;

  rootItem = new hostTreeItem(rootData);


  QFile file("default.txt");
  file.open(QIODevice::ReadOnly);
  auto data=QString(file.readAll());
  file.close();
  setupModelData(data.split('\n'), rootItem);
}

hostTreeModel::~hostTreeModel()
{
    delete rootItem;
}

QVariant hostTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
      return QVariant();

  if (role != Qt::DisplayRole && role != Qt::EditRole)
      return QVariant();

  auto *item = getItem(index);

  return item->data(index.column());
}

QVariant hostTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return rootItem->data(section);

  return QVariant();
}

QModelIndex hostTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() && parent.column() != 0)
      return QModelIndex();
  hostTreeItem *parentItem = getItem(parent);
  if (!parentItem)
      return QModelIndex();

  hostTreeItem *childItem = parentItem->child(row);
  if (childItem)
      return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex hostTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
      return QModelIndex();

  hostTreeItem *childItem = getItem(index);
  hostTreeItem *parentItem = childItem ? childItem->parent() : nullptr;

  if (parentItem == rootItem || !parentItem)
      return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}

int hostTreeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid() && parent.column() > 0)
      return 0;

  const hostTreeItem *parentItem = getItem(parent);

  return parentItem ? parentItem->childCount() : 0;
}

int hostTreeModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return rootItem->columnCount();
}

Qt::ItemFlags hostTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
      return Qt::NoItemFlags;

  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool hostTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
      return false;

  auto *item = getItem(index);
  bool result = item->setData(index.column(), value);

  if (result)
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

  return result;
}

bool hostTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  if (role != Qt::EditRole || orientation != Qt::Horizontal)
      return false;

  const bool result = rootItem->setData(section, value);

  if (result)
      emit headerDataChanged(orientation, section, section);

  return result;
}

bool hostTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
  beginInsertColumns(parent, position, position + columns - 1);
  const bool success = rootItem->insertColumns(position, columns);
  endInsertColumns();

  return success;
}

bool hostTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
  beginRemoveColumns(parent, position, position + columns - 1);
  const bool success = rootItem->removeColumns(position, columns);
  endRemoveColumns();

  if (rootItem->columnCount() == 0)
      removeRows(0, rowCount());

  return success;
}

bool hostTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  auto *parentItem = getItem(parent);
  if (!parentItem)
      return false;

  beginInsertRows(parent, position, position + rows - 1);
  const bool success = parentItem->insertChildren(position,
                                                  rows,
                                                  rootItem->columnCount());
  endInsertRows();

  return success;
}

bool hostTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  auto *parentItem = getItem(parent);
  if (!parentItem)
      return false;

  beginRemoveRows(parent, position, position + rows - 1);
  const bool success = parentItem->removeChildren(position, rows);
  endRemoveRows();

  return success;
}

void hostTreeModel::setupModelData(const QStringList &lines, hostTreeItem *parent)
{
  QVector<hostTreeItem*> parents;
  QVector<int> indentations;
  parents.append(parent);
  indentations << 0;

  int number = 0;

  while (number < lines.count()) {
      int position = 0;
      while (position < lines[number].length()) {
          if (lines[number].at(position) != ' ')
              break;
          ++position;
      }

      const QString lineData = lines[number].mid(position).trimmed();

      if (!lineData.isEmpty()) {
          // Read the column data from the rest of the line.
          const QStringList columnStrings =
              lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
          QVector<QVariant> columnData;
          columnData.reserve(columnStrings.size());
          for (const QString &columnString : columnStrings)
              columnData << columnString;

          if (position > indentations.last()) {
              // The last child of the current parent is now the new parent
              // unless the current parent has no children.

              if (parents.last()->childCount() > 0) {
                  parents << parents.last()->child(parents.last()->childCount()-1);
                  indentations << position;
              }
          } else {
              while (position < indentations.last() && parents.count() > 0) {
                  parents.pop_back();
                  indentations.pop_back();
              }
          }

          // Append a new item to the current parent's list of children.
          auto *parent = parents.last();
          parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
          for (int column = 0; column < columnData.size(); ++column)
              parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
      }
      ++number;
  }
}

hostTreeItem *hostTreeModel::getItem(const QModelIndex &index) const
{
  if (index.isValid()) {
      hostTreeItem *item = static_cast<hostTreeItem*>(index.internalPointer());
      if (item)
          return item;
  }
  return rootItem;
}
