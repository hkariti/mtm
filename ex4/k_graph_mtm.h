#ifndef K_GRAPH_MTM_H
#define K_GRAPH_MTM_H

#include "exceptions.h"
#include <unordered_map>
#include <vector>

namespace mtm {

// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
template<typename KeyType, typename ValueType, int k> class KGraph {
 protected:

  // A node. Represents the basic data unit in a kGraph. Has a key, a value, and
  // connected to at most k other nodes through k edges numbered from 0 to k-1.
  class Node {
	  KeyType key;
	  ValueType value;
	  std::vector<Node*> edges;
    public:
    // Constructs a new node with the given key and value.
    //
    // @param key key of the new node.
    // @param value value of the new node.
	Node(KeyType const &key, ValueType const &value)
			: key(key), value(value), edges(k, NULL) {
	}

    // A destructor.
	~Node() {}

    // Returns the key of the node.
    //
    // @return the key of the node.
	KeyType const& Key() const {
		return key;
	}

    // Returns the value of the node.
    //
    // @return the value of the node.
	ValueType& Value() {
		return value;
	}
	ValueType const& Value() const {
		return value;
	}

    // Returns a reference to the pointer to the neighbor node connected through
    // edge i.
    //
    // @return (reference to) a pointer to the node connected through edge i.
	Node*& operator[](int i) {
		return edges[i];
	}
	const Node* const operator[](int i) const {
		return edges[i];
	}
  };

  std::unordered_map<KeyType, Node> nodes;
  ValueType default_value;

 public:
  class const_iterator;  // forward declaration
  
  // An iterator. Used to iterate over the data in a kGraph. At every given 
  // moment, the iterator points either to one of the nodes in the graph, or to
  // the end of the graph.
  class iterator {
	  friend const_iterator;

	Node* node;
	KGraph* graph;

  public:
	// Constructs a new iterator that points to a given node in the given graph.
	//
	// @param node the node the new iterator points to.
	// @param graph the kGraph over which the iterator iterates.
	iterator(Node* node, KGraph* graph)
		: node(node), graph(graph)
	{}

	// A copy constructor.
	//
	// @param it the iterator to copy.
	iterator(const iterator& it)
		: node(it.node), graph(it.graph)
	{}

	// A destructor.
	~iterator() {}

	// Moves the iterator to point to the node that is connected to the current
	// node through edge i.
	//
	// @param i the edge over which to move.
	// @return a reference to *this (the same iterator) after moving it.
	// @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
	// @throw KGraphIteratorReachedEnd when trying to move an iterator that
	//        points to the end of the graph.
	iterator& Move(int i) {
		if (i < 0 || i >= k) throw KGraphEdgeOutOfRange();
		if (NULL == node) throw KGraphIteratorReachedEnd();
		node = (*node)[i];
		return *this;
	}

	// Dereferne operator. Return the key of the node pointed by the iterator.
	//
	// @return the key of the node to which the iterator points.
	// @throw KGraphIteratorReachedEnd when trying to dereference an iterator
	//        that points to the end of the graph.
	KeyType const& operator*() const {
		if (NULL == node) throw KGraphIteratorReachedEnd();
		return node->Key();
	}

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
	bool operator==(const iterator& rhs) const {
		return node == rhs.node && graph == rhs.graph;
	}

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
	bool operator!=(const iterator& rhs) const {
		return !(*this == rhs);
	}

    // Equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
	bool operator==(const const_iterator& rhs) const {
		return node == rhs.node && graph == rhs.graph;
	}

    // Not equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
	bool operator!=(const const_iterator& rhs) const {
		return !(*this == rhs);
	}
  };

  // A const iterator. Used to iterate over the data in a constant kGraph.
  // Similarly to a regular iterator, at every given moment, the iterator points
  // either to one of the nodes in the graph, or to the end of the graph.
  class const_iterator {
	  friend iterator;

	const Node* node;
	const KGraph* graph;

   public:
    // Constructs a new const iterator that points to a given node in the given
    // graph.
    //
    // @param node the node the new iterator points to.
    // @param graph the kGraph over which the iterator iterates.
	const_iterator(const Node* node, const KGraph* graph)
		   : node(node), graph(graph)
	{}

    // A copy constructor.
    //
    // @param it the iterator to copy.
	const_iterator(const const_iterator& it)
		: node(it.node), graph(it.graph)
	{}

    // Conversion from a regular iterator. Constucts a new const iterator that
    // points to the same node as the given iterator.
    //
    // @param it the iterator we would like to convert to const iterator.
    const_iterator(const iterator& it)
		: node(it.node), graph(it.graph)
	{}

    // A destructor.
	~const_iterator() {}

    // Moves the iterator to point to the node that is connected to the current
    // node through edge i.
    //
    // @param i the edge over which to move.
    // @return a reference to *this (the same iterator) after moving it.
    // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
    // @throw KGraphIteratorReachedEnd when trying to move an iterator that
    //        points to the end of the graph.
	const_iterator& Move(int i) {
		if (i < 0 || i >= k) throw KGraphEdgeOutOfRange();
		if (NULL == node) throw KGraphIteratorReachedEnd();
		node = (*node)[i];
		return *this;
	}

    // Dereferne operator. Return the key of the node pointed by the iterator.
    //
    // @return the key of the node to which the iterator points.
    // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
    //        that points to the end of the graph.
	KeyType const& operator*() const {
		if (NULL == node) throw KGraphIteratorReachedEnd();
		return node->Key();
	}

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
	bool operator==(const const_iterator& rhs) const {
		return node == rhs.node && graph == rhs.graph;
	}

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
	bool operator!=(const const_iterator& rhs) const {
		return !(*this == rhs);
	}
  };

 public:
  // Constructs a new empty kGraph with the given default value.
  //
  // @param default_value the default value in the graph.
  explicit KGraph(ValueType const& default_value)
	  : default_value(default_value), nodes()
  {}

  // A copy constructor. Copies the given graph. The constructed graph will have
  // the exact same structure with copied data.
  //
  // @param k_graph the graph to copy.
  KGraph(const KGraph& k_graph)
	  : default_value(k_graph.default_value), nodes(k_graph.nodes)
  {
	  typename std::unordered_map<KeyType, Node>::iterator it;
	  for (it = nodes.begin(); it != nodes.end(); it++) {
		  Node* new_node = &(*it).second;
		  const Node* original_node = &k_graph.nodes.at(new_node->Key());
		  for (int i = 0; i < k; i++) {
			  if ((*new_node)[i] != NULL) {
				  (*new_node)[i] = &nodes.at((*original_node)[i]->Key());
			  }
		  }
	  }
  }

  // A destructor. Destroys the graph together with all resources allocated.
  ~KGraph() {}

  // Returns an iterator to the node with the given key.
  //
  // @param i the key of the node which the returned iterator points to.
  // @return iterator the newly constructed iterator.
  // @throw KGraphKeyNotFoundException when the given key is not found in the
  //        graph.
  iterator BeginAt(KeyType const& i) {
	  if (!Contains(i)) throw KGraphKeyNotFoundException();
	  return iterator(&nodes.at(i), this); //TODO: check if get's destroy after functions end
  }
  const_iterator BeginAt(KeyType const& i) const {
	  if (!Contains(i)) throw KGraphKeyNotFoundException();
	  return const_iterator(&nodes.at(i), this); //TODO: check if get's destroy after functions end
  }

  // Returns an iterator to the end of the graph.
  //
  // @return iterator an iterator to the end of the graph.
  const_iterator End() const {
	  return const_iterator(NULL, this);
  }

  // Inserts a new node with the given data to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @param value the value to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key, ValueType const& value) {
	  if (Contains(key)) {
		  throw KGraphKeyAlreadyExistsExpection();
	  }
	  nodes.insert({ key, Node(key, value) });
  }

  // Inserts a new node with the given key and the default value to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key) {
	  if (Contains(key)) {
		  throw KGraphKeyAlreadyExistsExpection();
	  }
	  nodes.insert({ key, Node(key, default_value) });
  }

  // Removes the node with the given key from the graph.
  //
  // @param key the key of the node to be removed.
  // @throw KGraphKeyNotFoundException when trying to remove a key that cannot
  //        be found in the graph.
  void Remove(KeyType const& key) {
	  if (!Contains(key)) throw KGraphKeyNotFoundException();
	  for (int i = 0; i < k; i++) {
		  if (nodes.at(key)[i] != NULL) {
			  Disconnect(nodes.at(key).Key(), (*nodes.at(key)[i]).Key());
		  }
	  }
	  nodes.erase(key);
  }

  // Removes the node pointed by the given iterator from the graph. If the
  // given iterator neither points to a node in this graph nor to the end of
  // this graph, the behaviour of this function is undefined.
  //
  // @param it the iterator that points to the node to be removed.
  // @throw KGraphIteratorReachedEnd when the given iterator points to the end
  //        of the graph.
  void Remove(const iterator& it) {
	  if (it == End()) throw KGraphIteratorReachedEnd();
	  for (int i = 0; i < k; i++) {
		  if (nodes.at(*it)[i] != NULL) {
			  Disconnect(nodes.at(*it).Key(), (*nodes.at(*it)[i]).Key());
		  }
	  }
	  nodes.erase(*it);
	  // TODO: what happens to iterator after it's node is erased?!
  }

  // The subscript operator. Returns a reference to the value assigned to
  // the given key in the graph. If the key does not exist, inserts a new node
  // to the graph with the given key and the default value, then returns a
  // refernce to its value.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  ValueType& operator[](KeyType const& key) {
	  if (!Contains(key)) {
		  Insert(key);
	  }
	  return nodes.at(key).Value();
  }

  // A const version of the subscript operator. Returns the value assigned to
  // the given key in the graph. If the key does not exist, throws an exception.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  // @throw KGraphKeyNotFoundException if the given key cannot be found in the 
  //        graph.
  ValueType const& operator[](KeyType const& key) const {
	  if (!Contains(key)) throw KGraphKeyNotFoundException();
	  return nodes.at(key).Value();
  }

  // Checks whether the graph contains the given key.
  //
  // @param key
  // @return true iff the graph contains the given key.
  bool Contains(KeyType const& key) const {
	  return nodes.find(key) != nodes.end();
  }

  // Connects two nodes in the graph with an edge.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @param i_u the index of the new edge at the first node.
  // @param i_v the index of the new edge at the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1].
  // @throw KGraphNodesAlreadyConnected if the two nodes are already connected.
  // @throw KGraphEdgeAlreadyInUse if at least one of the indices of the edge at
  //        one of the nodes is already in use.
  void Connect(KeyType const& key_u, KeyType const& key_v, int i_u, int i_v) {
	  if (nodes.find(key_u) == nodes.end() ||
		  nodes.find(key_v) == nodes.end()) {
		  throw KGraphKeyNotFoundException();
	  }
	  if (i_u < 0 || i_u >= k ||
		  i_v < 0 || i_v >= k) {
		  throw KGraphEdgeOutOfRange();
	  }
	  for (int i = 0; i < k; i++) {
		  if (nodes.at(key_u)[i] == &nodes.at(key_v) ||
			  nodes.at(key_v)[i] == &nodes.at(key_u)) {
			  throw KGraphNodesAlreadyConnected();
		  }
	  }
	  if (nodes.at(key_u)[i_u] != NULL ||
		  nodes.at(key_v)[i_v] != NULL) {
		  throw KGraphEdgeAlreadyInUse();
	  }
	  nodes.at(key_v)[i_v] = &nodes.at(key_u);
	  nodes.at(key_u)[i_u] = &nodes.at(key_v);
  }

  // Connects a node to itself via a self loop.
  //
  // @param key the key of the node.
  // @param i the index of the self loop.
  // @throw KGraphKeyNotFoundException if the given keys cannot be found in the
  //        graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
  // @throw KGraphNodesAlreadyConnected if the node is already self connected.
  // @throw KGraphEdgeAlreadyInUse if the index of the self loop is already in
  //        use.
  void Connect(KeyType const& key, int i) {
	  if (nodes.find(key) == nodes.end()) {
		  throw KGraphKeyNotFoundException();
	  }
	  if (i < 0 || i >= k) {
		  throw KGraphEdgeOutOfRange();
	  }
	  for (int i = 0; i < k; i++) {
		  if (nodes.at(key)[i] == &nodes.at(key)) {
			  throw KGraphNodesAlreadyConnected();
		  }
	  }
	  if (nodes.at(key)[i] != NULL) {
		  throw KGraphEdgeAlreadyInUse();
	  }
	  nodes.at(key)[i] = &nodes.at(key);
  }

  // Disconnects two connected nodes.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw kGraphNodesAreNotConnected if the two nodes are not connected.
  void Disconnect(KeyType const& key_u, KeyType const& key_v) {
	  if (nodes.find(key_u) == nodes.end() ||
		  nodes.find(key_v) == nodes.end()) {
		  throw KGraphKeyNotFoundException();
	  }
	  for (int i_u = 0; i_u < k; i_u++) {
		  if (nodes.at(key_u)[i_u] == &nodes.at(key_v)) {
			  for (int i_v = 0; i_v < k; i_v++) {
				  if (nodes.at(key_v)[i_v] == &nodes.at(key_u)) {
					  nodes.at(key_v)[i_v] = NULL;
					  nodes.at(key_u)[i_u] = NULL;
					  return;
				  }
			  }
			  break;
		  }
	  }
	  throw kGraphNodesAreNotConnected();
  }
};

}  // namespace mtm

#endif  // K_GRAPH_MTM_H
