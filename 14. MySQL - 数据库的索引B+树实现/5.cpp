/**
 * 该头文件修改无效，仅供阅读参考，不要修改
 */
#pragma once

#include <queue>
#include <string>
#include <vector>

#include "concurrency/transaction.h"
#include "storage/index/index_iterator.h"
#include "storage/page/b_plus_tree_internal_page.h"
#include "storage/page/b_plus_tree_leaf_page.h"

namespace bustub {

#define BPLUSTREE_TYPE BPlusTree<KeyType, ValueType, KeyComparator>

/**
 * B+树类声明
 * (1) key不重复，主键唯一
 * (2) 支持插入和删除功能(该关卡仅实现删除,仅展示删除所需函数)
 * (4) 请实现B+树index_iterator类，供B+树遍历使用
 */
INDEX_TEMPLATE_ARGUMENTS
class BPlusTree {

 public:
  explicit BPlusTree(std::string name, BufferPoolManager *buffer_pool_manager, const KeyComparator &comparator,
                     int leaf_max_size = LEAF_PAGE_SIZE, int internal_max_size = INTERNAL_PAGE_SIZE);

  bool IsEmpty() const;

  // Remove a key and its value from this B+ tree.
  void Remove(const KeyType &key, Transaction *transaction = nullptr);

 private:

  template <typename N>
  bool CoalesceOrRedistribute(N *node, Transaction *transaction = nullptr);

  template <typename N>
  bool FindSibling(N *node, N *&sibling);

  template <typename N>
  bool Coalesce(N **neighbor_node, N **node, BPlusTreeInternalPage<KeyType, page_id_t, KeyComparator> **parent,
                int index, Transaction *transaction = nullptr);

  template <typename N>
  void Redistribute(N *neighbor_node, N *node, int index);

  bool AdjustRoot(BPlusTreePage *node);

  void UpdateRootPageId(int insert_record = 0);

  std::string index_name_;
  page_id_t root_page_id_;
  BufferPoolManager *buffer_pool_manager_;
  KeyComparator comparator_;
  int leaf_max_size_;
  int internal_max_size_;
  std::mutex latch_;
};

}  // namespace bustub
