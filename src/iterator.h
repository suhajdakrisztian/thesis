#pragma once

#include <iterator>
#include <ranges>
#include <vector>
#include <iostream>
#include <string>
#include <type_traits>

template<class ForwardIter, class Predicate> requires std::forward_iterator<ForwardIter>
class LambdaFilter {
private:
    //std::ranges::filter_view<std::ranges::join_view<std::ranges::ref_view<std::vector<std::vector<ForwardIter>>>>, UnaryPred> _values;
    std::vector<ForwardIter> _raw_values;
public:
    LambdaFilter(const std::vector<std::vector<ForwardIter>>& results, Predicate pred) {
        auto filtered_elements = results | std::ranges::views::join;

        for(auto element: filtered_elements) {
            if(pred(*element)) {
                _raw_values.push_back(element);
            }
        }
    }

    [[nodiscard]] auto &get_values() const {
        return _raw_values;
    }

    size_t size() const {
        return _raw_values.size();
    }

    struct Iterator {
        Iterator(ForwardIter *ptr) : m_ptr(ptr) {}

        ForwardIter &operator*() const { return *m_ptr; }

        ForwardIter *operator->() { return m_ptr; }

        Iterator &operator++() {
            m_ptr ++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++ (*this);
            return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };

        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };

        friend bool operator<(const Iterator &a, const Iterator &b) { return a.m_ptr < b.m_ptr; };

    private:
        ForwardIter *m_ptr;
    };

    Iterator begin() { return Iterator(&_raw_values[0]); }

    Iterator end() { return Iterator(&_raw_values[_raw_values.size()]); }
};
