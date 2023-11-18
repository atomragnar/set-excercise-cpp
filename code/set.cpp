#include "set.hpp"

#include <algorithm>


std::size_t Set::Node::count_nodes = 0;


std::size_t Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

void Set::insert(Node *p, const int value) {
    Node *newPtr = new Node{value, p->next};
    p->next = newPtr;
    counter++;
}

void Set::remove(Node *p) {
    delete p;
    counter--;
}


Set::Set() : head{new Node{}}, counter{0} {  // create the dummy node
}


Set::Set(int x) : Set() {
    insert(head, x);
}

Set::Set(const std::vector<int> &elements) : Set() {

    for (int i = 0; i < std::ssize(elements); i++) {
        Node *node = head;
        while (true) {
            if (node->next == nullptr) {
                insert(node, elements[i]);
                break;
            }
            if (node->next->value == elements[i]) {
                break;
            }
            if (node->next->value > elements[i]) {
                insert(node, elements[i]);
                break;
            }
            node = node->next;
        }
    }
}


Set::Set(const Set &rhs) : Set() {
    Node *p = head;
    Node *otherPtr = rhs.head->next;
    while (otherPtr != nullptr) {
        insert(p, otherPtr->value);
        p = p->next;
        otherPtr = otherPtr->next;
    }
    counter = rhs.counter;
}


Set &Set::operator=(Set rhs) {
    Set temp(rhs);
    auto swap = [this](Set &other) {
        using std::swap;
        swap(head, other.head);
        swap(counter, other.counter);
    };
    swap(temp);
    return *this;
}


Set::~Set() {
    while (head != nullptr) {
        Node *temp = head;
        head = temp->next;
        remove(temp);
    }
    counter = 0;
}

std::size_t Set::cardinality() const {
    return counter;
}

bool Set::empty() const {
    return head->next == nullptr;
}

bool Set::member(int x) const {
    Node *ptr = head->next;
    while (ptr != nullptr) {
        if (ptr->value == x) {
            return true;
        }
        if (ptr->value > x) {
            return false;
        }
        ptr = ptr->next;
    }
    return false;
}


bool Set::is_subset(const Set &b) const {
    Node *thisPtr = head->next;
    Node *otherPtr = b.head->next;
    while (otherPtr != nullptr && thisPtr != nullptr) {
        if (otherPtr->value == thisPtr->value) {
            otherPtr = otherPtr->next;
            thisPtr = thisPtr->next;
            continue;
        }
        if (otherPtr->value > thisPtr->value) {
            return false;
        }
        otherPtr = otherPtr->next;
        if (otherPtr == nullptr) {
            break;
        }
    }
    if (thisPtr == nullptr) {
        return true;
    }
    return false;
}


Set Set::set_union(const Set &b) const {
    Node *thisPtr = head->next;
    Node *otherPtr = b.head->next;
    Node *unionPtr;
    // creating a set like this instead of new Set{}, is more efficient
    // since this will just be a local var and created on the stack vs the heap.
    Set unionSet = Set{};
    unionPtr = unionSet.head;
    int toInsert;

    while (thisPtr != nullptr || otherPtr != nullptr) {

        if (thisPtr != nullptr && otherPtr != nullptr) {
            if (thisPtr->value == otherPtr->value) {
                toInsert = otherPtr->value;
                thisPtr = thisPtr->next;
                otherPtr = otherPtr->next;
            } else if (thisPtr->value > otherPtr->value) {
                toInsert = otherPtr->value;
                otherPtr = otherPtr->next;
            } else {
                toInsert = thisPtr->value;
                thisPtr = thisPtr->next;
            }
        } else {
            if (thisPtr == nullptr) {
                toInsert = otherPtr->value;
                otherPtr = otherPtr->next;
            } else {
                toInsert = thisPtr->value;
                thisPtr = thisPtr->next;
            }
        }
        unionSet.insert(unionPtr, toInsert);
        unionPtr = unionPtr->next;

    }
    return unionSet;
}


Set Set::set_intersection(const Set &b) const {

    Node *thisPtr = head->next;
    Node *otherPtr = b.head->next;
    Node *intersectionPtr;
    Set intersectionSet = Set{};
    intersectionPtr = intersectionSet.head;

    while (otherPtr != nullptr && thisPtr != nullptr) {
        if (otherPtr->value == thisPtr->value) {
            intersectionSet.insert(intersectionPtr, thisPtr->value);
            intersectionPtr = intersectionPtr->next;
            thisPtr = thisPtr->next;
            otherPtr = otherPtr->next;
        } else if (otherPtr->value > thisPtr->value) {
            thisPtr = thisPtr->next;
        } else {
            otherPtr = otherPtr->next;
        }

    }

    return intersectionSet;
}

Set Set::set_difference(const Set &b) const {
    Node *thisPtr = head->next;
    Node *otherPtr = b.head->next;
    Set diffSet = Set{};
    Node *differencePtr;
    differencePtr = diffSet.head;

    while (thisPtr != nullptr) {
        if (otherPtr != nullptr) {
            if (otherPtr->value == thisPtr->value) {
                otherPtr = otherPtr->next;
                thisPtr = thisPtr->next;
                continue;
            }
            if (thisPtr->value > otherPtr->value) {
                otherPtr = otherPtr->next;
                continue;
            }

        }
        diffSet.insert(differencePtr, thisPtr->value);
        thisPtr = thisPtr->next;
        differencePtr = differencePtr->next;
    }
    return diffSet;
}

std::ostream &operator<<(std::ostream &os, const Set &rhs) {
    if (rhs.empty()) {
        os << "Set is empty!";
    } else {
        Set::Node *ptr = rhs.head->next;
        os << "{ ";

        while (ptr != nullptr) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
    return os;
}



