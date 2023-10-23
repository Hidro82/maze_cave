#ifndef S21_MAZE_H
#define S21_MAZE_H
#include <experimental/random>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

namespace s21 {

class maze {
 public:
  maze() : rows_(2), cols_(2){};
  maze(int rows, int cols);

  std::vector<std::vector<int>> GetVerticalField() { return vertical_field_; }

  std::vector<std::vector<int>> GetHorizontalField() {
    return horizontal_field_;
  }

  int GetRows() { return rows_; }

  int GetCols() { return cols_; }

  std::queue<std::pair<int, int>> GetSolution() { return way_coord_; }

  void ParseFromFile(std::string path);

  void PathFinder(std::pair<int, int> starter, std::pair<int, int> finisher);

  std::string SaveTextMaker() {
    std::string to_file;
    std::string buffer =
        std::to_string(rows_) + " " + std::to_string(cols_) + "\n";
    to_file.append(buffer);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        buffer = std::to_string(vertical_field_[i][j]) + " ";
        to_file.append(buffer);
      }
      to_file.append("\n");
    }
    to_file.append("\n");
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        buffer = std::to_string(horizontal_field_[i][j]) + " ";
        to_file.append(buffer);
      }
      to_file.append("\n");
    }
    return to_file;
  }

 private:
  void CreateArray();

  void build();
  bool IsBuild() { return (std::experimental::randint(0, 1)); }

  void BuildWall(int i, int j) { vertical_field_[i][j] = 1; }
  void BuildFloor(int i, int j) { horizontal_field_[i][j] = 1; }

  void BuildMazeWall(int row);
  void BuildMazeWallEnd(int row);
  void BuildMazeFloor(int row);

  void RebuildLine(int row);

  void PrintLine(int row);

  int FindWay(int row, int col);

  int IsWayCounter(int row, int col) {
    int count = 0;
    if (!IsFloor(row, col)) {
      count++;
    }
    if (!IsWall(row, col)) {
      count++;
    }
    if (!IsLeft(row, col)) {
      count++;
    }
    if (!IsUp(row, col)) {
      count++;
    }
    return count;
  }

  void RoutineSwap();
  void FillRow(int row, int col);
  void FillBinaryWay(int row, int col);
  bool IsWall(int row, int col) {
    int answer = 0;
    if (vertical_field_[row][col] == 1) {
      answer = 1;
    } else if (way_field_[row][col + 1] != 0) {
      answer = 1;
    }
    return answer;
  }

  bool IsFloor(int row, int col) {
    int answer = 0;
    if (horizontal_field_[row][col] == 1) {
      answer = 1;
    } else if (way_field_[row + 1][col] != 0) {
      answer = 1;
    }
    return answer;
  }

  bool IsLeft(int row, int col) {
    int answer = 0;
    if (col == 0) {
      answer = 1;
    } else if (vertical_field_[row][col - 1] != 0 ||
               way_field_[row][col - 1] != 0) {
      answer = 1;
    }

    return answer;
  }

  bool IsUp(int row, int col) {
    int answer = 0;
    if (row == 0) {
      answer = 1;
    } else if (horizontal_field_[row - 1][col] != 0 ||
               way_field_[row - 1][col] != 0) {
      answer = 1;
    }
    return answer;
  }

  bool IsWayUp(int row, int col);
  bool IsWayLeft(int row, int col);
  bool IsWayRight(int row, int col);
  bool IsWayDown(int row, int col);

  int rows_;
  int cols_;
  std::vector<std::vector<int>> count_field_;
  std::vector<std::vector<int>> vertical_field_;
  std::vector<std::vector<int>> horizontal_field_;
  std::vector<std::vector<int>> way_field_;
  std::vector<std::vector<int>> way_binary_field_;

  std::pair<int, int> start_point_;
  std::pair<int, int> finish_point_;
  std::queue<std::pair<int, int>> queue_fork_;
  std::queue<std::pair<int, int>> way_coord_;
  int counter_ = 0;
  int flag_step_back_ = 0;

  void print_array(std::vector<std::vector<int>> array) {
    std::cout << "\n";
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        std::cout << array[i][j] << " ";
      }
      std::cout << "\n";
    }
  }
};

}  // namespace s21

#endif  // S21_MAZE_H
