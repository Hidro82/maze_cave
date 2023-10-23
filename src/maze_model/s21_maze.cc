#include "s21_maze.h"

namespace s21 {

void maze::CreateArray() {
  for (int i = 0; i < rows_; i++) {
    count_field_.push_back(std::vector<int>(cols_));
    vertical_field_.push_back(std::vector<int>(cols_));
    way_binary_field_.push_back(std::vector<int>(cols_));
    horizontal_field_.push_back(std::vector<int>(cols_));
    way_field_.push_back(std::vector<int>(cols_));
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      vertical_field_[i][j] = 0;
      horizontal_field_[i][j] = 0;
      way_field_[i][j] = 0;
      way_binary_field_[i][j] = 0;
      if (i == start_point_.first && j == start_point_.second) {
        way_field_[i][j] = 1;
      }
    }
  }
}

maze::maze(int rows, int cols) : rows_(rows), cols_(cols) {
  start_point_ = std::make_pair(0, 0);
  finish_point_ = std::make_pair(rows - 1, cols - 1);

  if ((start_point_.first > 0 && start_point_.second > 0) ||
      start_point_.first < 0 || start_point_.second < 0) {
    throw std::invalid_argument("wrong starts points");
  }

  if ((finish_point_.first > rows_ && finish_point_.second > cols_) ||
      finish_point_.first > rows_ || finish_point_.second > cols_) {
    throw std::invalid_argument("wrong finish points");
  }

  if (rows_ < 2 || cols_ < 2) {
    throw std::invalid_argument("Wrong input numbers, check rows and cols");
  }

  CreateArray();
  build();

  // std::cout << "// vertical_field_";
  // print_array(vertical_field_);
  // std::cout << "// horizontal_field_";
  // print_array(horizontal_field_);
}

void maze::PathFinder(std::pair<int, int> starter,
                      std::pair<int, int> finisher) {
  start_point_ = starter;
  finish_point_ = finisher;

  FindWay(start_point_.first, start_point_.second);
  FillBinaryWay(finish_point_.first, finish_point_.second);

  // std::cout << "// way_field_";
  // print_array(way_field_);
  // std::cout << "// binary_way_field_";
  // print_array(way_binary_field_);
}

void maze::build() {
  for (int row = 0; row < rows_; row++) {
    if (row == 0) {
      for (int i = 0; i < cols_; i++) {
        count_field_[0][i] = ++counter_;
      }
    }

    if (row > 0) {
      RebuildLine(row);
    }

    BuildMazeWall(row);
    BuildMazeFloor(row);

    if (row == rows_ - 1) {
      // std::cout << "// BEFOR LASY count_field_";
      // print_array(vertical_field_);

      BuildMazeWallEnd(row);
    }
  }
}

void maze::BuildMazeWall(int row) {
  for (int cell = 0; cell < cols_; cell++) {
    if (cell != cols_ - 1 &&
        count_field_[row][cell] == count_field_[row][cell + 1]) {
      BuildWall(row, cell);
      continue;
    }

    if (IsBuild()) {
      BuildWall(row, cell);

    } else {
      if (cell == cols_ - 1) {
        BuildWall(row, cell);
        break;
      }

      int multiple = count_field_[row][cell + 1];

      for (int i = 0; i < cols_; i++) {
        if (count_field_[row][i] == multiple) {
          count_field_[row][i] = count_field_[row][cell];
        }
      }
    }
  }
}

void maze::BuildMazeWallEnd(int row) {
  for (int cell = 0; cell < cols_; cell++) {
    if (cell == cols_ - 1) {
      break;
    }

    if (count_field_[row][cell] != count_field_[row][cell + 1]) {
      vertical_field_[row][cell] = 0;
      int multiple = count_field_[row][cell + 1];
      for (int i = 0; i < cols_; i++) {
        if (count_field_[row][i] == multiple) {
          count_field_[row][i] = count_field_[row][cell];
        }
      }
    }
  }
}

void maze::BuildMazeFloor(int row) {
  std::map<int, int> count_free_cells;

  for (int cell = 0; cell < cols_; cell++) {
    if (row == rows_ - 1) {
      BuildFloor(row, cell);
      continue;
    }

    if (count_free_cells.find(count_field_[row][cell]) ==
        count_free_cells.end()) {
      int count_all = 0;

      for (int i = 0; i < cols_; i++) {
        if (count_field_[row][cell] == count_field_[row][i]) {
          count_all++;
        }
      }

      count_free_cells.insert({count_field_[row][cell], count_all});
    }

    if (IsBuild()) {
      if (count_free_cells[count_field_[row][cell]] > 1) {
        count_free_cells[count_field_[row][cell]] =
            count_free_cells[count_field_[row][cell]] - 1;
        BuildFloor(row, cell);
      }
    }
  }
}

void maze::RebuildLine(int row) {
  for (int cell = 0; cell < cols_; cell++) {
    count_field_[row][cell] = count_field_[row - 1][cell];

    if (horizontal_field_[row - 1][cell] == 1) {
      count_field_[row][cell] = ++counter_;
    }
  }
}

void maze::PrintLine(int row) {
  for (int i = 0; i < cols_; i++) {
    std::cout << count_field_[row][i] << " ";
  }
}

void maze::RoutineSwap() {
  std::pair<int, int> temp = queue_fork_.front();
  queue_fork_.pop();
  flag_step_back_ = 1;
  FindWay(temp.first, temp.second);
}

void maze::FillRow(int row, int col) {
  if (!IsWall(row, col)) {
    way_field_[row][col + 1] = way_field_[row][col] + 1;
    flag_step_back_ = 0;

    FindWay(row, col + 1);

  } else if (!IsFloor(row, col)) {
    way_field_[row + 1][col] = way_field_[row][col] + 1;
    flag_step_back_ = 0;

    FindWay(row + 1, col);
  } else if (!IsLeft(row, col)) {
    way_field_[row][col - 1] = way_field_[row][col] + 1;
    flag_step_back_ = 0;

    FindWay(row, col - 1);
  } else if (!IsUp(row, col)) {
    way_field_[row - 1][col] = way_field_[row][col] + 1;
    flag_step_back_ = 0;

    FindWay(row - 1, col);
  }
}

int maze::FindWay(int row, int col) {
  if (IsWayCounter(row, col) == 0) {
    if (queue_fork_.empty()) {
      return 1;
    }
    RoutineSwap();
  }

  if (IsWayCounter(row, col) == 1 || flag_step_back_) {
    FillRow(row, col);

  }

  else if (IsWayCounter(row, col) > 1) {
    for (int i = 0; i < IsWayCounter(row, col); i++) {
      queue_fork_.push(std::make_pair(row, col));
    }

    RoutineSwap();
  }

  return 0;
}

bool maze::IsWayUp(int row, int col) {
  int answer = 0;
  if (row == 0) {
    answer = 0;
  } else if (horizontal_field_[row - 1][col] == 1) {
    answer = 0;
  } else if (way_field_[row - 1][col] == way_field_[row][col] - 1) {
    answer = 1;
  }
  return answer;
}

bool maze::IsWayLeft(int row, int col) {
  int answer = 0;
  if (col == 0) {
    answer = 0;
  } else if (vertical_field_[row][col - 1] == 1) {
    answer = 0;
  } else if (way_field_[row][col - 1] == way_field_[row][col] - 1) {
    answer = 1;
  }
  return answer;
}

bool maze::IsWayRight(int row, int col) {
  int answer = 0;
  if (col == cols_ - 1) {
    answer = 0;
  } else if (vertical_field_[row][col] == 1) {
    answer = 0;
  } else if (way_field_[row][col + 1] == way_field_[row][col] - 1) {
    answer = 1;
  }
  return answer;
}

bool maze::IsWayDown(int row, int col) {
  int answer = 0;
  if (row == rows_ - 1) {
    answer = 0;
  } else if (horizontal_field_[row][col] == 1) {
    answer = 0;
  } else if (way_field_[row + 1][col] == way_field_[row][col] - 1) {
    answer = 1;
  }
  return answer;
}

void maze::FillBinaryWay(int row, int col) {
  way_binary_field_[row][col] = 1;
  way_coord_.push(std::make_pair(row, col));

  if (IsWayUp(row, col)) {
    FillBinaryWay(row - 1, col);
  }

  else if (IsWayLeft(row, col)) {
    FillBinaryWay(row, col - 1);

  }

  else if (IsWayRight(row, col)) {
    FillBinaryWay(row, col + 1);

  }

  else if (IsWayDown(row, col)) {
    FillBinaryWay(row + 1, col);
  }
}

void maze::ParseFromFile(std::string path) {
  // std::cout << "\n_________\n";

  int count = 0;
  std::string line;

  std::ifstream in(path);

  int flag_change_matrix = 0;

  if (in.is_open()) {
    while (std::getline(in, line)) {
      std::vector<int> v;
      std::istringstream cast(line);
      int temp;

      while (cast >> temp) {
        v.push_back(temp);
      }
      // std::cout << "\n____2_____\n";
      if (v.size() == 0) {
        flag_change_matrix = 1;
        count = 1;
        continue;
      }
      if (count == 0) {
        rows_ = v[0];
        cols_ = v[1];
        CreateArray();
        count++;
        continue;
      }

      int col = 0;
      for (auto i = v.begin(); i != v.end(); i++) {
        if (!flag_change_matrix) {
          vertical_field_[count - 1][col] = *i;
        } else {
          horizontal_field_[count - 1][col] = *i;
        }
        col++;
      }
      // std::cout << line << std::endl;

      count++;
    }
  }
  in.close();
  start_point_ = std::make_pair(0, 0);
  finish_point_ = std::make_pair(rows_ - 1, cols_ - 1);
}

}  // namespace s21
