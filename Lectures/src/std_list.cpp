#include <cstddef>

template <typename T>
class List {
  struct BaseNode {
    BaseNode* next;
    BaseNode* prev;
  };
  struct Node : BaseNode {
    T value;
  };
  BaseNode fake_node_;
  size_t sz_;
public:
  List() 
    : fake_node_{&fake_node_, &fake_node_},
      sz_(0) 
  {}

  //sort()
  //merge()
  //splice()

};

//forward_list
