#include <iterator>
#include <ranges>
#include <vector>
#include <iostream>
#include <string>

template<class ForwardIter, class UnaryPred>
class LambdaFilter {
private:
    //std::ranges::filter_view<std::ranges::join_view<std::ranges::ref_view<std::vector<std::vector<ForwardIter>>>>, UnaryPred> _values;
    std::vector<ForwardIter> _raw_values;

public:
    LambdaFilter(std::vector<std::vector<ForwardIter>> results, UnaryPred pred) {
        auto filtered_elements = results | std::ranges::views::join | std::views::filter(pred);
        for(auto element: filtered_elements) {
            _raw_values.push_back(element);
        }
    }

    [[nodiscard]] auto &get_values() const {
        return _raw_values;
    }

    struct Iterator {
        Iterator(ForwardIter *ptr) : m_ptr(ptr) {}

        ForwardIter &operator*() const { return *m_ptr; }

        ForwardIter *operator->() { return m_ptr; }

        // Prefix increment
        Iterator &operator++() {
            m_ptr ++;
            return *this;
        }

        // Postfix increment
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
