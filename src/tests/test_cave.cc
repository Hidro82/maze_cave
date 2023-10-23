#include <chrono>
#include <fstream>
#include <thread>

#include "../cave/cave.h"
#include "gtest/gtest.h"

void timeSleep(const int millisec) {
  std::cout << "Timer started for " << millisec << " milliseconds."
            << std::endl;
  auto start_time = std::chrono::steady_clock::now();
  while (true) {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            current_time - start_time)
                            .count();
    if (elapsed_time >= millisec) {
      std::cout << "Time's up!" << std::endl;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void printCave(const Cave& cave) {
  std::cout << std::endl;
  for (const std::vector<int>& line : cave.getCave()) {
    for (const int& val : line) {
      std::cout << val << ' ';
    }
    std::cout << std::endl;
  }
}

TEST(ParseFile, test1) {
  // Cave cave(5, 5, 6, 4, 50);
  Cave cave("cave/file.txt", 5, 6);
  std::vector<std::vector<int>> matrixFromFile{
      {0, 1, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 0}, {0, 0, 1, 1}};
  bool isSame = true;
  for (int i = 0; i < static_cast<int>(matrixFromFile.size()); ++i) {
    for (int k = 0; k < static_cast<int>(matrixFromFile.at(i).size()); ++k) {
      if (matrixFromFile[i][k] != cave.getCave()[i][k]) {
        isSame = false;
        break;
      }
    }
  }
  ASSERT_TRUE(isSame);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
