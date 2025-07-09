#ifndef SYSTEM_TOOLS_CSV_H
#define SYSTEM_TOOLS_CSV_H


#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace System::Tools {
    class CSV {
    private:
        std::vector<std::vector<std::string>> data;
		char delimiter = ',';
		bool HasHeader = false;
		bool HasFieldsEnclosedInQuotes = true;
		bool TrimWhiteSpace = true;
    public:
		CSV(){
			delimiter = ',';
			HasHeader = false;
			HasFieldsEnclosedInQuotes = true;
			TrimWhiteSpace = true;
		}
		void SetDelimiter(char newDelimiter) {
			delimiter = newDelimiter;
		}
		void Load(const std::string& filename) {
			std::ifstream file(filename);
			if (!file.is_open())
				throw std::runtime_error("Failed to open file: " + filename);

			std::string line;
			std::vector<std::string> row;
			std::string cell;
			bool inQuote = false;

			while (std::getline(file, line)){
				if (!line.empty() && line.back() == '\r') line.pop_back();

				for (size_t i = 0; i < line.size(); ++i) {
					char c = line[i];

					if (c == '\"') {
						if (inQuote && i + 1 < line.size() && line[i + 1] == '\"') {
							cell += '\"'; // Escaped quote
							++i;
						} else {
							inQuote = !inQuote; // Toggle quote context
						}
					} else if (c == delimiter && !inQuote) {
						row.push_back(cell);
						cell.clear();
					} else {
						cell += c;
					}
				}

				if (inQuote) {
					cell += '\n';
					continue;
				}

				row.push_back(cell);
				data.push_back(row);
				row.clear();
				cell.clear();
				inQuote = false;
			}

			// Handle unfinished quoted cell if file ends mid-quote
			if (!cell.empty() || !row.empty()) {
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