#include <iostream>

#include "array_list.hpp"

int main() {
  auto int_list = ArrayList<int>();

  for (int i = 0; i < 10; ++i)
    int_list.add(i);
  std::cout << int_list << std::endl;

  std::cout << "removing: " << int_list.remove(4) << std::endl;
  std::cout << int_list << std::endl;

  std::cout << "adding 4 back but to the end" << std::endl;
  int_list.add(4);
  std::cout << int_list << std::endl;

  std::cout << "removing: " << int_list.remove(0) << std::endl;
  std::cout << int_list << std::endl;

  std::cout << "inserting 42 at index 1" << std::endl;
  int_list.insert(1, 42);
  std::cout << int_list << std::endl;

  std::cout << "setting second last element to 1000" << std::endl;
  int_list[int_list.size() - 2] = 1000;
  std::cout << int_list << std::endl;

  auto new_list = int_list;
  std::cout << "new_list: " << new_list << std::endl;
  std::cout << "setting first element of int_list to 12345" << std::endl;
  int_list[0] = 12345;

  std::cout << "int_list: " << int_list << std::endl;
  std::cout << "new_list: " << new_list << std::endl;

  std::cout << "Creating move list" << std::endl;
  ArrayList<int> move_list;
  std::cout << "move_list: " << move_list << std::endl;

  std::cout << "Moving new_list to move_list" << std::endl;
  move_list = std::move(new_list);
  std::cout << "new_list: " << new_list << std::endl;
  std::cout << "move_list: " << move_list << std::endl;

  return 0;
}
