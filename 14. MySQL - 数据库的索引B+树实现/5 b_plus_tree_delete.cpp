#include <string>
#include <iostream>
#include "common/exception.h"
#include "common/rid.h"
#include "storage/index/b_plus_tree.h"
#include "storage/page/header_page.h"

namespace bustub {
INDEX_TEMPLATE_ARGUMENTS
BPLUSTREE_TYPE::BPlusTree(std::string name, BufferPoolManager *buffer_pool_manager, const KeyComparator &comparator,
                          int leaf_max_size, int internal_max_size)
    : index_name_(std::move(name)),
      root_page_id_(INVALID_PAGE_ID),
      buffer_pool_manager_(buffer_pool_manager),
      comparator_(comparator),
      leaf_max_size_(leaf_max_size),
      internal_max_size_(internal_max_size) {}


/*****************************************************************************
 * 删除
 *****************************************************************************/
/*
 * 函数功能：
 *  在B+树中删除key对应的记录
 * 建议：
 *  1.若B+树为空直接返回
 *  2.通过上一个关卡实现的FindLeafPage(key)查找key值所在页并删除记录
 *  3.删除后需判断结点元素是否小于min_size,并调用CoalesceOrRedistribute()进行后续处理
 *  4.注意unpinPage，避免缓冲区内存泄露
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::Remove(const KeyType &key) {
  std::lock_guard<std::mutex> lock(latch_);
  if (IsEmpty()) return;
  Page *page = FindLeafPage(key);
  assert(page != nullptr);
  B_PLUS_TREE_LEAF_PAGE_TYPE *leaf = reinterpret_cast<B_PLUS_TREE_LEAF_PAGE_TYPE *>(page->GetData());
  //printf("Remove key:%ld\n", key.ToString());
  int size = leaf->RemoveAndDeleteRecord(key, comparator_);
  if (size < leaf->GetMinSize()) CoalesceOrRedistribute(leaf);
  //printf("%s\n", this->ToString(true).c_str());
  buffer_pool_manager_->UnpinPage(page->GetPageId(),true);
  return;
}

/*
 * 函数功能：
 *  对元素个数小于Min_size的结点进行处理
 *      @param N *node 表示内部结点或叶结点
 *      @return: 若有结点合并，返回true，否则返回false
 * 建议：
 *  1.当node为根结点时，需要调用AdjustRoot()进行调整
 *  2.首先要通过调用FindSibling()找到兄弟结点
 *  3.如果两个结点的元素可以合并，则调用Coalesce()进行合并
 *  4.如果元素过多，无法合并，则调用Redistribute()进行结点元素的调整
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
bool BPLUSTREE_TYPE::CoalesceOrRedistribute(N *node) {
  //如果可以合并：进行页的选择和合并
  //如果无法合并，则选择从其他页抽取一个结点，先找兄弟FindSibling
  if (node->IsRootPage()) {
		//该函数可能递归调用至根结点，故需要对根结点进行特殊处理
    return AdjustRoot(node);
  }
  N *sibling;
  // result 为true代表此时node为父页的首结点
  bool result = FindSibling(node, sibling);
  Page *parent = buffer_pool_manager_->FetchPage(node->GetParentPageId());
  B_PLUS_TREE_INTERNAL_PAGE *parentPage = reinterpret_cast<B_PLUS_TREE_INTERNAL_PAGE *>(parent->GetData());
  if (node->GetSize() + sibling->GetSize() <= node->GetMaxSize()) {
    //进行融合
    int removedIndex;
		//进行if判断，保证在父页删除的key为父页的非首结点
    if (result) {
      //此时需要把后继页合并到首结点上
      removedIndex = parentPage->ValueIndex(sibling->GetPageId());
      Coalesce(&node, &sibling, &parentPage, removedIndex);
    } else {
      //当前结点合并到前驱结点上
      removedIndex = parentPage->ValueIndex(node->GetPageId());
      Coalesce(&sibling, &node, &parentPage, removedIndex);
    }

    buffer_pool_manager_->UnpinPage(parentPage->GetPageId(), true);
    return true;
  }
  int nodeIndex = parentPage->ValueIndex(node->GetPageId());
  Redistribute(sibling, node, nodeIndex);  // unpin node,node2
  buffer_pool_manager_->UnpinPage(parentPage->GetPageId(), false);
  return false;
}

/*
 * 函数功能：
 *  对元素个数小于Min_size的根结点进行更新
 *      @return: 若有结点被删除，返回true，否则返回false
 * 建议：
 *  1.根结点可能是叶结点，也可能是内部结点，分开处理
 *  2.当根结点中最后一个元素被删除，此时B+树为空，调用UpdateRootPageId()更新根结点page_id
 *  3.当根结点中仅剩余一个元素，替换更新B+树的根结点
 *  4.注意unpinPage和deletePage
 */
INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::AdjustRoot(BPlusTreePage *old_root_node) {
    if (old_root_node->IsLeafPage()) {
        //此时root中size为0，且树中只有root，故直接删除结点，更新相关数据即可
        buffer_pool_manager_->UnpinPage(old_root_node->GetPageId(), false);
        buffer_pool_manager_->DeletePage(old_root_node->GetPageId());
        root_page_id_ = INVALID_PAGE_ID;
        UpdateRootPageId();
        return true;
    }
    if (old_root_node->GetSize() == 1) {
        //此时root中size为1，即根结点只有一个子页，原根结点可以被替代，故进行根结点的更新（根页作为内部页时最少需2个结点）
        B_PLUS_TREE_INTERNAL_PAGE *root = reinterpret_cast<B_PLUS_TREE_INTERNAL_PAGE *>(old_root_node);
        const page_id_t newRootId = root->RemoveAndReturnOnlyChild();
        root_page_id_ = newRootId;
        UpdateRootPageId();
        Page *page = buffer_pool_manager_->FetchPage(root_page_id_);
        assert(page != nullptr);
        B_PLUS_TREE_INTERNAL_PAGE *newRoot = reinterpret_cast<B_PLUS_TREE_INTERNAL_PAGE *>(page->GetData());
        newRoot->SetParentPageId(INVALID_PAGE_ID);
        buffer_pool_manager_->UnpinPage(root_page_id_, true);
        buffer_pool_manager_->UnpinPage(old_root_node->GetPageId(), false);
        buffer_pool_manager_->DeletePage(old_root_node->GetPageId());
    }
    return false;
}

/*
 * 函数功能：
 *  寻找node结点的兄弟结点sibling
 *      @return: 若兄弟结点为右结点，则返回true，左结点则为false
 * 建议：
 *  1.兄弟结点最好是索引在同一父节点内的结点
 *  2.注意unpinPage，避免缓冲区内存泄露
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
bool BPLUSTREE_TYPE::FindSibling(N *node, N *&sibling) {
  Page *page = buffer_pool_manager_->FetchPage(node->GetParentPageId());
  B_PLUS_TREE_INTERNAL_PAGE *parent = reinterpret_cast<B_PLUS_TREE_INTERNAL_PAGE *>(page->GetData());
  int index = parent->ValueIndex(node->GetPageId());
  int siblingIndex = index - 1;
  //如果是该页的结点是父页的首结点，则从后继页抽一个，如果不是，则从前驱页抽一个。这样可以保证影响范围都是在同一个父页内，操作简单
  if (index == 0) {
    siblingIndex = index + 1;
  }
  page = buffer_pool_manager_->FetchPage(parent->ValueAt(siblingIndex));
  sibling = reinterpret_cast<N *>(page->GetData());
  buffer_pool_manager_->UnpinPage(parent->GetPageId(), false);
  return index == 0;
}
/*
 * 函数功能：
 *  将node中的元素全部合并到neighbor_node中
 *      @param   neighbor_node      幸存结点
 *      @param   node               待合并结点
 *      @param   parent             二者的父结点
 *      @param   index              待合并结点在父结点中的索引
 *      @return: 若有结点被删除，返回true，否则返回false
 * 建议：
 *  1.node合并后需对父结点进行相关调整
 *  2.父结点元素被删除后，若小于Min_size则调用CoalesceOrRedistribute处理
 *  3.注意unpinPage和deletePage
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
bool BPLUSTREE_TYPE::Coalesce(N **neighbor_node, N **node,
                              BPlusTreeInternalPage<KeyType, page_id_t, KeyComparator> **parent, int index) {
  assert((*node)->GetSize() + (*neighbor_node)->GetSize() <= (*node)->GetMaxSize());
  (*node)->MoveAllTo(*neighbor_node,buffer_pool_manager_);
  page_id_t pId = (*node)->GetPageId();
  buffer_pool_manager_->UnpinPage(pId, true);
  buffer_pool_manager_->DeletePage(pId);
  buffer_pool_manager_->UnpinPage((*neighbor_node)->GetPageId(), true);
  (*parent)->Remove(index);
	//当内部页中size过小时或者仅剩余一个有效结点时，递归触发CoalesceOrRedistribute函数，对当前结点进行处理
  if ((*parent)->GetSize() < (*parent)->GetMinSize() || (*parent)->GetSize() == 2) {
    return CoalesceOrRedistribute(*parent);
  }
  return false;
}

/*
 * 函数功能：
 *  将node中的元素移动到neighbor_node中
 * 建议：
 *  1.若index为0，将neighbor_node首元素移动到node尾部
 *  否则将neighbor_node的尾元素移动到node首部
 *  2.注意unpinPage，避免缓冲区内存泄露
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
void BPLUSTREE_TYPE::Redistribute(N *neighbor_node, N *node, int index) {
  if (index == 0) {
    neighbor_node->MoveFirstToEndOf(node, buffer_pool_manager_);
  } else {
    neighbor_node->MoveLastToFrontOf(node, buffer_pool_manager_);
  }
  buffer_pool_manager_->UnpinPage(node->GetPageId(), true);
  buffer_pool_manager_->UnpinPage(neighbor_node->GetPageId(), true);
}

/*
 * 函数功能：
 *  当B+树根结点发生变化时，调用该函数在header_page中对根结点ID进行更新
 *      @param insert_record 缺省时insert_record为0
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::UpdateRootPageId(int insert_record) {
    HeaderPage *header_page = static_cast<HeaderPage *>(buffer_pool_manager_->FetchPage(HEADER_PAGE_ID));
    if (insert_record != 0) {
        // create a new record<index_name + root_page_id> in header_page
        header_page->InsertRecord(index_name_, root_page_id_);
    } else {
        // update root_page_id in header_page
        header_page->UpdateRecord(index_name_, root_page_id_);
    }
    buffer_pool_manager_->UnpinPage(HEADER_PAGE_ID, true);
}

template class BPlusTree<GenericKey<4>, RID, GenericComparator<4>>;
template class BPlusTree<GenericKey<8>, RID, GenericComparator<8>>;
template class BPlusTree<GenericKey<16>, RID, GenericComparator<16>>;
template class BPlusTree<GenericKey<32>, RID, GenericComparator<32>>;
template class BPlusTree<GenericKey<64>, RID, GenericComparator<64>>;

}  // namespace bustub
