
#include "tree.h"
#include "data.h"

Node::Node(const Data& data) : data_(new Data(data)) {}

Node::~Node() {
	delete data_;
	for (auto& child : children_)
		delete child;
}

std::vector<Node*>::const_iterator Node::begin() const {
	return children_.begin();
}

std::vector<Node*>::const_iterator Node::end() const {
	return children_.end();
}

// Add from initialized node pointer
void Node::add(Node*& child) {
	child->father_ = this;
	children_.push_back(child);
}

// Remove child elements from the specified location
void Node::remove(const int index) {
	this->children_.erase(children_.begin() + index);
}

// Add from a Data copy
void Node::add(const Data& data) {
	Node* child = new Node(data);
	child->father_ = this;
	children_.push_back(child);
}

// Return data_ of current node
Data* Node::get() const {
	return data_;
}

// Return degree of current node
int Node::degree() const {
	return children_.size();
}

// Return father node for current node
Node* Node::father() const {
	return father_;
}

// Access support by index
Node* Node::operator[] (const int index) {
	return children_[index];
}

const Node* Node::operator[] (const int index) const {
	return children_[index];
}

/* ----------------------------------------------------- */

Tree::Tree(Node* root) : root_(root) {}

Tree::~Tree() {
	delete root_;
}

// Find if specified node in tree
bool Tree::exist(const Node* target) const {
	BFSTraverse traverser(root_);

	while (traverser.status()) {
		Node* current = traverser.yield();
		if (current == target)
			return true;
	}

	return false;
}

std::stack<Node*> Tree::search(const Node* node) const {
	if (exist(node)) {
		return path(node);
	}
  return std::stack<Node*>();
}

std::stack<Node*> Tree::path(const Node* node) {
	std::stack<Node*> path;
	Node* current = const_cast<Node*>(node);
	while (true) {
		path.push(current);
		current = current->father();
		if (current == nullptr) break;
	}
	return path;
}

// Give an iterator that generates a root node to a leaf node
Tree::Parser::Parser(Node* root) : traverser_(DFSTraverse(root)) { }
bool Tree::Parser::status() const { return running_; }

std::stack<Node*> Tree::Parser::yield() {
	while (traverser_.status()) {
		Node* current = traverser_.yield();
		if (current == nullptr) break;

		// If current has no child - it is a leaf node
		if (!current->degree())
			return path(current);
	}

	// If all paths has been traversed, return empty stack
	running_ = false;
	return std::stack<Node*>();
}

Tree::Parser Tree::leaves() const{
	return Parser(root_);
}
