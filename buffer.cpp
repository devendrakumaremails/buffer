/*
Buffer for single thread.
*/
#include <stdio.h>
#include <iostream>
#include <utility>
#include <vector>

template <typename T, unsigned int N>
class Buffer {
 private:
  std::vector<T> storage_;
  // T storage_[N];
  unsigned int putIndex_{};
  unsigned int getIndex_{};
  bool empty_{true};
  bool full_{false};

 public:
  Buffer() { storage_.reserve(N); };
  Buffer(Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(Buffer &) = delete;
  Buffer &operator=(Buffer &&) = delete;
  explicit operator bool() const { return !empty_; }
  void operator<<(T &&data) { put(std::forward<T>(data)); }

  bool put(const T &&data)  // universal reference to accept both rvalue and lvalue
  {
    bool success = true;
    if (isFull())  //((getIndex_ == putIndex_) && (!isEmpty()))
    {
      std::cout << "Buffer Full!\n";
      success = false;
    }

    else {
      storage_[putIndex_] = data;
      putIndex_ = (++putIndex_) % N;
      full_ = getIndex_ == putIndex_;
      empty_ = false;
    }
    print();
    return success;
  }
  T get() {
    if (isEmpty()) {
      std::cout << "Buffer Empty!\n";
      return T();
    }

    T &data = storage_[getIndex_];
    getIndex_ = (++getIndex_) % N;
    empty_ = getIndex_ == putIndex_;
    full_ = false;

    print();
    return data;
  }
  bool isEmpty() const { return empty_; }
  bool isFull() const { return full_; }
  /*std::outstream& operator << (std::outstream out, const T& data)
  {
          
  }*/
  void print() const {
    if (isEmpty()) {
      std::cout << "Buffer empty!\n";
      return;
    }

    int itr = getIndex_;
    std::cout << "Buffer: " << storage_[itr] << '\t';
    for (itr = (itr + 1) % N; (itr != putIndex_); itr = (itr + 1) % N) std::cout << storage_[itr] << '\t';
    std::cout << std::endl;
  }
};

int main() {
  Buffer<int, 5> buffer;
  int command = -1, num = 0;

  while (command) {
    switch (command) {
      case 1: {
        buffer.put(num++);  // lvalue
        buffer << 23;       // rvalue
        break;
      }
      case 2: {
        if (buffer) {
          int getVal = buffer.get();
          std::cout << "getVal: " << getVal << '\n';
        }
        break;
      }
      default: {
        std::cout << "1: put data into buffer\n"
                  << "2: get data from buffer\n"
                  << "0: exit\n";
        break;
      }
    }
    std::cout << "\nCommand: ";
    std::cin >> command;
  }
}
