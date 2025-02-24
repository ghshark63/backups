#include <cstddef>
#include <sys/select.h>
#include <utility>
template <typename Key, typename Value>
class Map {
  struct BaseNode {
    BaseNode* left;
    BaseNode* right;
    BaseNode* parent;
  };
  struct Node : BaseNode {
    std::pair<const Key, Value> kv;
    bool is_red;
  };
  BaseNode* fake_node_;
  BaseNode* begin_;
  size_t sz_;
};

//there are set, multiset, multimap;
