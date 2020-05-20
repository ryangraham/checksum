#include <iostream>
#include <range/v3/all.hpp>
#include <string>

using namespace ranges;

// unsigned int to_int(char c) { return c - 48; }

bool rtn_checksum(const std::string& routing_number) {
  std::vector<int> const multipliers{3, 7, 1, 3, 7, 1, 3, 7, 1};

  auto to_int = [](char c) { return std::stoul(&c, nullptr, 10); };
  auto const routing_digits = routing_number | views::transform(to_int);

  auto multiply = [](auto&& values) {
    auto const& [a, b] = values;
    return a * b;
  };
  auto sum = accumulate(
      views::zip(routing_digits, multipliers) | views::transform(multiply), 0);

  return sum % 10 == 0;
}

// TODO
bool luhn_check(const std::string& pan) { return true; }

int main() {
  std::string routing_number = "325081403";
  auto valid = rtn_checksum(routing_number);
  std::cout << "Routing number: " << routing_number << std::endl;
  std::cout << "Valid? " << std::boolalpha << valid << std::endl;

  return 0;
}
