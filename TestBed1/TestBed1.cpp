// TestBed1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <memory>

class TestHolder {
public:
  using payload_t = std::uint64_t;

  std::unique_ptr<payload_t> _payload;
public:
  TestHolder() : TestHolder(0ull) {
    std::cout << "Default Constructor";
  }

  TestHolder(const payload_t &i_source) {
    std::cout << "Initializing Constructor (" << i_source << ")\n";
    _payload = std::make_unique<payload_t>();
    *_payload = i_source;
  }

  TestHolder(const TestHolder& i_other) {
    std::cout << "Copy Constructor\n";
    *this = i_other;
  }

  TestHolder(TestHolder&& i_other) noexcept {
    std::cout << "Move Constructor\n";
    *this = std::move(i_other);
  }

  TestHolder& operator =(const TestHolder& i_other) {
    std::cout << "Copy Assignment Operator\n";
    if (this == &i_other) {
      return *this;
    }

    if (i_other._payload == nullptr) {
      throw std::runtime_error("Invalid Object State");
    }

    this->_payload = std::make_unique<payload_t>(*(i_other._payload));
    return *this;
  }

  TestHolder& operator =(TestHolder&& i_other) noexcept {
    std::cout << "Move Assignment Operator\n";

    this->_payload = std::move(i_other._payload);
    return *this;
  }

  void print_payload() const noexcept {
    if (_payload) {
      std::cout << "[" << *_payload << "]\n";
    }
    else {
      std::cout << "[EMPTY]\n";
    }
  }

  void printout() const noexcept {
    std::cout << "Object Printout: ";
    print_payload();

  }

  ~TestHolder() {
    std::cout << "Destructor of ";
    print_payload();
  }
};

TestHolder generate_holder() {
  static std::uint64_t s_generate_counter = 10;
  TestHolder r(s_generate_counter++);
  return r;
}

TestHolder & generate_payload_ref() {
  static std::uint64_t s_generate_counter = 1000;
  TestHolder r(s_generate_counter++);
  return r;
}
int main()
{
    std::cout << "MAIN SCOPE BEGINS\n";

    // Create + Move + Destroy
    std::cout << "\nTestHolder c; test case >>>>> \n";
    TestHolder c = generate_holder();
    c.printout();

    // Same as previous
    std::cout << "\nconst TestHolder &d; test case >>>>> \n";
    const TestHolder &d = generate_holder();
    d.printout();

    std::cout << "\nTestHolder&& e; test case >>>>> \n";
    TestHolder&& e = generate_holder();
    e.printout();
    // Does not compile
    //TestHolder& m = generate_count_sequence();
    // m.printout();

    // Generates compiler warning + Undefined behaviour(Access violation, crash, or nothing) at runtime.
    // TestHolder& mm = generate_count_sequence_bad();
    // mm.printout();
    std::cout << "\nMAIN SCOPE ENDS\n\n";
}