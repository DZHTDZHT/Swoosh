#pragma once
#include <vector>
#include <fstream>

#include <iostream>

struct save {
  std::vector<std::string> names;
  std::vector<long> scores;

  const bool empty() { return names.empty(); }

  void writeToFile(std::string path) {
    std::ofstream outfile(path);

    if (!outfile) { return;  }

    if (names.empty()) {
      for (int i = 0; i < 5; i++) {
        char buffer[4];
        strcpy_s(buffer, "AAA\0");
        outfile.write(buffer, 4);
        outfile << 0;
      }
    }
    else {
      for (int i = 0; i < names.size(); i++) {
        char buffer[4];
        strcpy_s(buffer, 4, names[i].c_str());
        buffer[3] = '\0';
        outfile.write(buffer, 4);
        outfile.write((char*)scores[i], sizeof(long));
      }
    }

    outfile.close();
  }

  void loadFromFile(std::string path) {
    std::ifstream infile(path);
    char name[4];
    int score;

    if (!infile) { return; }
   
    while (infile) {
      infile.read(name, 4);
      infile >> score;

      std::cout << name << ", " << score << std::endl;
      names.push_back(name);
      scores.push_back(score);
    }

    infile.close();
  }
};