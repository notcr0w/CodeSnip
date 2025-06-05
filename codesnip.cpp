#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Overwrites a specific line in a file with a set of new lines, preserving indentation.
 */

 void file_overwrite(std::string& target_file, std::vector<std::string>& lines, int line_number) {
    // Open the target file for reading
    std::ifstream target_input(target_file);
    std::vector<std::string> target_lines;
    std::string line;

    // Check if the file opened successfully
    if (!target_input.is_open()) {
        std::cerr << "Error opening target file: " << target_file << std::endl;
        return;
    }

    // Read each line from the target file into a vector
    while (std::getline(target_input, line)) {
        target_lines.push_back(line);
    }
    target_input.close();

    // If the file is shorter than the desired line number, pad it with empty lines
    while ((int)target_lines.size() < line_number) {
        target_lines.push_back("");
    }

    // Determine the indentation of the line to be overwritten
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

    // Apply the same indentation to all new lines to maintain visual consistency
    for (int i = 0; i < (int)lines.size(); ++i) {
        lines[i] = indent + lines[i];
    }

    // Remove the original line and insert the new lines in its place
    target_lines.erase(target_lines.begin() + (line_number - 1));
    for (int i = 0; i < (int)lines.size(); ++i) {
        target_lines.insert(target_lines.begin() + (line_number - 1 + i), lines[i]);
    }

    // Open the target file again, this time for writing the updated content
    std::ofstream target_output(target_file);
    if (!target_output.is_open()) {
        std::cerr << "Error writing to target file: " << target_file << std::endl;
        return;
    }

    // Write all modified lines back into the file
    for (int i = 0; i < (int)target_lines.size(); ++i) {
        target_output << target_lines[i] << '\n';
    }

    target_output.close();
}


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

    // Overwrite the target file with the snippet at the specified line
    file_overwrite(target_file, snippet_lines, line_number);
    
    // Output a success message
    std::cout << "Inserted snippet '" << template_name << "' into "
              << target_file << " at line " << line_number << "." << std::endl;
}

void extract(std::string& source_file, int start_line,
int end_line, std::string& new_template_name, std::string& snippet_file) {
    int line_number = 0;
    std::ifstream source_input(source_file);
    if (!source_input.is_open()) {
        std::cerr << "Error opening source file: " << source_file << std::endl;
        return;
    }

    std::vector<std::string> extracted_lines;
    std::string line;

    while (std::getline(source_input, line)) {
        line_number++;
        if (line_number >= start_line && line_number <= end_line) {
            extracted_lines.push_back(line);
        }
    }

    if (extracted_lines.empty()) {
        std::cerr << "No lines extracted from " << source_file
                  << " for the specified range." << std::endl;
        source_input.close();
        return;
    }

    if (extracted_lines.size() != (end_line - start_line + 1)) {
        std::cerr << "Extracted lines do not match the specified range." << std::endl;
        return;
    }

    source_input.close();

    // std::ofstream snippet_output(snippet_file, )
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
            std::cerr << "Usage: " << argv[0] << " extract <source_file> <start_line> <end_line> <new_template_name> <snippet_file>" << std::endl;
            return 1;
        }

        std::string source_file = argv[2];
        if (source_file.empty()) {
            std::cerr << "Source file cannot be empty." << std::endl;
            return 1;
        }

        int start_line = std::stoi(argv[3]);
        int end_line = std::stoi(argv[4]);
        if (start_line < 1 || end_line < start_line || start_line > end_line) {
            std::cerr << "Invalid line range." << std::endl;
            return 1;
        }

        std::string new_template_name = argv[5];
        if (new_template_name.empty()) {
            std::cerr << "New template name cannot be empty." << std::endl;
            return 1;
        }

        std::string snippet_file = argv[6];

        extract(source_file, start_line, end_line, new_template_name, snippet_file);
    }
    
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }


    return 0;
}