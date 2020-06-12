#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>

template <typename tkey, typename tvalue>
class BinarySearchTree {
   public:
    class BinarySearchTreeNode {
       public:
        BinarySearchTreeNode(tkey key, tvalue value)
            : key_(key), value_(value), left_(nullptr), right_(nullptr) {}

        ~BinarySearchTreeNode() {
            std::cout << "Destructed node with key " << key_ << " "
                      << " value " << value_ << std::endl;
        }

        tkey getKey() { return key_; }
        tvalue getValue() { return value_; }
        std::unique_ptr<BinarySearchTreeNode>& getLeft() { return left_; }
        std::unique_ptr<BinarySearchTreeNode>& getRight() { return right_; }

       private:
        tkey key_;
        tvalue value_;
        std::unique_ptr<BinarySearchTreeNode> left_;
        std::unique_ptr<BinarySearchTreeNode> right_;
    };
    using Node = BinarySearchTreeNode;

   public:
    BinarySearchTree() : root_(nullptr) {}

    BinarySearchTree(std::initializer_list<std::pair<tkey, tvalue>> init_list)
        : root_(nullptr) {
        const std::pair<tkey, tvalue>* it = init_list.begin();
        const std::pair<tkey, tvalue>* const end = init_list.end();
        for (; it != end; ++it) {
            addNode(it->first, it->second);
        }
    }

    void addNode(tkey key, tvalue value) {
        if (!root_) {
            root_ = std::make_unique<Node>(key, value);
            return;
        }

        addNode(key, value, root_.get());
    }

    Node* searchNode(tkey key) {
        if (!root_) {
            std::cout << "Tree empty" << std::endl;
            return nullptr;
        }

        return searchNode(key, root_.get());
    }

    void showNode(Node* node) const {
        if (node) {
            std::cout << "Node with key " << node->getKey() << " and value "
                      << node->getValue() << std::endl;
        } else {
            std::cout << "Node empty." << std::endl;
        }
    }

    void printPreOrder() {
        if (!root_) {
            std::cout << "Tree empty." << std::endl;
            return;
        }
        printPreOrder(root_.get());
    }

    void printInOrder() {
        if (!root_) {
            std::cout << "Tree empty." << std::endl;
            return;
        }
        printInOrder(root_.get());
    }

   private:
    void addNode(tkey key, tvalue value, Node* node) {
        if (key > node->getKey()) {
            if (!node->getRight()) {
                node->getRight() = std::make_unique<Node>(key, value);
            } else {
                addNode(key, value, node->getRight().get());
            }
        } else {
            if (!node->getLeft()) {
                node->getLeft() = std::make_unique<Node>(key, value);
            } else {
                addNode(key, value, node->getLeft().get());
            }
        }
    }

    Node* searchNode(tkey key, Node* node) {
        if (node) {
            if (key == node->getKey()) {
                return node;
            } else if (key > node->getKey()) {
                if (node->getRight()) {
                    return searchNode(key, node->getRight().get());
                }
            } else {
                if (node->getLeft()) {
                    return searchNode(key, node->getLeft().get());
                }
            }
        } else {
            return nullptr;
        }
    }

    void printPreOrder(Node* node) {
        if (node) {
            std::cout << "Key -> " << node->getKey() << " Value -> "
                      << node->getValue() << std::endl;
            printPreOrder(node->getLeft().get());
            printPreOrder(node->getRight().get());
        }
    }

    void printInOrder(Node* node) {
        if (node) {
            printInOrder(node->getLeft().get());
            std::cout << "Key -> " << node->getKey() << " Value -> "
                      << node->getValue() << std::endl;
            printInOrder(node->getRight().get());
        }
    }

    std::unique_ptr<Node> root_;
};

int main(int argc, char** argv) {
    BinarySearchTree<int, int> bst;
    bst.addNode(1, 21);
    bst.addNode(5, 22);
    bst.addNode(3, 23);
    bst.addNode(30, 24);
    bst.addNode(7, 25);
    bst.addNode(17, 26);
    bst.addNode(800, 27);
    bst.addNode(50, 28);
    bst.printInOrder();
    BinarySearchTree<int, int>::Node* node = bst.searchNode(7);
    bst.showNode(node);

    // Initializer list
    BinarySearchTree<int, int> list_initialized_bst = {
        {1, 2}, {4, 3}, {5, 6}, {7, 8}};
    list_initialized_bst.printInOrder();

    return 0;
}
