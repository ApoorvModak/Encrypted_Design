#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void writeDataToCSV(const std::vector<std::pair<unsigned char*, int>>& data, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& entry : data) {
        for (int i = 0; i < 16; ++i) {
            file << static_cast<int>(entry.first[i]); // Writing the character array as integer values
            if(i<15){
                file << ' ';
            }
        }
        file << "," << entry.second << "\n";
    }

    file.close();
}

int main() {
    std::vector<std::pair<unsigned char*, int>> data = {
        {new unsigned char[16]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'}, 42},
        {new unsigned char[16]{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f'}, 99},
        // Add more data entries as needed
    };

    // Open the file in truncation mode to erase the existing content
    std::ofstream existingFile("inverted_index.csv", std::ofstream::trunc);
    existingFile.close();

    writeDataToCSV(data, "inverted_index.csv");

    // Don't forget to deallocate the dynamically allocated memory
    for (auto& entry : data) {
        delete[] entry.first;
    }

    return 0;
}
