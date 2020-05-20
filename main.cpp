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

/*
  Reverse the digits:
    61789372994
  Sum the odd digits:
    6 + 7 + 9 + 7 + 9 + 4 = 42 = s1
  The even digits:
      1,  8,  3,  2,  9
    Two times each even digit:
      2, 16,  6,  4, 18
    Sum the digits of each multiplication:
      2,  7,  6,  4,  9
    Sum the last:
      2 + 7 + 6 + 4 + 9 = 28 = s2

  s1 + s2 = 70 which ends in zero which means that 49927398716 passes the Luhn
  test
 */
bool luhn_check(const std::string& pan) {
  auto to_int = [](char c) {
    return c - 48;
  };  // ^ idk why this worked and stoul didnt but w/e
  auto const digits = pan | views::transform(to_int) | views::reverse;

  auto odd_digits = digits | views::stride(2);
  auto odd_digit_sum = accumulate(odd_digits, 0);

  auto even_digits = digits | views::drop(1) | views::stride(2);
  auto multiply_and_maybe_sum = [](auto n) {
    return (n != 9) ? (2 * n) % 9 : 9;
  };  // ^ insanity
      // irb(main):001:0> 2 * 8
      // => 16
      // irb(main):002:0> (2 * 8) % 9
      // => 7
      // 1 + 6 = 7
  auto partially_summed_products =
      even_digits | views::transform(multiply_and_maybe_sum);
  auto even_digit_sum = accumulate(partially_summed_products, 0);

  auto sum = odd_digit_sum + even_digit_sum;

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
