#ifndef S21_MAZE_CONTROL_H
#define S21_MAZE_CONTROL_H

#include "../maze_model/s21_maze.h"

namespace s21 {

class MazeControl {
 public:
  MazeControl(maze* fuckbox) { maze_data = fuckbox; }

  bool IsWall(int row, int col) {
    std::vector<std::vector<int>> buffer = maze_data->GetVerticalField();
    return (buffer[row][col] == 1);
  }

  bool IsFloor(int row, int col) {
    std::vector<std::vector<int>> buffer = maze_data->GetHorizontalField();
    return (buffer[row][col] == 1);
  }

  int VerSize() { return maze_data->GetRows(); }

  int HorSize() { return maze_data->GetCols(); }

  std::queue<std::pair<int, int>> SolutionPlace() {
    return maze_data->GetSolution();
  }

  void CreateSolution(std::pair<int, int> starter,
                      std::pair<int, int> finisher) {
    maze_data->PathFinder(starter, finisher);
  }

 private:
  maze* maze_data;
};

}  // namespace s21

#endif  // S21_MAZE_CONTROL_H