#ifndef S21_CAVE_CONTROL_H
#define S21_CAVE_CONTROL_H

#include "../cave_model/cave.h"

namespace s21 {

class CaveControl {
 public:
  CaveControl(Cave *fuckbox) { cave_data = fuckbox; }

  void Randomizer(int rows, int cols, int chance) {
    cave_data->initRandom(rows, cols, chance);
  }

  void FileParser(const std::string &filePath) {
    cave_data->initFromFile(filePath);
  }

  bool IsAlive(int row, int col) {
    std::vector<std::vector<int>> buffer = cave_data->getCave();
    return (buffer[row][col] == 1);
  }

  int VerSize() { return cave_data->getRows(); }

  int HorSize() { return cave_data->getCols(); }

  bool MakeUpdate() {
    bool is_same = true;

    std::vector<std::vector<int>> previous = cave_data->getCave();
    cave_data->update();
    std::vector<std::vector<int>> current = cave_data->getCave();
    for (int i = 0; i < cave_data->getRows(); i++) {
      if (current[i] != previous[i]) {
        is_same = false;
        break;
      }
    }
    return is_same;
  }

  void SetBD(int birth, int death) { cave_data->setLimits(birth, death); }

 private:
  Cave *cave_data;
};

}  // namespace s21

#endif  // S21_CAVE_CONTROL_H