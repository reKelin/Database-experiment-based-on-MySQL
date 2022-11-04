#include "storage/page/b_plus_tree_page.h"

namespace bustub {

/*
 * 函数功能：
 *  判断页类型是否为叶子结点
 * 建议：
 *  enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE }
 *  （定义于b_plus_tree_page.h头文件中）
 */
bool BPlusTreePage::IsLeafPage() const {
  return page_type_ == IndexPageType::LEAF_PAGE;
}
/*
 * 函数功能：
 *  判断页类型是否为根结点
 * 建议：
 *  static constexpr int INVALID_PAGE_ID = -1;// invalid page id（定义与config.h中）
 *  父节点pageId为INVALID_PAGE_ID时即为根结点
 */
bool BPlusTreePage::IsRootPage() const {
  return parent_page_id_ == INVALID_PAGE_ID;
}
void BPlusTreePage::SetPageType(IndexPageType page_type) { page_type_ = page_type; }

/*
 * 函数功能：
 *  get/set size (size：当前结点中存放的元素个数)
 */
int BPlusTreePage::GetSize() const { return size_; }
void BPlusTreePage::SetSize(int size) {
  size_ = size;
  return;
}
void BPlusTreePage::IncreaseSize(int amount) {
  size_ += amount;
}

/*
 * 函数功能：
 *  get/set max size
 */
int BPlusTreePage::GetMaxSize() const { return max_size_; }
void BPlusTreePage::SetMaxSize(int size) {
  max_size_ = size;
}

/*
 * 函数功能：
 *  获取当前结点允许的最少元素个数
 * 建议：
 *  1.如果此时为根节点：根节点可能是内部节点也可能是叶子节点
 *  内部节点：此时至少存在两个索引
 *  叶子节点：此时至少存在一条记录
 *  2.非根结点正常处理
 */
int BPlusTreePage::GetMinSize() const {
  if (IsRootPage()) {
    return IsLeafPage() ? 1 : 2 ;
  }
  return (max_size_ + 1 )/ 2;
}

/*
 *函数功能：
 *  get/set parent page id
 */
page_id_t BPlusTreePage::GetParentPageId() const { return parent_page_id_; }
void BPlusTreePage::SetParentPageId(page_id_t parent_page_id) { parent_page_id_ = parent_page_id; }

/*
 *函数功能：
 *  get/set self page id
 */
page_id_t BPlusTreePage::GetPageId() const { return page_id_; }
void BPlusTreePage::SetPageId(page_id_t page_id) { page_id_ = page_id; }

/*
 *函数功能：
 * set lsn
 */
void BPlusTreePage::SetLSN(lsn_t lsn) { lsn_ = lsn; }

}
