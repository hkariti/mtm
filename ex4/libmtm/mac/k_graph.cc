#include "k_graph.h"
#include "location.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include "exceptions.h"

using std::string;
using mtm::pokemongo::Location;
using std::map;

namespace mtm {

typedef KGraph<string, Location*, 4> KGraphImpl;

template<> class KGraph<string, Location*, 4> {
 private:
  class Node {
    public:
    Node(string const &key, Location* const &value);
    Node(const Node& node);
    ~Node();
    string const& Key() const;
    Location*& Value();
    Location* const& Value() const;
    Node*& operator[](int i);
    const Node* const operator[](int i) const;
   private:
    NodePrivateMembers* private_;
  };

 public:
  class const_iterator;
  class iterator {
   public:
    iterator(Node* node, KGraph* graph);
    iterator(const iterator& it);
    ~iterator();
    iterator& Move(int i);
    string const& operator*() const;
    bool operator==(const iterator& rhs) const;
    bool operator!=(const iterator& rhs) const;
    bool operator==(const const_iterator& rhs) const;
    bool operator!=(const const_iterator& rhs) const;
   private:
    friend class const_iterator;
    IteratorPrivateMembers* private_;
  };

  class const_iterator {
   public:
    const_iterator(const Node* node, const KGraph* graph);
    const_iterator(const const_iterator& it);
    const_iterator(const iterator& it);
    ~const_iterator();
    const_iterator& Move(int i);
    string const& operator*() const;
    bool operator==(const const_iterator& rhs) const;
    bool operator!=(const const_iterator& rhs) const;
   
   private:
    ConstIteratorPrivateMembers* private_;
  };

  explicit KGraph(Location* const& default_value);
  KGraph(const KGraph& k_graph);
  ~KGraph();
  iterator BeginAt(string const& i);
  const_iterator BeginAt(string const& i) const;
  const_iterator End() const;
  void Insert(string const& key, Location* const& value);
  void Insert(string const& key);
  void Remove(string const& key);
  void Remove(const iterator& it);
  Location*& operator[](string const& key);
  Location* const& operator[](string const& key) const;
  bool Contains(string const& key) const;
  void Connect(string const& key_u, string const& key_v, int i_u, int i_v);
  void Connect(string const& key, int i);
  void Disconnect(string const& key_u, string const& key_v);
 
 protected:
  std::map<string, Node*> nodes_;

 private:
  friend class KGraphPrivateMembers;
  friend class NodePrivateMembers;
  friend class IteratorPrivateMembers;
  friend class ConstIteratorPrivateMembers;
  KGraphPrivateMembers* private_;
};

class KGraphPrivateMembers {
 public:
  KGraphPrivateMembers(
      Location* const& default_value, map<string, KGraphImpl::Node*>& nodes) :
      default_value_(default_value), size_(0), nodes_(nodes) {}

  KGraphImpl::Node& GetNode(string const& key) {
    try {
      return *(nodes_.at(key));
    } catch (const std::out_of_range&) {
      throw KGraphKeyNotFoundException();
    }
  }

  const KGraphImpl::Node& GetNode(string const& key) const {
    try {
      return *(nodes_.at(key));
    } catch (const std::out_of_range&) {
      throw KGraphKeyNotFoundException();
    }
  }

  const bool FindConnection(
      string const& key_u, string const& key_v, int* i_u, int* i_v)  const {
    const KGraphImpl::Node& u = GetNode(key_u);
    const KGraphImpl::Node& v = GetNode(key_v);
    for (int i = 0; i < 4; ++i) {
      if (u[i] == &v) {
        *i_u = i;
      }
      if (v[i] == &u) {
        *i_v = i;
      }
    }
  }

  map<string, KGraphImpl::Node*>& nodes_;
  int size_;
  Location* const default_value_;
};

class NodePrivateMembers {
 public:
  NodePrivateMembers(string const& key, Location* const& value) :
      key_(key), value_(value) {
    for (int i = 0; i < 4; ++i) {
      neighbors_[i] = nullptr;
    }
  }

  string key_;
  Location* value_;
  KGraphImpl::Node* neighbors_[4];
};

class IteratorPrivateMembers {
 public:
  IteratorPrivateMembers(KGraphImpl::Node* node, KGraphImpl* graph) :
          current_node_(node), graph_(graph) {}

  KGraphImpl::Node* current_node_;
  KGraphImpl* graph_;
};

class ConstIteratorPrivateMembers {
 public:
  ConstIteratorPrivateMembers(
      const KGraphImpl::Node* node, const KGraphImpl* graph) :
          current_node_(node), graph_(graph) {}
  const KGraphImpl::Node* current_node_;
  const KGraphImpl* graph_;
};


KGraph<string, Location*, 4>::Node::Node(
    string const & key, Location* const& value) :
        private_(new NodePrivateMembers(key, value)) {}

KGraphImpl::Node::Node(const KGraphImpl::Node& node) :
    private_(new NodePrivateMembers(*(node.private_))) {}

KGraphImpl::Node::~Node() {
  delete private_;
}

Location*& KGraphImpl::Node::Value() {
  return private_->value_;
}

Location* const& KGraphImpl::Node::Value() const {
  return private_->value_;
}

string const& KGraphImpl::Node::Key() const {
  return private_->key_;
}

KGraphImpl::Node*& KGraphImpl::Node::operator[](int i) {
  return private_->neighbors_[i];
}

const KGraphImpl::Node* const KGraphImpl::Node::operator[](int i)
    const {
  return private_->neighbors_[i];
}

KGraphImpl::iterator::iterator(Node* node, KGraph* graph) :
    private_(new IteratorPrivateMembers(node, graph)) {}



KGraphImpl::iterator::~iterator() {
  delete private_;
}
    
KGraphImpl::iterator& KGraphImpl::iterator::Move(int i) {
  if (i > 3 || i < 0) {
    throw KGraphEdgeOutOfRange();
  }
  if (!private_->current_node_) {
    throw KGraphIteratorReachedEnd();
  }
  private_->current_node_ = (*(private_->current_node_))[i];
  return *this;
}

KGraphImpl::iterator::iterator(const KGraphImpl::iterator& it) :
    private_(new IteratorPrivateMembers(*(it.private_))) {}

string const& KGraphImpl::iterator::operator*() const {
  if (!private_->current_node_) {
    throw KGraphIteratorReachedEnd();
  }
  return private_->current_node_->Key();
}

bool KGraphImpl::iterator::operator==(
    const KGraphImpl::iterator& rhs) const {
  return (private_->current_node_ == rhs.private_->current_node_) &&
         (private_->graph_ == rhs.private_->graph_);
}

bool KGraphImpl::iterator::operator!=(
    const KGraphImpl::iterator& rhs) const {
  return !(*this == rhs);
}

bool KGraphImpl::iterator::operator==(
    const KGraphImpl::const_iterator& rhs) const {
  return const_iterator(*this) == rhs;
}

bool KGraphImpl::iterator::operator!=(
    const KGraphImpl::const_iterator& rhs) const {
  return const_iterator(*this) != rhs;
}

KGraphImpl::const_iterator::const_iterator(
    const Node* node, const KGraph* graph) :
        private_(new ConstIteratorPrivateMembers(node, graph)) {}

KGraphImpl::const_iterator::const_iterator(
    const KGraphImpl::iterator& it) :
        private_(new ConstIteratorPrivateMembers(
            it.private_->current_node_, it.private_->graph_)) {
}

KGraphImpl::const_iterator::const_iterator(const KGraphImpl::const_iterator& it) :
    private_(new ConstIteratorPrivateMembers(*(it.private_))) {}

KGraphImpl::const_iterator::~const_iterator() {
  delete private_;
}

KGraphImpl::const_iterator& KGraphImpl::const_iterator::Move(int i) {
  if (i > 3 || i < 0) {
    throw KGraphEdgeOutOfRange();
  }
  if (!private_->current_node_) {
    throw KGraphIteratorReachedEnd();
  }
  private_->current_node_ = (*(private_->current_node_))[i];
  return *this;  
}

string const& KGraphImpl::const_iterator::operator*() const {
  if (!private_->current_node_) {
    throw KGraphIteratorReachedEnd();
  }
  return private_->current_node_->Key(); 
}

bool KGraphImpl::const_iterator::operator==(
    const KGraphImpl::const_iterator& rhs) const {
  return private_->current_node_ == rhs.private_->current_node_;
}

bool KGraphImpl::const_iterator::operator!=(
    const KGraphImpl::const_iterator& rhs) const {
  return private_->current_node_ != rhs.private_->current_node_;  
}
    
KGraphImpl::KGraph(Location* const& default_value) :
    nodes_(), private_(new KGraphPrivateMembers(default_value, nodes_)) {}

KGraphImpl::KGraph(const KGraphImpl& k_graph) :
    nodes_(k_graph.nodes_),
    private_(new KGraphPrivateMembers(k_graph.private_->default_value_, nodes_)) {
  // Copy nodes and add to map.
  for (auto it = k_graph.private_->nodes_.begin();
        it != k_graph.private_->nodes_.end(); ++it) {
    private_->nodes_[it->first] =
      new KGraphImpl::Node(it->second->Key(), it->second->Value());
  }
  // Connect edges.
  for (auto it = private_->nodes_.begin();
        it != private_->nodes_.end(); ++it) {
    for (int i = 0; i < 4; ++i) {
      if (k_graph.private_->GetNode(it->first)[i] == nullptr) {
        continue;
      }
      (*(it->second))[i] =
          &(private_->GetNode(
              (k_graph.private_->GetNode(it->first)[i])->Key()));
    }
  }
}

KGraphImpl::~KGraph() {
  for (auto it = private_->nodes_.begin(); it != private_->nodes_.end(); ++it) {
    delete it->second;
  }
  delete private_;
}

KGraphImpl::iterator KGraphImpl::BeginAt(string const& i) {
  return KGraphImpl::iterator(&(private_->GetNode(i)), this);
}

KGraphImpl::const_iterator KGraphImpl::BeginAt(string const& i) const {
  return KGraphImpl::const_iterator(&(private_->GetNode(i)), this);
}

KGraphImpl::const_iterator KGraphImpl::End() const {
  return KGraphImpl::const_iterator(nullptr, this);
}

void KGraphImpl::Insert(string const& key, Location* const& value) {
  if (Contains(key)) {
    throw KGraphKeyAlreadyExistsExpection();
  }
  KGraphImpl::Node* new_node = new KGraphImpl::Node(key, value);
  private_->nodes_[key] = new_node;
  return;
}

void KGraphImpl::Insert(string const& key) {
  Location* const def_ptr = private_->default_value_;
  Insert(key, def_ptr);
}

void KGraphImpl::Remove(string const& key) {
  if (private_->nodes_.erase(key) < 1) {
    throw KGraphKeyNotFoundException();
  }
}

void KGraphImpl::Remove(const KGraphImpl::iterator& it) {
  if (it == End()) {
    throw KGraphIteratorReachedEnd();
  }
  private_->nodes_.erase(private_->nodes_.find(*it));
}

Location*& KGraphImpl::operator[](string const& key) {
  if (!Contains(key)) {
    Insert(key);
  }
  return private_->GetNode(key).Value();
}

Location* const& KGraphImpl::operator[](string const& key) const {
  return private_->GetNode(key).Value();
}

bool KGraphImpl::Contains(string const& key) const {
  try {
    const KGraphImpl::Node& temp = private_->GetNode(key);
  } catch (const KGraphKeyNotFoundException&) {
    return false;
  }
  return true;
}

void KGraphImpl::Connect(
    string const& key_u, string const& key_v, int i_u, int i_v) {
  KGraphImpl::Node& u = private_->GetNode(key_u);
  KGraphImpl::Node& v = private_->GetNode(key_v);
  if (i_u > 3 || i_u < 0 || i_v > 3 || i_v < 0) {
    throw KGraphEdgeOutOfRange();
  }
  int temp_i_u = -1;
  int temp_i_v = -1;
  private_->FindConnection(key_u, key_v, &temp_i_u, &temp_i_v);
  if (temp_i_u != -1 || temp_i_v != -1) {
    throw KGraphNodesAlreadyConnected();
  }
  if (u[i_u] != nullptr || v[i_v] != nullptr) {
    throw KGraphEdgeAlreadyInUse();
  }
  
  u[i_u] = &v;
  v[i_v] = &u;
}

void KGraphImpl::Connect(string const& key, int i) {
  KGraphImpl::Node& u = private_->GetNode(key);
  if (i > 3 || i < 0) {
    throw KGraphEdgeOutOfRange();
  }
  int temp_i = -1;
  private_->FindConnection(key, key, &temp_i, &temp_i);
  if (temp_i != -1 || temp_i != -1) {
    throw KGraphNodesAlreadyConnected();
  }
  if (u[i] != nullptr) {
    throw KGraphEdgeAlreadyInUse();
  }
  
  u[i] = &u;
}

void KGraphImpl::Disconnect(
    string const& key_u, string const& key_v) {
  int temp_i_u = -1;
  int temp_i_v = -1;
  KGraphImpl::Node& u = private_->GetNode(key_u);
  KGraphImpl::Node& v = private_->GetNode(key_v);
  private_->FindConnection(key_u, key_v, &temp_i_u, &temp_i_v);
  if (temp_i_u == -1 || temp_i_v == -1) {
    throw kGraphNodesAreNotConnected();
  }
  u[temp_i_u] = nullptr;
  v[temp_i_v] = nullptr;
}

}  // namespace mtm
