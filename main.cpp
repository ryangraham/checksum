#include <iostream>
#include <range/v3/all.hpp>
#include <string>

using namespace ranges;

// unsigned int to_int(char c) { return c - 48; }

bool rtn_checksum(const std::string& routing_number) {
  std::vector<int> const multipliers{3, 7, 1, 3, 7, 1, 3, 7, 1};

  auto to_int = [](char c) { return std::stoul(&c, nullptr, 10); };
  auto const routing_digits = routing_number | views::transform(to_int);

  auto multiply = [](auto a, auto b) { return a * b; };
  auto sum =
      accumulate(views::zip_with(multiply, routing_digits, multipliers), 0);

  return sum % 10 == 0;
}

bool luhn_check(const std::string& pan) {
  auto to_int = [](char c) { return c - 48; };

  auto sum_digits = [](int n) { return n / 10 + n % 10; };
  auto do_digits = [&sum_digits](auto&& values) {
    auto const& [index, digit] = values;
    return (index % 2 == 0) ? digit : sum_digits(digit * 2);
  };

  auto rng = pan 
              | views::transform(to_int)
              | views::reverse
              | views::enumerate
              | views::transform(do_digits);

  auto sum = accumulate(rng, 0);

  return sum % 10 == 0;
}

int main() {
  std::string routing_number = "325081403";
  auto valid = rtn_checksum(routing_number);
  std::cout << "Routing number: " << routing_number << std::endl;
  std::cout << "Valid RTN Checksum? " << std::boolalpha << valid << std::endl
            << std::endl;

  auto cards = {"49927398716", "49927398717", "1234567812345678",
                "1234567812345670"};
  auto print = [](const std::string& pan) {
    std::cout << "Card number: " << pan << std::endl
              << "Valid LUHN Checksum? " << std::boolalpha << luhn_check(pan)
              << std::endl
              << std::endl;
  };
  for_each(cards, print);

  return 0;
}
