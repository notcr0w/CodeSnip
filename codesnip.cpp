#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Inserts a named snippet from a snippet file into a specific line of a target file.
 * Preserves indentation by detecting the target line's formatting, pads lines if necessary,
 * and replaces the line at the specified location with the snippet content.
 */
void insert(std::string& snippet_file, std::string& target_file,
std::string& template_name, int line_number) {
    // Open the snippet file for reading
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }

    std::vector<std::string> snippet_lines;
    std::string line;
    bool found = false;

    // Search for the template by name and extract its lines
    while (std::getline(snippet_input, line)) {
        if (line == "#-- name: " + template_name) {
            found = true;
            continue;
        }

        if (found) {
            if (line == "#-- end") {
                break;
            }
            snippet_lines.push_back(line);
        }
    }

    snippet_input.close();

    // If the template wasn't found or was empty, exit
    if (!found) {
        std::cerr << "Template '" << template_name << "' not found in snippet file." << std::endl;
        return;
    }

    if (snippet_lines.empty()) {
        std::cerr << "No lines found for template '" << template_name << "'." << std::endl;
        return;
    }

    // Read the target file line-by-line into a vector
    std::ifstream target_input(target_file);
    std::vector<std::string> target_lines;

    if (target_input.is_open()) {
        while (std::getline(target_input, line)) {
            target_lines.push_back(line);
        }
        target_input.close();
    }

    // Pad the target file with empty lines if it's too short
    while ((int)target_lines.size() < line_number) {
        target_lines.push_back("");
    }

    // Detect indentation of the target line to preserve formatting
    std::string indent = "";
    if (!target_lines[line_number - 1].empty()) {
        std::string& target_line = target_lines[line_number - 1];
        for (int i = 0; i < (int)target_line.size(); ++i) {
            if (target_line[i] == ' ' || target_line[i] == '\t') {
                indent += target_line[i];
            } else {
                break;
            }
        }
    }

    // Prepend indentation to each line of the snippet
    for (int i = 0; i < (int)snippet_lines.size(); ++i) {
        snippet_lines[i] = indent + snippet_lines[i];
    }

    // Overwrite the original line and insert the snippet in its place
    target_lines.erase(target_lines.begin() + (line_number - 1));
    for (int i = 0; i < (int)snippet_lines.size(); ++i) {
        target_lines.insert(target_lines.begin() + (line_number - 1 + i), snippet_lines[i]);
    }

    // Write the modified content back to the target file
    std::ofstream target_output(target_file);
    if (!target_output.is_open()) {
        std::cerr << "Error writing to target file: " << target_file << std::endl;
        return;
    }

    for (int i = 0; i < (int)target_lines.size(); ++i) {
        target_output << target_lines[i] << '\n';
    }

    target_output.close();
}

void extract(std::string& snippet_file, std::string& target_file,
std::string& template_name, int line_number) {
    std::ifstream target_input(target_file);
    if (!target_input.is_open()) {
        std::cerr << "Error opening target file: " << target_file << std::endl;
        return;
    }

    std::vector<std::string> target_lines;
    std::string line;

    while (std::getline(target_input, line)) {
        target_lines.push_back(line);
    }

    target_input.close();

    if (line_number < 1 || line_number > (int)target_lines.size()) {
        std::cerr << "Line number out of range." << std::endl;
        return;
    }

    std::ofstream snippet_output(snippet_file, std::ios_base::app);
    if (!snippet_output.is_open()) {
        std::cerr << "Error writing to snippet file: " << snippet_file << std::endl;
        return;
    }

    snippet_output << "#-- name: " << template_name << '\n';
    snippet_output << target_lines[line_number - 1] << '\n';
    snippet_output << "#-- end\n";

    snippet_output.close();
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 1;
    }

    std::string command(argv[1]);

    if (command == "insert") {
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0] << " insert <template_name> <target_file> <line_number> <snippet_file>" << std::endl;
            return 1;
        }

        std::string snippet_file = argv[5];
        std::string target_file = argv[3];
        std::string template_name = argv[2];
        int line_number = std::stoi(argv[4]);

        insert(snippet_file, target_file, template_name, line_number);
    }
    
    else if (command == "extract") {
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0] << " extract <template_name> <target_file> <line_number> <snippet_file>" << std::endl;
            return 1;
        }

        std::string snippet_file = argv[5];
        std::string target_file = argv[3];
        std::string template_name = argv[2];
        int line_number = std::stoi(argv[4]);

        extract(snippet_file, target_file, template_name, line_number);
    }
    
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }


    return 0;
}