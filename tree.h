#pragma once

#include "header.h"

/* Tree (data structure) support */
class Node
{
    Data* data_;
    Node* father_ = nullptr;
    std::vector<Node*> children_;

public:
    Node(const Data& data);
    ~Node();

    std::vector<Node*>::const_iterator begin() const;
    std::vector<Node*>::const_iterator end() const;

    Data* get() const;
    int degree() const;
    Node* father() const;

    void add(Node* child);

    Node* operator[] (int index);
    const Node* operator[] (int index) const;
};

/* Traversing support */
template <typename Container>
class Traverse
{
protected:
    Node* current_;
    bool running_ = true;
    Container unvisited_;

public:
    virtual ~Traverse() = default;

    typedef Node* data;

    explicit Traverse(Node* root)
        : current_(root)
        , unvisited_ { {root} } {	}

    bool status() const
    {
        return running_;
    }

    virtual void next() = 0;

    data get() const
    {
        return current_;
    }

    data yield()
    {
        try
        {
            next();
            return current_;
        }
        catch (std::out_of_range&)
        {
            running_ = false;
            return nullptr;
        }
    }
};

/*
    Traversing the tree using the BFS algorithm
    Traverse all nodes of the tree using the queue's first-in,
    first-out (FIFO) feature level.
*/
class BFSTraverse : public Traverse<std::queue<Node*>>
{
    using Traverse::Traverse;
    void next() override
    {
        // If queue is empty
        if (this->unvisited_.empty())
            throw std::out_of_range("traverse ended.");

        current_ = unvisited_.front();
        unvisited_.pop();

        // Traversing the child nodes to the queue
        if (!current_->degree())
            return;
        for (auto child : *(current_))
            unvisited_.push(child);
    }
};

/*
    Traversing the tree using the DFS algorithm
    The principle is the same as above, but the stack is LIFO
*/
class DFSTraverse : public Traverse<std::stack<Node*>>
{
    using Traverse::Traverse;
    void next() override
    {
        // If list if empty
        if (this->unvisited_.empty())
            throw std::out_of_range("traverse ended.");

        this->current_ = this->unvisited_.top();
        this->unvisited_.pop();

        // Traversing the child nodes to the queue
        if (!this->current_->degree())
            return;
        for (auto child : *(this->current_))
            this->unvisited_.push(child);
    }
};

/*
    Tree support
    bool exist(Node* target); // Find if specified node in tree
    std::vector<Node*> path(const Node*); // Find path for specified node
*/
class Tree
{
    Node* root_;

public:
    Tree(Node*);
    ~Tree();
    bool exist(const Node*) const;
    std::stack<Node*> search(const Node*) const;
    static std::stack<Node*> path(const Node*);

    // Give an iterator that generates a root node to a leaf node
    class Parser
    {
        bool running_ = true;
        DFSTraverse traverser_;

    public:
        Parser(Node*);
        bool status() const;
        std::stack<Node*> yield();
    };

    Parser leaves() const;
};
