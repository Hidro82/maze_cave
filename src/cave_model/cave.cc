#include "cave.h"

#include <fstream>
#include <random>
#include <string>

#include "bits/stdc++.h"

namespace s21 {

Cave::Cave(){};

Cave::Cave(const std::string& filePath, const int& limitBirth,
           const int& limitDeath) {
  if (filePath == "" || !checkLimit(limitBirth, limitDeath))
    throw std::invalid_argument("wrong parameters: " + filePath + ' ' +
                                std::to_string(limitBirth) + ' ' +
                                std::to_string(limitDeath));
  initFromFile(filePath);
  setLimits(limitBirth, limitDeath);
}

Cave::Cave(const int& rows, const int& cols, const int& limitBirth,
           const int& limitDeath, const int& chance) {
  if (!checkSize(rows, cols) || !checkLimit(limitBirth, limitDeath))
    throw std::invalid_argument("wrong parameters: " + std::to_string(rows) +
                                ' ' + std::to_string(cols) + ' ' +
                                std::to_string(limitBirth) + ' ' +
                                std::to_string(limitDeath));
  initRandom(rows, cols, chance);
  setLimits(limitBirth, limitDeath);
}

const std::vector<std::vector<int>>& Cave::getCave() const noexcept {
  return cave;
}

void Cave::update() noexcept {
  int sizeVertical = cave.size();
  int sizeHorizontal = cave.at(0).size();
  for (int i = 0; i < sizeVertical; ++i) {
    for (int k = 0; k < sizeHorizontal; ++k) {
      if (cave.at(i).at(k) && countAlivesAround(i, k) < limitDeath)
        cave[i][k] = 0;
      else if (!cave.at(i).at(k) && countAlivesAround(i, k) > limitBirth)
        cave[i][k] = 1;
    }
  }
}

int Cave::countAlivesAround(const int i, const int k) const noexcept {
  int lastVertical = cave.size() - 1;
  int lastHorizontal = cave.at(i).size() - 1;
  int count = 0;

  if (!i || cave.at(i - 1).at(k)) ++count;            // UP
  if (!i || !k || cave.at(i - 1).at(k - 1)) ++count;  // UP-LEFT
  if (!i || k == lastHorizontal || cave.at(i - 1).at(k + 1))
    ++count;  // UP-RIGHT

  if (i == lastVertical || cave.at(i + 1).at(k)) ++count;  // DOWN
  if (i == lastVertical || !k || cave.at(i + 1).at(k - 1))
    ++count;  // DOWN-LEFT
  if (i == lastVertical || k == lastHorizontal || cave.at(i + 1).at(k + 1))
    ++count;  // DOWN-RIGHT

  if (!k || cave.at(i).at(k - 1)) ++count;                   // LEFT
  if (k == lastHorizontal || cave.at(i).at(k + 1)) ++count;  // RIGHT
  return count;
}

void Cave::setLimits(const int limitBirth, const int limitDeath) {
  if (!checkLimit(limitBirth, limitDeath))
    throw std::invalid_argument("wrong limit parameters: " + limitBirth +
                                limitDeath);
  this->limitBirth = limitBirth;
  this->limitDeath = limitDeath;
}

std::vector<std::string> Cave::readFile(const std::string& filePath) const {
  std::fstream file(filePath);
  if (!file.is_open())
    throw std::invalid_argument("file didn't open: " + filePath);
  std::string line;
  std::vector<std::string> fileData;
  while (getline(file, line)) {
    fileData.push_back(line);
  }
  file.close();
  return fileData;
}

void Cave::parseFile(const std::vector<std::string>& fileData) {
  if (!fileData.size()) throw std::invalid_argument("read file is empty");
  std::vector<std::string> rowColLine;
  std::stringstream ss(fileData.at(0));
  std::string item;
  while (getline(ss, item, ' ')) rowColLine.push_back(item);
  int rows_tmp = atoi(rowColLine.at(0).c_str());
  int cols_tmp = atoi(rowColLine.at(1).c_str());
  if (!checkSize(rows_tmp, cols_tmp))
    throw std::invalid_argument("wrong matrix size parameters: " + rows_tmp +
                                cols_tmp);
  rows = rows_tmp;
  cols = cols_tmp;
  cave.reserve(rows);
  for (int i = 1, j = 0; i < rows + 1; ++i, ++j) {
    std::vector<int> line;
    for (int k = 0, n = 0;
         k < static_cast<int>(fileData.at(i).size()) && n < cols; ++k) {
      if (fileData.at(i).at(k) == ' ') continue;
      if (!std::isdigit(static_cast<unsigned char>(fileData.at(i).at(k)))) {
        cave.clear();
        throw std::invalid_argument("wrong symbol in matrix: " +
                                    fileData.at(i).at(k));
      }
      line.push_back(fileData.at(i).at(k) - '0');
      ++n;
    }
    cave.push_back(line);
  }
}

void Cave::initFromFile(const std::string& filePath) {
  if (filePath == "") throw std::invalid_argument("empty file path");
  cave.clear();
  parseFile(readFile(filePath));
}

void Cave::initRandom(const int rows, const int cols, const int chance) {
  if (!checkSize(rows, cols))
    throw std::invalid_argument("wrong matrix size parameters: " + rows + cols);
  cave.clear();
  this->rows = rows;
  this->cols = cols;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution distrib(1, 100);
  for (int i = 0; i < rows; ++i) {
    std::vector<int> line;
    for (int k = 0; k < cols; ++k) {
      distrib(gen) <= chance ? line.push_back(1) : line.push_back(0);
    }
    cave.push_back(line);
  }
}

bool Cave::checkSize(const int& row, const int& col) const noexcept {
  if (row < minCaveSizeRowCol || row > maxCaveSizeRow ||
      col < minCaveSizeRowCol || col > maxCaveSizeCol)
    return false;
  return true;
}

bool Cave::checkLimit(const int& birth, const int& death) const noexcept {
  if (birth < minDeathBirthLimit || death < minDeathBirthLimit ||
      birth > maxDeathBirthLimit || death > maxDeathBirthLimit)
    return false;
  return true;
}

}  // namespace s21
