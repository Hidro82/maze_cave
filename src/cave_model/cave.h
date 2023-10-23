#ifndef CAVE_H
#define CAVE_H

#include <iostream>
#include <vector>

namespace s21 {

class Cave {
 public:
  Cave();
  Cave(const std::string &filePath, const int &limitBirth,
       const int &limitDeath);
  Cave(const int &rows, const int &cols, const int &limitBirth,
       const int &limitDeath, const int &chance);

  void initFromFile(const std::string &filePath);
  void initRandom(const int rows, const int cols, const int chance);
  void setLimits(const int limitBirth, const int limitDeath);

  const std::vector<std::vector<int>> &getCave() const noexcept;
  void update() noexcept;

  int getRows() { return rows; }

  int getCols() { return cols; }

  std::string SaveTextMaker() {
    std::string to_file;
    std::string buffer =
        std::to_string(rows) + " " + std::to_string(cols) + "\n";
    to_file.append(buffer);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        buffer = std::to_string(cave[i][j]) + " ";
        to_file.append(buffer);
      }
      to_file.append("\n");
    }
    return to_file;
  }

 private:
  std::vector<std::string> readFile(const std::string &filePath) const;
  void parseFile(const std::vector<std::string> &fileData);
  bool checkSize(const int &row, const int &col) const noexcept;
  bool checkLimit(const int &birth, const int &death) const noexcept;
  int countAlivesAround(const int i, const int k) const noexcept;
  const int minCaveSizeRowCol = 1;
  const int maxCaveSizeRow = 50;
  const int maxCaveSizeCol = 50;
  const int minDeathBirthLimit = 0;
  const int maxDeathBirthLimit = 7;
  int rows = 0;
  int cols = 0;
  int limitBirth = 0;
  int limitDeath = 0;
  std::vector<std::vector<int>> cave;
};

}  // namespace s21

#endif  //  CAVE_H
