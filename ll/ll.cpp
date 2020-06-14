#include <cstddef>
#include <exception>
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

        const tvalue& getValue() const { return value_; }
        tvalue& getValue() { return value_; }
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

    void appendNode(const tvalue& value) {
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

    void showAllNodes() const {
        std::size_t node_index = 0;
        Node* node = root_.get();
        while (node) {
            std::cout << "Node with index " << node_index << " has value "
                      << node->getValue() << "." << std::endl;
            node = node->getNext().get();
            ++node_index;
        }
    }

    void addNodeAtPosition(const std::size_t position, const tvalue& value) {
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

    const tvalue& operator[](const std::size_t index) const {
        Node* node = goToNode(index);
        if (!node) {
            throw std::runtime_error("Index out of bounds.");
        }
        return node->getValue();
    }

    tvalue& operator[](const std::size_t index) {
        Node* node = goToNode(index);
        if (!node) {
            throw std::runtime_error("Index out of bounds.");
        }
        return node->getValue();
    }

    void prependNode(const tvalue& value) { addNodeAtPosition(0, value); }

   private:
    void prependToRoot(const tvalue& value) {
        std::unique_ptr<Node> old_root = std::move(root_);
        root_ = std::make_unique<Node>(value);
        root_->getNext() = std::move(old_root);
    }

    Node* goToNode(const std::size_t index) {
        if (index < getNumberOfNodes()) {
            std::size_t current_index = 0;
            Node* node = root_.get();
            while (current_index != index) {
                ++current_index;
                node = node->getNext().get();
            }
            return node;
        }
        return nullptr;
    }

    std::unique_ptr<Node> root_;
};

int main(int argc, char** argv) {
    LinkedList<int> ll;
    std::cout << "Appending nodes with values 6, 9, 78, 43" << std::endl;
    ll.appendNode(6);
    ll.appendNode(9);
    ll.appendNode(78);
    ll.appendNode(43);
    std::cout << "Adding node with value 200 at position 3" << std::endl;
    ll.addNodeAtPosition(3, 200);
    std::cout << "Showing all nodes" << std::endl;
    ll.showAllNodes();
    std::cout << "Adding node with value 23 at position 2" << std::endl;
    ll.addNodeAtPosition(2, 23);
    std::cout << "Adding node with value 80 at position 20" << std::endl;
    ll.addNodeAtPosition(20, 80);
    std::cout << "Prepending node with value 500 to root" << std::endl;
    ll.prependNode(500);
    std::cout << "Showing all nodes" << std::endl;
    ll.showAllNodes();
    int value_at_node_3 = ll[3];
    std::cout << "Retrieving value at node 3 " << value_at_node_3 << std::endl;
    std::cout << "Assigning value 123 at node 3" << std::endl;
    ll[3] = 123;
    value_at_node_3 = ll[3];
    std::cout << "Showing all nodes" << std::endl;
    ll.showAllNodes();

    return 0;
}

