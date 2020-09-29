#pragma once

/**
 * @file func_compose.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-28
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <iterator>
#include <utility>

template <typename F, typename G>
auto Compose(F&& f, G&& g) {
    return [=](auto x) { f(g(x)); };
}

template <typename F, typename... Fs>
auto Compose(F&& f, Fs... fs) {
    return [=](auto x) { return f(Compose(fs...)(x)); };
}

// template <typename F, typename G>
// auto operator|(F&& f, G&& g) {
//     return Compose(std::forward<F>(f), std::forward<G>(g));
// }

// template <typename F, typename... Fs>
// auto operator|(F&& f, Fs... fs) {
//     return operator|(std::forward<F>(f), fs...);
// }

template <typename C, typename Fn>
auto Map(C r, Fn&& f) {
    std::transform(std::begin(r), std::end(r), std::begin(r), std::forward<Fn>(f));
    return r;
}

template <typename C, typename Fn>
auto Filter(C c, Fn&& f) {
    C ret(c.size());
    auto first = std::begin(c);
    auto last = std::end(c);
    auto result = std::begin(ret);

    auto inserted = false;
    while (last != first) {
        if (f(*first)) {
            *(result++) = *first;
            inserted = true;
        }
        ++first;
    }

    if (!inserted) {
        ret.clear();
        ret.resize(0);
    }

    return ret;
}