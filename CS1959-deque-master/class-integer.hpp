class Integer {
   private:
    int data;

   public:
    Integer(const int& value) : data(value) {}
    Integer(const Integer& other) : data(other.data) {}
    bool operator==(const Integer& t) { return data == t.data; }
    friend std::ostream& operator<<(std::ostream& os, const Integer& obj) {
        os << obj.data;  // 将对象的数据成员输出到流中
        return os;       // 返回流
    }
    explicit operator bool() const {
        return data != 0;  // 当data不等于0时返回true，否则返回false
    }
};
