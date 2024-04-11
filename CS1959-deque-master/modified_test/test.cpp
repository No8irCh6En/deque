#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <queue>
#include "deque.hpp"
#include "exceptions.hpp"

/***************************/
bool need_to_check_throw = 1;
bool good_complexity = 1;  // if the complexity is N^2, change to 0
int N = good_complexity ? 50000 : 1000;
/***************************/

class T {
   private:
    int x;

   public:
    T(int x) : x(x) {}
    int num() const { return x; }
    void change(int y) { x = y; }
};
bool operator==(const T& a, const T& b) {
    return a.num() == b.num();
}
bool operator!=(const T& a, const T& b) {
    return a.num() != b.num();
}
sjtu::deque<T> q;
std::deque<T> stl;
sjtu::deque<T>::iterator it_q;
std::deque<T>::iterator it_stl;
sjtu::deque<T>::const_iterator _it_q;
std::deque<T>::const_iterator _it_stl;
bool equal() {
    if (q.size() != stl.size())
        return 0;
    if (q.empty() != stl.empty())
        return 0;
    int cnt = 0;
    for (it_q = q.begin(), it_stl = stl.begin();
         it_q != q.end() || it_stl != stl.end(); it_q++, it_stl++) {
        if (*it_q != *it_stl) {
            printf("cnt = %d\n", cnt);
            return 0;
        }
        cnt++;
    }
    return 1;
}
void test1() {
    printf("test1: push & pop                    ");
    for (int i = 1; i <= 20; i++) {
        if (i % 10 <= 3) {
            std::cout << "i:" << i << std::endl;
            q.push_back(T(i)), stl.push_back(T(i));
            std::cout << q.back().num() << "  " << stl.back().num()
                      << std::endl;
            std::cout << q.front().num() << "  " << stl.front().num()
                      << std::endl;
        } else if (i % 10 <= 7) {
            std::cout << "i:" << i << std::endl;
            q.push_front(T(i)), stl.push_front(T(i));
            std::cout << q.back().num() << "  " << stl.back().num()
                      << std::endl;
            std::cout << q.front().num() << "  " << stl.front().num()
                      << std::endl;
        } else if (i % 10 <= 8) {
            std::cout << "pop_back chosen " << i << "th turn" << std::endl;
            q.pop_back(), stl.pop_back();
            std::cout << q.back().num() << "  " << stl.back().num()
                      << std::endl;
            std::cout << q.front().num() << "  " << stl.front().num()
                      << std::endl;
        } else if (i % 10 <= 9) {
            std::cout << "pop_front chosen " << i << "th turn" << std::endl;
            q.pop_front(), stl.pop_front();
            std::cout << q.back().num() << "  " << stl.back().num()
                      << std::endl;
            std::cout << q.front().num() << "  " << stl.front().num()
                      << std::endl;
        }
    }
    if (!equal()) {
        puts("Wrong Answer");
        return;
    }
    while (!q.empty()) {
        q.pop_back();
        stl.pop_back();
    }
    puts("Accept");
}
int main() {
    test1();
    return 0;
}