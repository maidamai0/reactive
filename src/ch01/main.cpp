#include <cassert>
#include <exception>
#include <iostream>
#include <vector>

#include "common2.h"

class Enumerator final : public IEnumerator<int> {
   public:
    explicit Enumerator(std::vector<value_type>& numbers) : numbers_(numbers) {}
    bool HasMore() override {
        return index_ < numbers_.size();
    }

    value_type Next() override {
        assert(HasMore() && "No more data");
        return numbers_[index_++];
    }

   private:
    std::vector<value_type>& numbers_;
    size_t index_ = 0;
};

class ConcreteEnumerable final : public IEnumerable<int> {
   public:
    explicit ConcreteEnumerable(std::vector<int>& numbers) : numbers_{numbers} {}
    ConcreteEnumerable() = default;
    ~ConcreteEnumerable() override = default;

    Enumerator* GetEnumerator() override {
        return new Enumerator(numbers_);
    }

   private:
    std::vector<value_type> numbers_;
};

class EvenNumberObservable final : public IObservable<int> {
   public:
    explicit EvenNumberObservable(std::vector<value_type>& numbers) : numbers_(numbers) {}

    bool Subscribe(IObserver<value_type>& observer) override {
        for (const auto& number : numbers_) {
            if (number % 2 == 0) {
                observer.OnNext(number);
            } else {
                observer.OnError();
            }
        }

        observer.OnCompleted();
        return true;
    }

   private:
    std::vector<value_type>& numbers_;
};

class SimpleObserver final : public IObserver<int> {
   public:
    void OnNext(value_type v) override {
        std::cout << "get " << v << std::endl;
    }

    void OnCompleted() override {
        std::cout << "completed" << std::endl;
    }

    void OnError() override {
        std::cerr << "error" << std::endl;
    }
};

int main() {
    std::vector<int> sources{1, 2, 3, 4, 5};
    auto* t = new ConcreteEnumerable(sources);
    auto* numbers = t->GetEnumerator();
    while (numbers->HasMore()) {
        std::cout << "value is " << numbers->Next() << std::endl;
    }

    {
        EvenNumberObservable numbers(sources);
        SimpleObserver observer;
        numbers.Subscribe(observer);
    }

    return 0;
}