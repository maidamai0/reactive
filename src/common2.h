#pragma once

struct CustomException {
    const char* what() const noexcept {
        return "C++ Exception";
    }
};

template <typename T>
class IEnumerator {
   public:
    using value_type = T;
    virtual ~IEnumerator() = default;

    virtual bool HasMore() = 0;
    virtual value_type Next() = 0;
};

template <typename T>
class IEnumerable {
   public:
    using value_type = T;
    virtual ~IEnumerable() = default;

    virtual IEnumerator<value_type>* GetEnumerator() = 0;
};

template <typename T>
class IObserver {
   public:
    using value_type = T;
    virtual ~IObserver() = default;

    virtual void OnCompleted() = 0;
    virtual void OnError() = 0;
    virtual void OnNext(value_type value) = 0;
};

template <typename T>
class IObservable {
   public:
    using value_type = T;
    virtual ~IObservable() = default;

    virtual bool Subscribe(IObserver<T>& observable) = 0;
};