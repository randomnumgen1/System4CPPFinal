#ifndef SYSTEM_TOOLS_CSV_H
#define SYSTEM_TOOLS_CSV_H


#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace System::Tools {
    class CSV {
    private:
        std::vector<std::vector<std::string>> data;

    public:
        void Load(const std::string& filename) {
            std::ifstream file(filename);
            std::string line;
            bool inQuote = false;

            while (std::getline(file, line)) {
                std::vector<std::string> row;
                std::string cell;
                for (size_t i = 0; i < line.size(); ++i) {
                    char c = line[i];

                    if (c == '\"') {
                        inQuote = !inQuote;
                    } else if (c == ',' && !inQuote) {
                        row.push_back(cell);
                        cell.clear();
                    } else {
                        cell += c;
                    }
                }
                row.push_back(cell);
                data.push_back(row);
            }
        }

        const std::vector<std::vector<std::string>>& GetData() const {
            return data;
        }
    };
}


#endif