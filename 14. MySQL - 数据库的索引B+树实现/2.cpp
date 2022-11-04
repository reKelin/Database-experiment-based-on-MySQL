#pragma once

#include <queue>

#include "storage/page/b_plus_tree_page.h"

namespace bustub {

#define B_PLUS_TREE_INTERNAL_PAGE_TYPE BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>
#define B_PLUS_TREE_INTERNAL_PAGE BPlusTreeInternalPage<KeyType, page_id_t, KeyComparator>
#define INTERNAL_PAGE_HEADER_SIZE 24
//缺省时内部结点的Max_size
#define INTERNAL_PAGE_SIZE ((PAGE_SIZE - INTERNAL_PAGE_HEADER_SIZE) / (sizeof(MappingType))) - 1

INDEX_TEMPLATE_ARGUMENTS
class BPlusTreeInternalPage : public BPlusTreePage {
 public:
  void Init(page_id_t page_id, page_id_t parent_id = INVALID_PAGE_ID, int max_size = INTERNAL_PAGE_SIZE);

  KeyType KeyAt(int index) const;
  void SetKeyAt(int index, const KeyType &key);
  int ValueIndex(const ValueType &value) const;
  ValueType ValueAt(int index) const;

  ValueType Lookup(const KeyType &key, const KeyComparator &comparator) const;
  void PopulateNewRoot(const ValueType &old_value, const KeyType &new_key, const ValueType &new_value);
  int InsertNodeAfter(const ValueType &old_value, const KeyType &new_key, const ValueType &new_value);
  void Remove(int index);
  ValueType RemoveAndReturnOnlyChild();

  void MoveAllTo(BPlusTreeInternalPage *recipient,BufferPoolManager *buffer_pool_manager);
  void MoveHalfTo(BPlusTreeInternalPage *recipient, BufferPoolManager *buffer_pool_manager);
  void MoveFirstToEndOf(BPlusTreeInternalPage *recipient,
                        BufferPoolManager *buffer_pool_manager);
  void MoveLastToFrontOf(BPlusTreeInternalPage *recipient, 
                         BufferPoolManager *buffer_pool_manager);

 private:
  void CopyNFrom(MappingType *items, int size, BufferPoolManager *buffer_pool_manager);
  void CopyLastFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager);
  void CopyFirstFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager);
  MappingType array_[0];
};
}
