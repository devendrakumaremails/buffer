/*
Buffer for single thread.
*/
#include <stdio.h>
#include <iostream>
#include <utility>
#include <vector>

template <typename T, unsigned int N>
class Buffer
{
  private:
    std::vector<T> storage_;                // other way:  T storage_[N];
    unsigned int put_index_{};
    unsigned int get_index_{};
    bool empty_{true};
    bool full_{false};

  public:
    Buffer()
    {
      storage_.reserve(N);
    }

    Buffer(Buffer &) = delete;
    Buffer(Buffer &&) = delete;
    Buffer &operator=(Buffer &) = delete;
    Buffer &operator=(Buffer &&) = delete;

    explicit operator bool() const
    {
      return !empty_;
    }
    bool operator << (T &&data)
    {
      return Put(std::forward<T>(data)); 
    }

    bool Put(const T &&data)  // universal reference to accept both rvalue and lvalue
    {
      bool success = true;
      if (IsFull())
      {
        success = false;
      }
      else
      {
        storage_[put_index_] = data;
        put_index_ = (++put_index_) % N;
        full_ = get_index_ == put_index_;
        empty_ = false;
      }

      return success;
    }
    bool Get(T &output_data_variable)
    {
      if (IsEmpty())
      {
        return false;
      }
      output_data_variable = storage_[get_index_];
      get_index_ = (++get_index_) % N;
      empty_ = get_index_ == put_index_;
      full_ = false;

      return true;
    }
    bool IsEmpty() const 
    {
      return empty_;
    }
    bool IsFull() const
    {
      return full_;
    }

    void Print() const
    {
      if (IsEmpty())
      {
        std::cout << "Buffer empty!\n";
        return;
      }

      int itr = get_index_;
      std::cout << "Buffer: " << storage_[itr] << '\t';
      for (itr = (itr + 1) % N; (itr != put_index_); itr = (itr + 1) % N)
      {
        std::cout << storage_[itr] << '\t';
      }
      std::cout << std::endl;
    }
};

int main() {
  Buffer<int, 5> buffer;
  int command = -1, num = 0;

  while (command)
  {
    switch (command)
    {
      case 1:
      {
        if(!buffer.Put(num++))                // lvalue
        {
          std::cout << "Buffer is Full!\n";
        }
        if(!(buffer << 23))                   // rvalue
        {
          std::cout << "Buffer is Full!\n";
        }
        break;
      }
      case 2:
      {
        int getVal;
        if (buffer.Get(getVal))
        {
          std::cout << "getVal: " << getVal << '\n';
        }
        else
        {
          std::cout << "Buffer is Empty!\n";
        }
        
        break;
      }
      default:
      {
        std::cout << "1: put data into buffer\n"
                  << "2: get data from buffer\n"
                  << "0: exit\n";
        break;
      }
    }
    buffer.Print();
    std::cout << "\nCommand: ";
    std::cin >> command;
  }
}
