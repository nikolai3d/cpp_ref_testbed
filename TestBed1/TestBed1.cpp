// TestBed1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <future>

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

void main_test_return_types() {
  std::cout << "MAIN SCOPE BEGINS\n";

  // Create + Move + Destroy
  std::cout << "\nTestHolder c; test case >>>>> \n";
  TestHolder c = generate_holder();
  c.printout();

  // Same as previous
  std::cout << "\nconst TestHolder &d; test case >>>>> \n";
  const TestHolder& d = generate_holder();
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

std::future<void> launch_consumer_thread() {
  using namespace std::chrono_literals;
  std::shared_ptr<TestHolder> input_payload = std::make_shared<TestHolder>(42);



/*  std::future<void> consumer_thread_future = std::async([=] {
      std::cout << "THREAD: Printing and consuming payload: ";
      input_payload->printout();
      std::cout << "THREAD ENDS\n";
    });
*/

   /*std::future<void> propert_consumer_thread_future = std::async([payload = std::move(input_payload)]() mutable {
    std::cout << "THREAD: Printing and consuming payload: ";
    payload->printout();
    payload = nullptr;
    std::cout << "THREAD ENDS\n";
    }); */
  

   std::future<void> propert_consumer_thread_future = std::async([=]() mutable {
    std::cout << "THREAD: Printing and consuming payload: ";
    input_payload->printout();
    input_payload = nullptr; //Confusing, since captured object is basically different from source, we're just reducing refcount here.
    std::cout << "THREAD ENDS\n";
  });
  

   // Objects inside lambda capture, by default, survive till the lambda is destroyed. 
   // For the async argument, it means it survives till the corresponding future's call of "get()";
   // To destroy those explicitly, in a consumer pattern, use move semantics, and null out the object inside the lambda when it's no longer needed


  std::cout << "LAUNCHER: WAITING A BIT...\n";
  std::this_thread::sleep_for(1000ms);
  std::cout << "LAUNCHER: END\n";

  return propert_consumer_thread_future;
}

void main_test_async_lifetime() {
  using namespace std::chrono_literals;
  std::future<void> consumer_thread = launch_consumer_thread();
  std::cout << "MAIN: WAITING...\n";
  std::this_thread::sleep_for(2000ms);
  std::cout << "MAIN: WAITING SOME MORE...\n";
  std::this_thread::sleep_for(1000ms);

  std::cout << "MAIN: WAITING ON FUTURE...\n";
  (void) consumer_thread.get();

  std::cout << "MAIN: WAITING EVEN MORE...\n";
  std::this_thread::sleep_for(1000ms);
  std::cout << "MAIN: END\n";

}


int main()
{
  main_test_async_lifetime();
}


