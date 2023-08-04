// Copyright 2023 Zhu Junhui

#pragma once

#include <fstream>
#include <string>

namespace Pascal {

std::string read_file(const std::string& filename) {
  std::fstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::string text;
  // read file into text

  while (true) {
    std::string line;
    std::getline(file, line);
    text += line;
    if (file.eof()) {
      break;
    }
  }

  return text;
}
}  // namespace Pascal
