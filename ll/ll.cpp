#include <cstddef>
#include <iostream>
#include <memory>

template <typename tvalue>
class LinkedList {
   public:
    class LinkedListNode {
       public:
        LinkedListNode(tvalue value) : value_(value), next_(nullptr) {}

        ~LinkedListNode() {
            std::cout << "Destructed node with value " << value_ << "."
                      << std::endl;
        }

        tvalue getValue() const { return value_; }
        std::unique_ptr<LinkedListNode>& getNext() { return next_; }

       private:
        tvalue value_;
        std::unique_ptr<LinkedListNode> next_;
    };
    using Node = LinkedListNode;

    std::size_t getNumberOfNodes() const {
        std::size_t number_of_nodes = 0;
        Node* node = root_.get();
        while (node) {
            ++number_of_nodes;
            node = node->getNext().get();
        }
        return number_of_nodes;
    }

    void appendNode(tvalue value) {
        if (!root_) {
            root_ = std::make_unique<Node>(value);
            return;
        } else {
            Node* node = root_.get();
            while (node) {
                if (!node->getNext()) {
                    node->getNext() = std::make_unique<Node>(value);
                    return;
                }
                node = node->getNext().get();
            }
        }
    }

    void showNodes() const {
        std::size_t node_index = 0;
        Node* node = root_.get();
        while (node) {
            std::cout << "Node with index " << node_index << " has value "
                      << node->getValue() << "." << std::endl;
            node = node->getNext().get();
            ++node_index;
        }
    }

    void addNodeAtPosition(std::size_t position, tvalue value) {
        const std::size_t number_of_nodes = getNumberOfNodes();
        if (position > number_of_nodes) {
            std::cout << "Not enough nodes in the list. There are "
                      << number_of_nodes << " nodes." << std::endl;
        } else if (position == 0) {
            prependToRoot(value);
        } else if (number_of_nodes == position) {
            appendNode(value);
        } else {
            Node* node = root_.get();
            Node* previous_node = node;
            std::size_t current_index = 0;
            while (node) {
                if (current_index == position) {
                    std::unique_ptr<Node> node_to_append =
                        std::move(previous_node->getNext());
                    previous_node->getNext() = std::make_unique<Node>(value);
                    previous_node->getNext()->getNext() =
                        std::move(node_to_append);
                    return;
                }

                previous_node = node;
                node = node->getNext().get();
                ++current_index;
            }
        }
    }

    void prependNode(tvalue value) { addNodeAtPosition(0, value); }

   private:
    void prependToRoot(tvalue value) {
        std::unique_ptr<Node> old_root = std::move(root_);
        root_ = std::make_unique<Node>(value);
        root_->getNext() = std::move(old_root);
    }

    std::unique_ptr<Node> root_;
};

int main(int argc, char** argv) {
    LinkedList<int> ll;
    ll.appendNode(6);
    ll.appendNode(9);
    ll.appendNode(78);
    ll.appendNode(43);
    ll.addNodeAtPosition(3, 200);
    ll.showNodes();
    ll.addNodeAtPosition(2, 23);
    ll.addNodeAtPosition(20, 80);
    ll.prependNode(500);
    ll.showNodes();

    return 0;
}

