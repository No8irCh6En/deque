#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP
#define DEFAULT_CAPACITY 128
#include "exceptions.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
namespace sjtu {
template <class T>
class double_list {
   public:
    class Node {
       public:
        T* val_ptr;
        Node* prev;
        Node* next;
        Node(T* val_ptr = nullptr, Node* prev = nullptr, Node* next = nullptr)
            : val_ptr(val_ptr), prev(prev), next(next) {}
        ~Node() {
            prev = next = nullptr;
            if (val_ptr)
                delete val_ptr;
            val_ptr = nullptr;
        }
    };
    Node* end_ptr;
    Node end_node;
    Node* head;
    size_t size;
    // --------------------------

    double_list() : size(0), end_node() { head = end_ptr = &end_node; }
    double_list(const double_list<T>& other) : size(other.size), end_node() {
        head = end_ptr = &end_node;
        *this = other;
    }
    ~double_list() { this->clear(); }

    double_list& operator=(const double_list& other) {
        if (this == &other)
            return *this;
        clear();
        Node* _ptr = other.head;
        while (_ptr != other.end_ptr) {
            insert_tail(*(_ptr->val_ptr));
            _ptr = _ptr->next;
        }
        return *this;
    }
    T& back() {
        if (size == 0)
            throw std::runtime_error("double_list.back: invalid back");
        return *(end_ptr->prev->val_ptr);
    }
    T& front() {
        if (size == 0)
            throw std::runtime_error("double_list.front: invalid front");
        return *(head->val_ptr);
    }
    const T& back() const {
        if (size == 0)
            throw std::runtime_error("double_list.back: invalid back");
        return *(end_ptr->prev->val_ptr);
    }
    const T& front() const {
        if (size == 0)
            throw std::runtime_error("double_list.front: invalid front");
        return *(head->val_ptr);
    }
    class iterator {
       public:
        Node* ptr;
        // --------------------------

        iterator(Node* ptr = nullptr) : ptr(ptr) {}
        iterator(const iterator& t) { ptr = t.ptr; }
        ~iterator() {}

        iterator operator++(int) {
            iterator to_return(ptr);
            if (ptr && ptr->val_ptr) {
                ptr = ptr->next;
                return to_return;
            }
            throw std::runtime_error("index out of bound");
        }

        iterator& operator++() {
            if (ptr && ptr->val_ptr) {
                ptr = ptr->next;
                return *this;
            }
            throw std::runtime_error("index out of bound");
        }

        iterator operator--(int) {
            iterator to_return(ptr);
            if (ptr && ptr->prev) {
                ptr = ptr->prev;
                return to_return;
            }
            throw std::runtime_error("index out of bound");
        }

        iterator& operator--() {
            if (ptr && ptr->prev) {
                ptr = ptr->prev;
                return *this;
            }
            throw std::runtime_error("index out of bound");
        }
        /**
         * if the iter didn't point to a value
         * throw " invalid"
         */
        T& operator*() const {
            if (ptr && ptr->val_ptr) {
                return *(ptr->val_ptr);
            }
            throw std::runtime_error("119:T& operator*()");
        }
        /**
         * other operation
         */
        T* operator->() const noexcept { return ptr->val_ptr; }
        bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
        bool operator!=(const iterator& rhs) const { return ptr != rhs.ptr; }
    };
    /**
     * return an iterator to the beginning
     */
    iterator begin() const { return iterator(head); }
    /**
     * return an iterator to the ending
     * in fact, it returns the iterator point to nothing,
     * just after the last element.
     */
    iterator end() const { return iterator(end_ptr); }
    /**
     * if the iter didn't point to anything, do nothing,
     * otherwise, delete the element pointed by the iter
     * and return the iterator point at the same "index"
     * e.g.
     * 	if the origin iterator point at the 2nd element
     * 	the returned iterator also point at the
     *  2nd element of the list after the operation
     *  or nothing if the list after the operation
     *  don't contain 2nd elememt.
     */
    iterator erase(iterator pos) {
        if (pos == iterator() || pos == end())
            throw std::runtime_error("erase function: pointing to nothing");
        if (pos == begin()) {
            delete_head();
            return begin();
        }
        Node *to_delete = pos.ptr, *to_return = pos.ptr->next;
        (to_delete->prev)->next = to_delete->next;
        (to_delete->next)->prev = to_delete->prev;
        delete to_delete;
        size--;
        return iterator(to_return);
    }

    void insert_head(const T& val) {
        Node* node_ptr = new Node(new T(val));
        if (head == end_ptr) {
            head = node_ptr;
            end_ptr->prev = head;
            node_ptr->next = end_ptr;
        } else {
            node_ptr->next = head;
            head->prev = node_ptr;
            head = node_ptr;
        }
        size++;
    }
    void insert_tail(const T& val) {
        Node* node_ptr = new Node(new T(val));
        if (end_ptr != head) {
            end_ptr->prev->next = node_ptr;
            node_ptr->prev = end_ptr->prev;
            node_ptr->next = end_ptr;
            end_ptr->prev = node_ptr;
        } else {
            head = node_ptr;
            head->next = end_ptr;
            end_ptr->prev = head;
        }
        size++;
    }
    void delete_head() {
        if (head == end_ptr)
            return;
        Node* to_delete = head;
        head = head->next;
        head->prev = nullptr;
        delete to_delete;
        size--;
    }
    void delete_tail() {
        if (head == end_ptr)
            return;
        Node* to_delete = end_ptr->prev;
        if (to_delete->prev)
            to_delete->prev->next = end_ptr;
        else
            head = end_ptr;
        end_ptr->prev = to_delete->prev;
        delete to_delete;
        size--;
    }
    void clear() {
        Node* current = head;
        Node* to_delete = current;
        while (current != end_ptr) {
            to_delete = current;
            current = current->next;
            delete to_delete;
        }
        head = end_ptr = &end_node;
        size = 0;
    }
    /**
     * if didn't contain anything, return true,
     * otherwise false.
     */
    bool empty() { return !size; }
    //-----------------------------
    // newly add
    T& at(const size_t& pos) {
        if (pos >= size)
            throw std::runtime_error("at function: index_out_of_bound");
        Node* _ptr = head;
        for (int i = 1; i <= pos; i++) {
            _ptr = _ptr->next;
        }
        return *(_ptr->val_ptr);
    }
    const T& at(const size_t& pos) const {
        if (pos >= size)
            throw std::runtime_error("at function: index_out_of_bound");
        Node* _ptr = head;
        for (int i = 1; i <= pos; i++) {
            _ptr = _ptr->next;
        }
        return *(_ptr->val_ptr);
    }
    T& operator[](const size_t& pos) { return at(pos); }
    const T& operator[](const size_t& pos) const { return at(pos); }

    iterator insert(iterator pos, const T& value) {
        if (pos == iterator())
            throw std::runtime_error("insert function: invalid iterator");
        Node* new_ptr = new Node(new T(value));
        Node* ori_ptr = pos.ptr;
        if (!ori_ptr->prev)
            head = new_ptr;
        else
            ori_ptr->prev->next = new_ptr;
        new_ptr->next = ori_ptr;
        new_ptr->prev = ori_ptr->prev;
        ori_ptr->prev = new_ptr;
        size++;
        return iterator(new_ptr);
    }
    //--------------------------------
};
template <class T>
class deque {
   public:
    using list_Node = typename double_list<double_list<T>>::Node;
    using Node = typename double_list<T>::Node;
    using list_iterator = typename double_list<double_list<T>>::iterator;
    using Node_iterator = typename double_list<T>::iterator;

    double_list<double_list<T>> list;
    size_t total_size;

   public:
    class const_iterator;
    class iterator {
       public:
        /**
         * add data members.
         * just add whatever you want.
         */

        list_Node* list_ptr;
        Node* node_ptr;
        // to check if two iterator points to the same list
        deque* check_ptr;

       public:
        int get_step() const {
            Node* ptr1 = this->node_ptr;
            list_Node* ptr2 = this->list_ptr;
            int step = 0;
            if (ptr2->next) {
                while (ptr1) {
                    ptr1 = ptr1->prev;
                    ++step;
                }
                while (ptr2 && ptr2->prev) {
                    ptr2 = ptr2->prev;
                    step += ptr2->val_ptr->size;
                }
            }
            return step == 0 ? check_ptr->total_size : step - 1;
        }

        iterator quick_move(int step) const {
            if (step == 0) {
                return check_ptr->begin();
            }
            if (step == check_ptr->total_size) {
                return check_ptr->end();
            }
            size_t _remain = step;
            size_t step_inner;
            list_Node* l_ptr = check_ptr->list.head;
            for (int i = 0; i < check_ptr->list.size; i++) {
                if (_remain >= l_ptr->val_ptr->size) {
                    _remain -= l_ptr->val_ptr->size;
                    l_ptr = l_ptr->next;
                } else {
                    step_inner = _remain;
                    break;
                }
            }
            if (!l_ptr->val_ptr)
                throw std::runtime_error("quick move: out of bound");
            Node* n_ptr = l_ptr->val_ptr->head;
            for (int i = 0; i < step_inner; i++) {
                if (!n_ptr)
                    throw std::runtime_error("quick move: out of bound");
                n_ptr = n_ptr->next;
            }
            return iterator(l_ptr, n_ptr, check_ptr);
        }
        /**
         * return a new iterator which points to the n-next element.
         * if there are not enough elements, the behaviour is undefined.
         * same for operator-.
         */
        iterator(list_Node* ptr1 = nullptr,
                 Node* ptr2 = nullptr,
                 deque* ptr3 = nullptr)
            : list_ptr(ptr1), node_ptr(ptr2), check_ptr(ptr3) {}
        iterator operator+(const int& n) const {
            int step = get_step() + n;
            return quick_move(step);
        }
        iterator operator-(const int& n) const {
            int step = get_step() - n;
            return quick_move(step);
        }

        /**
         * return the distance between two iterators.
         * if they point to different vectors, throw
         * invaild_iterator.
         */
        int operator-(const iterator& rhs) const {
            list_Node *_ptr1 = this->list_ptr, *_ptr2 = rhs.list_ptr;
            if (check_ptr != rhs.check_ptr)
                throw std::runtime_error(
                    "distance function: not the same list");
            int step = get_step() - rhs.get_step();
            return step;
        }
        iterator& operator+=(const int& n) {
            int step = get_step() + n;
            auto iter = quick_move(step);
            *this = iter;
            return *this;
        }
        iterator& operator-=(const int& n) {
            int step = get_step() - n;
            auto iter = quick_move(step);
            *this = iter;
            return *this;
        }

        /**
         * iter++
         */
        iterator operator++(int) {
            if (node_ptr && node_ptr->next && node_ptr->next->val_ptr) {
                auto iter = iterator(list_ptr, node_ptr, check_ptr);
                node_ptr = node_ptr->next;
                return iter;
            } else if (list_ptr && list_ptr->next) {
                auto iter = iterator(list_ptr, node_ptr, check_ptr);
                list_ptr = list_ptr->next;
                node_ptr =
                    list_ptr->val_ptr ? list_ptr->val_ptr->head : nullptr;
                return iter;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * ++iter
         */
        iterator& operator++() {
            if (node_ptr && node_ptr->next && node_ptr->next->val_ptr) {
                node_ptr = node_ptr->next;
                return *this;
            } else if (list_ptr && list_ptr->next) {
                list_ptr = list_ptr->next;
                node_ptr =
                    list_ptr->val_ptr ? list_ptr->val_ptr->head : nullptr;
                return *this;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (node_ptr && node_ptr->prev) {
                auto iter = iterator(list_ptr, node_ptr, check_ptr);
                node_ptr = node_ptr->prev;
                return iter;
            } else if (list_ptr && list_ptr->prev) {
                auto iter = iterator(list_ptr, node_ptr);
                list_ptr = list_ptr->prev;
                node_ptr = list_ptr->val_ptr->end_ptr->prev;
                // add something maybe
                return iter;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * --iter
         */
        iterator& operator--() {
            if (node_ptr && node_ptr->prev) {
                node_ptr = node_ptr->prev;
                return *this;
            } else if (list_ptr && list_ptr->prev) {
                list_ptr = list_ptr->prev;
                node_ptr = list_ptr->val_ptr->end_ptr->prev;
                // add something maybe
                return *this;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }

        /**
         * *it
         */
        T& operator*() const {
            if (node_ptr && node_ptr->val_ptr)
                return *(node_ptr->val_ptr);
            throw std::runtime_error("operator* function: invalid iterator");
        }
        /**
         * it->field
         */
        T* operator->() const noexcept {
            if (node_ptr && node_ptr->val_ptr)
                return node_ptr->val_ptr;
            throw std::runtime_error("operator* function: invalid iterator");
        }

        /**
         * check whether two iterators are the same (pointing to the same
         * memory).
         */
        bool operator==(const iterator& rhs) const {
            return this->node_ptr == rhs.node_ptr &&
                   this->list_ptr == rhs.list_ptr;
        }
        bool operator==(const const_iterator& rhs) const {
            return this->node_ptr == rhs.node_ptr &&
                   this->list_ptr == rhs.list_ptr;
        }
        /**
         * some other operator for iterators.
         */
        bool operator!=(const iterator& rhs) const {
            return this->node_ptr != rhs.node_ptr ||
                   this->list_ptr != rhs.list_ptr;
        }
        bool operator!=(const const_iterator& rhs) const {
            return this->node_ptr != rhs.node_ptr ||
                   this->list_ptr != rhs.list_ptr;
        }
    };

    class const_iterator {
        /**
         * it should has similar member method as iterator.
         * you can copy them, but with care!
         * and it should be able to be constructed from an iterator.
         */
       public:
        Node* node_ptr;
        list_Node* list_ptr;
        const deque* check_ptr;

       public:
        int get_step() const {
            Node* ptr1 = this->node_ptr;
            list_Node* ptr2 = this->list_ptr;
            int step = 0;
            if (ptr2->next) {
                while (ptr1) {
                    ptr1 = ptr1->prev;
                    ++step;
                }
                while (ptr2) {
                    ptr2 = ptr2->prev;
                    if (ptr2)
                        step += ptr2->val_ptr->size;
                }
            }
            return step - 1 == -1 ? check_ptr->total_size : step - 1;
        }

        const_iterator quick_move(int step) const {
            if (step == 0) {
                return check_ptr->cbegin();
            }
            if (step == check_ptr->total_size) {
                return check_ptr->cend();
            }
            size_t _remain = step - 1;
            size_t step_inner;
            list_Node* l_ptr = check_ptr->list.head;
            for (int i = 0; i < check_ptr->list.size; i++) {
                if (_remain >= l_ptr->val_ptr->size) {
                    _remain -= l_ptr->val_ptr->size;
                    l_ptr = l_ptr->next;
                } else {
                    step_inner = _remain;
                    break;
                }
            }
            Node* n_ptr = l_ptr->val_ptr->head;
            for (int i = 0; i < step_inner; i++) {
                if (!n_ptr) {
                    throw std::runtime_error("quick move: out of bound");
                }
                n_ptr = n_ptr->next;
            }
            return ++const_iterator(l_ptr, n_ptr, check_ptr);
        }
        const_iterator(list_Node* ptr1 = nullptr,
                       Node* ptr2 = nullptr,
                       const deque* ptr3 = nullptr)
            : list_ptr(ptr1), node_ptr(ptr2), check_ptr(ptr3) {}
        const_iterator operator+(const int& n) const {
            int step = get_step() + n;
            return quick_move(step);
        }
        const_iterator operator-(const int& n) const {
            int step = get_step() - n;
            return quick_move(step);
        }
        int operator-(const const_iterator& rhs) const {
            if (check_ptr != rhs.check_ptr)
                throw std::runtime_error(
                    "distance function: not the same list");
            int step = this->get_step() - rhs.get_step();
            return step;
        }
        const_iterator& operator+=(const int& n) {
            int step = get_step() + n;
            auto iter = quick_move(step);
            *this = iter;
            return *this;
        }
        const_iterator& operator-=(const int& n) {
            int step = get_step() - n;
            auto iter = quick_move(step);
            *this = iter;
            return *this;
        }
        /**
         * iter++
         */
        const_iterator operator++(int) {
            if (node_ptr && node_ptr->next && node_ptr->next->val_ptr) {
                auto iter = const_iterator(list_ptr, node_ptr, check_ptr);
                node_ptr = node_ptr->next;
                return iter;
            } else if (list_ptr && list_ptr->next && list_ptr->next->val_ptr) {
                auto iter = const_iterator(list_ptr, node_ptr, check_ptr);
                list_ptr = list_ptr->next;
                node_ptr = list_ptr->val_ptr->head;
                return iter;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * ++iter
         */
        const_iterator& operator++() {
            if (node_ptr && node_ptr->next && node_ptr->next->val_ptr) {
                node_ptr = node_ptr->next;
                return *this;
            } else if (list_ptr && list_ptr->next && list_ptr->next->val_ptr) {
                list_ptr = list_ptr->next;
                node_ptr = list_ptr->val_ptr->head;
                return *this;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            if (node_ptr && node_ptr->prev) {
                auto iter = const_iterator(list_ptr, node_ptr, check_ptr);
                node_ptr = node_ptr->prev;
                return iter;
            } else if (list_ptr && list_ptr->prev) {
                auto iter = const_iterator(list_ptr, node_ptr, check_ptr);
                list_ptr = list_ptr->prev;
                node_ptr = list_ptr->val_ptr->end_ptr->prev;
                // add something maybe
                return iter;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }
        /**
         * --iter
         */
        const_iterator& operator--() {
            if (node_ptr && node_ptr->prev) {
                node_ptr = node_ptr->prev;
                return *this;
            } else if (list_ptr && list_ptr->prev) {
                list_ptr = list_ptr->prev;
                node_ptr = list_ptr->val_ptr->end_ptr->prev;
                // add something maybe
                return *this;
            }
            throw std::runtime_error("iterator funtion: index out of bound");
        }

        /**
         * *it
         */
        const T& operator*() const {
            if (node_ptr && node_ptr->val_ptr)
                return *(node_ptr->val_ptr);
            throw std::runtime_error("operator* function: invalid iterator");
        }
        /**
         * it->field
         */
        const T* operator->() const noexcept {
            if (node_ptr && node_ptr->val_ptr)
                return node_ptr->val_ptr;
            throw std::runtime_error("operator* function: invalid iterator");
        }

        bool operator==(const iterator& rhs) const {
            return this->node_ptr == rhs.node_ptr &&
                   this->list_ptr == rhs.list_ptr;
        }
        bool operator==(const const_iterator& rhs) const {
            return this->node_ptr == rhs.node_ptr &&
                   this->list_ptr == rhs.list_ptr;
        }
        /**
         * some other operator for iterators.
         */
        bool operator!=(const iterator& rhs) const {
            return this->node_ptr != rhs.node_ptr ||
                   this->list_ptr != rhs.list_ptr;
        }
        bool operator!=(const const_iterator& rhs) const {
            return this->node_ptr != rhs.node_ptr ||
                   this->list_ptr != rhs.list_ptr;
        }
    };

   public:
    /**
     * constructors.
     */
    deque() : list(), total_size(0) {}
    deque(const deque& other)
        : total_size(other.total_size), list(other.list) {}

    /**
     * deconstructor.
     */
    ~deque() {}

    /**
     * assignment operator.
     */
    deque& operator=(const deque& other) {
        if (this == &other)
            return *this;
        total_size = other.total_size;
        last_modified_Size = other.last_modified_Size;
        list = other.list;
        return *this;
    }

    /**
     * access a specified element with bound checking.
     * throw index_out_of_bound if out of bound.
     */
    T& at(const size_t& pos) {
        iterator iter = begin();
        try {
            iter += pos;
        } catch (...) {
            throw std::runtime_error("at function: index_out_of_bound");
        }
        if (!iter.node_ptr || !iter.node_ptr->val_ptr)
            throw std::runtime_error("at function: index_out_of_bound");
        return *(iter.node_ptr->val_ptr);
    }
    const T& at(const size_t& pos) const {
        const_iterator iter = cbegin();
        try {
            iter += pos;
        } catch (...) {
            throw std::runtime_error("at function: index_out_of_bound");
        }
        if (!iter.node_ptr || !iter.node_ptr->val_ptr)
            throw std::runtime_error("at function: index_out_of_bound");
        return *(iter.node_ptr->val_ptr);
    }
    T& operator[](const size_t& pos) { return at(pos); }
    const T& operator[](const size_t& pos) const { return at(pos); }

    /**
     * access the first element.
     * throw container_is_empty when the container is empty.
     */
    const T& front() const {
        if (!total_size)
            throw std::runtime_error("front function: container is empty");
        return list.front().front();
    }
    /**
     * access the last element.
     * throw container_is_empty when the container is empty.
     */
    const T& back() const {
        if (!total_size)
            throw std::runtime_error("front function: container is empty");
        return list.back().back();
    }

    /**
     * return an iterator to the beginning.
     */
    iterator begin() {
        if (total_size)
            return iterator(list.head, list.head->val_ptr->head, this);
        return end();
    }
    const_iterator cbegin() const {
        if (total_size)
            return const_iterator(list.head, list.head->val_ptr->head, this);
        return cend();
    }

    /**
     * return an iterator to the end.
     */
    iterator end() { return iterator(list.end_ptr, nullptr, this); }
    const_iterator cend() const {
        return const_iterator(list.end_ptr, nullptr, this);
    }

    /**
     * check whether the container is empty.
     */
    bool empty() const { return !total_size; }

    /**
     * return the number of elements.
     */
    size_t size() const { return total_size; }

    /**
     * clear all contents.
     */
    void clear() {
        total_size = 0;
        last_modified_Size = DEFAULT_CAPACITY;
        list.clear();
    }
    //------------------------------
    // assist function
    // to guarantee the time complicity
    //
    //
    //------------------------------
    static size_t last_modified_Size;
    int get_BlockSize() {
        if (total_size > 4 * last_modified_Size ||
            4 * total_size < last_modified_Size) {
            last_modified_Size = total_size;
        }
        return std::max((int)sqrt(last_modified_Size), DEFAULT_CAPACITY);
    }
    void compress(list_Node* lst_ptr) {
        if (!lst_ptr || lst_ptr == list.end_ptr)
            return;
        list_Node *lst_prev = lst_ptr->prev, *lst_next = lst_ptr->next;
        if (lst_prev && lst_prev->val_ptr->size + lst_ptr->val_ptr->size <=
                            get_BlockSize()) {
            while (!lst_prev->val_ptr->empty()) {
                lst_ptr->val_ptr->insert_head(lst_prev->val_ptr->back());
                lst_prev->val_ptr->delete_tail();
            }
            list.erase(list_iterator(lst_prev));
        } else if (lst_next->val_ptr &&
                   lst_next->val_ptr->size + lst_ptr->val_ptr->size <=
                       get_BlockSize()) {
            while (!lst_next->val_ptr->empty()) {
                lst_ptr->val_ptr->insert_tail(lst_next->val_ptr->front());
                lst_next->val_ptr->delete_head();
            }
            list.erase(list_iterator(lst_next));
        }
        // std::cout << "compress function deployed" << std::endl;
    }

    void expand(list_Node* lst_ptr) {
        if (lst_ptr == list.end_ptr)
            return;
        if (lst_ptr->val_ptr->size < 2 * get_BlockSize())
            return;
        if (lst_ptr->prev) {
            list_iterator iter =
                list.insert(list_iterator(lst_ptr), double_list<T>());
            list_Node* add_ptr = iter.ptr;
            size_t divide_blockSize = lst_ptr->val_ptr->size / 2;
            for (int i = 1; i <= divide_blockSize; i++) {
                add_ptr->val_ptr->insert_tail(lst_ptr->val_ptr->front());
                lst_ptr->val_ptr->delete_head();
            }
            return;
        }
        // otherwise, it's the head_node
        list.insert_head(double_list<T>());
        list_Node* add_ptr = list.head;
        size_t divide_blockSize = lst_ptr->val_ptr->size / 2;
        for (int i = 1; i <= divide_blockSize; i++) {
            add_ptr->val_ptr->insert_tail(lst_ptr->val_ptr->front());
            lst_ptr->val_ptr->delete_head();
        }
        return;
        // std::cout << "expand funtion deployed" << std::endl;
    }

    /**
     * insert value before pos.
     * return an iterator pointing to the inserted value.
     * throw if the iterator is invalid or it points to a wrong place.
     */
    iterator insert(iterator pos, const T& value) {
        if (pos.check_ptr != this)
            throw std::runtime_error(
                "insert function: not pointing to the same list");
        if (pos == end()) {
            push_back(value);
            return --end();
        }
        list_Node* lst_ptr = pos.list_ptr;
        Node* nde_ptr = pos.node_ptr;
        Node_iterator iter = Node_iterator(nde_ptr);
        Node_iterator _ret = lst_ptr->val_ptr->insert(iter, value);
        ++total_size;
        Node* _target_ptr = _ret.ptr;
        size_t _step = 0;
        while (_target_ptr != lst_ptr->val_ptr->end_ptr->prev) {
            _target_ptr = _target_ptr->next;
            ++_step;
        }
        expand(lst_ptr);
        iterator _iter = iterator(lst_ptr, _target_ptr, this);
        return _iter - _step;
    }

    /**
     * remove the element at pos.
     * return an iterator pointing to the following element. if pos points to
     * the last element, return end(). throw if the container is empty,
     * the iterator is invalid, or it points to a wrong place.
     */
    iterator erase(iterator pos) {
        if (pos.check_ptr != this)
            throw std::runtime_error(
                "erase function: not pointing to the same list");
        if (list.empty())
            throw std::runtime_error("erase function: empty container");
        if (pos == end())
            throw std::runtime_error("erase funciton: erase end");
        list_Node* lst_ptr = pos.list_ptr;
        Node* nde_ptr = pos.node_ptr;
        Node_iterator iter = Node_iterator(nde_ptr);
        Node_iterator _ret = lst_ptr->val_ptr->erase(iter);
        total_size--;
        // todo
        if (lst_ptr->val_ptr->size == 0) {
            list_iterator _it = list.erase(lst_ptr);
            if (_it == list.end())
                return end();
            return iterator(_it.ptr, _it.ptr->val_ptr->head, this);
        }
        if (!_ret.ptr->next) {
            Node* _ptr = _ret.ptr->prev;
            compress(lst_ptr);
            return ++iterator(lst_ptr, _ptr, this);
        }
        compress(lst_ptr);
        return iterator(lst_ptr, _ret.ptr, this);
    }

    /**
     * add an element to the end.
     */
    void push_back(const T& value) {
        if (!list.size) {
            list.insert_tail(double_list<T>());
        }
        list.back().insert_tail(value);
        ++total_size;
        expand(list.end_ptr->prev);
    }

    /**
     * remove the last element.
     * throw when the container is empty.
     */
    void pop_back() {
        if (!total_size)
            throw std::runtime_error("cannot pop_back");
        list.back().delete_tail();
        --total_size;
        if (list.back().size == 0) {
            list.erase(--list.end());
        } else {
            compress(list.end_ptr->prev);
        }
    }

    /**
     * insert an element to the beginning.
     */
    void push_front(const T& value) {
        if (!list.size)
            list.insert_tail(double_list<T>());
        list.front().insert_head(value);
        ++total_size;
        expand(list.head);
    }

    /**
     * remove the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (list.empty())
            throw std::runtime_error("pop_front function: container is empty.");
        list.front().delete_head();
        --total_size;
        if (list.front().size == 0) {
            list.erase(list.begin());
        } else {
            compress(list.head);
        }
    }
};
template <class T>
size_t deque<T>::last_modified_Size = DEFAULT_CAPACITY;
}  // namespace sjtu

#endif
