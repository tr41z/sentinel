#include "include/flow.h"
#include <iostream>
#include <unordered_map>

void add_test(int a, int b) {
  std::unordered_map<int, int> flows_map;

  // Insert key-value pairs into the map
  flows_map.insert({a, a});
  flows_map.insert({b, b});

  // Display the contents of the map
  for (const auto &pair : flows_map) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second
              << std::endl;
  }
}
