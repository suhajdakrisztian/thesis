#include "iterator.h"
#include "p_find_if.h"
#include "p_iota.h"


#include <chrono>
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v {1, 2, 3, 4, 5, 6};
    std::vector<int> res(3);

    //pstl::copy_backward(v.begin(), v.end(), res.end());

    for(const auto& elem: res) {
      std::cout << elem << std::endl;
    }

    std::copy_backward(v.begin(), v.end(), res.end());

    for(auto r: res) {
        std::cout << r << " ";
    }

  return 0;
}
