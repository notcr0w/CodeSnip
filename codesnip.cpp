#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Overwrites a specific line in a target file with a given set of lines.
 *
 * Reads the target file into memory, pads it with empty lines if needed, and replaces the line at the
 * specified position with the given lines. The inserted lines are automatically indented to match the
 * indentation of the original line being replaced. The modified content is then written back to the file.
 *
 * @param target_file The path to the file to modify.
 * @param lines A vector of strings to insert into the file.
 * @param line_number The 1-based line number where the insertion should occur.
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
 * @brief Inserts a named code snippet into a target file at a specified line number.
 *
 * This function reads a code snippet from the given snippet file using a defined template format,
 * then inserts the snippet into the target file at the specified line number. If the target file
 * has fewer lines than the insertion point, it is padded with empty lines. The inserted snippet
 * is automatically indented to match the target line's existing indentation.
 *
 * Snippets in the snippet file must be enclosed between:
 *   #-- name: <template_name>
 *   ... (snippet content) ...
 *   #-- end
 *
 * @param snippet_file The file path containing named code snippets.
 * @param target_file The file path to insert the snippet into.
 * @param template_name The name of the snippet template to insert.
 * @param line_number The line number in the target file to insert the snippet (1-based index).
 *
 * @note
 * - If the template is not found or contains no content, the function exits with an error message.
 * - If the target file doesn't exist or is empty, it will be created or padded accordingly.
 * - The original line at the insertion point is overwritten by the snippet.
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

/**
 * @brief Extracts a range of lines from a source file and saves them as a new named template
 *        in a snippet file, using a standard format with header and end markers.
 *
 * The function checks for errors such as missing files, invalid ranges, or duplicate template names.
 * It appends the extracted content to the end of the snippet file, formatted with `#-- name:` and `#-- end` markers.
 *
 * @param source_file Path to the source file to extract lines from.
 * @param start_line First line of the range to extract (1-based index).
 * @param end_line Last line of the range to extract (inclusive).
 * @param new_template_name Name of the new snippet template.
 * @param snippet_file Path to the snippet file where the template will be stored.
 */

void extract(std::string& source_file, int start_line,
int end_line, std::string& new_template_name, std::string& snippet_file) {
    int line_number = 0;

    // Opening and validating the source file for reading
    std::ifstream source_input(source_file);
    if (!source_input.is_open()) {
        std::cerr << "Error opening source file: " << source_file << std::endl;
        return;
    }

    std::vector<std::string> extracted_lines;
    std::string line;

    // Adding header line
    extracted_lines.push_back("#-- name: " + new_template_name);

    // Adding lines within the specified range
    while (std::getline(source_input, line)) {
        line_number++;
        if (line_number >= start_line && line_number <= end_line) {
            extracted_lines.push_back(line);
        }
    }

    // Add the end line to the extracted lines
    extracted_lines.push_back("#-- end");

    // Check if any lines were extracted
    if (extracted_lines.size() == 2) { // Only the header and end lines
        std::cerr << "No lines extracted from " << source_file
                  << " for the specified range." << std::endl;
        source_input.close();
        return;
    }

    // If the number of extracted lines does not match the expected range, report an error
    if ((int)extracted_lines.size() != (end_line - start_line + 3)) { // +3 for header and end lines
        std::cerr << "Extracted lines do not match the specified range." << std::endl;
        return;
    }

    source_input.close();

    line_number = 0;
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }
    
    // Check if the template already exists in the snippet file
    while (std::getline(snippet_input, line)) {
        line_number++;
        if (line == "#-- name: " + new_template_name) {
            std::cerr << "Template '" << new_template_name << "' already exists in snippet file." << std::endl;
            snippet_input.close();
            return;
        }
    }

    snippet_input.close();

    file_overwrite(snippet_file, extracted_lines, line_number + 2); // +2 to account for the header and end lines

    // Output a success message
    std::cout << "Extracted lines from " << source_file
              << " and saved as template '" << new_template_name
              << "' in snippet file " << snippet_file << "." << std::endl;
}

/**
 * @brief Lists all template names stored in the given snippet file.
 *
 * Searches for lines starting with "#-- name: " and prints the name of each template.
 * If no templates are found, prints a message indicating so.
 *
 * @param snippet_file Path to the snippet file to read from.
 */

 void list_templates(std::string& snippet_file) {
    // Open the snippet file for reading
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }

    std::string line;
    bool found = false;

    // Read the file line by line
    while (std::getline(snippet_input, line)) {
        // Look for lines that define a template name
        if (line.find("#-- name: ") == 0) {
            found = true;
            // Print just the template name (omit the "#-- name: " prefix)
            std::cout << line.substr(10) << std::endl;
        }
    }

    // If no template markers were found, print a message
    if (!found) {
        std::cout << "No templates found in " << snippet_file << "." << std::endl;
    }

    // Close the file
    snippet_input.close();
}

/**
 * @brief Displays the contents of a specific template from a snippet file.
 *
 * Searches for the specified template name in the snippet file and prints its
 * entire content, including the `#-- name:` header and all lines up to (but not including)
 * the `#-- end` marker. If the template does not exist, an error message is shown.
 *
 * @param template_name The name of the template to display.
 * @param snippet_file Path to the snippet file containing the templates.
 */

void show(std::string& template_name, std::string& snippet_file) {
    // Open the snippet file for reading
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }

    std::string line;
    bool found = false;

    // Search for the template by name and print its lines
    while (std::getline(snippet_input, line)) {
        if (line == "#-- name: " + template_name) {
            found = true;
            std::cout << line << std::endl; // Print the header line
            continue;
        }

        if (found) {
            if (line == "#-- end") {
                break; // Stop reading when we reach the end marker
            }
            std::cout << line << std::endl; // Print the content of the template
        }
    }

    snippet_input.close();

    // If the template wasn't found, print an error message
    if (!found) {
        std::cerr << "Template '" << template_name << "' not found in snippet file." << std::endl;
    }
}

/**
 * @brief Deletes a specified template from a snippet file.
 *
 * Scans the snippet file for a template with the given name and removes its entire content,
 * including the `#-- name:` header and `#-- end` marker. All other lines are preserved.
 * If the template is not found, an error message is displayed. The file is then overwritten
 * with the remaining content.
 *
 * @param template_name The name of the template to delete.
 * @param snippet_file Path to the snippet file to update.
 */

void delete_template(std::string& template_name, std::string& snippet_file) {
    // Open the snippet file for reading
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    // Read the file line by line
    while (std::getline(snippet_input, line)) {
        if (line == "#-- name: " + template_name) {
            found = true; // Template found, skip it
            while (std::getline(snippet_input, line) && line != "#-- end") {
                // Skip lines until we reach the end marker
            }
            continue; // Skip the end marker as well
        }
        lines.push_back(line); // Keep other lines
    }

    snippet_input.close();

    // If the template was not found, print an error message
    if (!found) {
        std::cerr << "Template '" << template_name << "' not found in snippet file." << std::endl;
        return;
    }

    file_overwrite(snippet_file, lines, 1); // Overwrite the snippet file with the remaining lines
    
    // Output a success message
    std::cout << "Deleted template '" << template_name << "' from " << snippet_file << "." << std::endl;
}

/**
 * @brief Renames a template within a snippet file.
 *
 * Searches for a template with the specified old name in the snippet file and replaces
 * its `#-- name:` header with a new name. The rest of the template's content remains unchanged.
 * If the original template name is not found, an error message is displayed. The file is then
 * overwritten with the updated lines.
 *
 * @param old_template_name The current name of the template to rename.
 * @param new_template_name The new name to assign to the template.
 * @param snippet_file Path to the snippet file containing the template.
 */

void rename(std::string& old_template_name, std::string& new_template_name, std::string& snippet_file) {
    // Open the snippet file for reading
    std::ifstream snippet_input(snippet_file);
    if (!snippet_input.is_open()) {
        std::cerr << "Error opening snippet file: " << snippet_file << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    // Read the file line by line
    while (std::getline(snippet_input, line)) {
        if (line == "#-- name: " + old_template_name) {
            found = true; // Template found, rename it
            lines.push_back("#-- name: " + new_template_name);
            continue; // Skip the old name line
        }
        lines.push_back(line); // Keep other lines
    }

    snippet_input.close();

    // If the template was not found, print an error message
    if (!found) {
        std::cerr << "Template '" << old_template_name << "' not found in snippet file." << std::endl;
        return;
    }

    file_overwrite(snippet_file, lines, 1); // Overwrite the snippet file with the modified lines
    
    // Output a success message
    std::cout << "Renamed template '" << old_template_name << "' to '" << new_template_name 
              << "' in " << snippet_file << "." << std::endl;
}

int main(int argc, char* argv[]) {
    // Check if at least one argument (the command) is provided
    if (argc == 1) {
        return 1; // Exit early if no command is given
    }

    std::string command(argv[1]);

    // Handle 'insert' command
    if (command == "insert") {
        if (argc < 6) {
            // Print usage instructions if arguments are missing
            std::cerr << "Usage: " << argv[0] << " insert <template_name> <target_file> <line_number> <snippet_file>" << std::endl;
            return 1;
        }

        // Parse arguments
        std::string snippet_file = argv[5];
        std::string target_file = argv[3];
        std::string template_name = argv[2];
        int line_number = std::stoi(argv[4]);

        insert(snippet_file, target_file, template_name, line_number);
    }

    // Handle 'extract' command
    else if (command == "extract") {
        if (argc < 7) {
            std::cerr << "Usage: " << argv[0] << " extract <source_file> <start_line> <end_line> <new_template_name> <snippet_file>" << std::endl;
            return 1;
        }

        // Parse and validate line numbers
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

    // Handle 'list' command
    else if (command == "list") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " list <snippet_file>" << std::endl;
            return 1;
        }

        std::string snippet_file = argv[2];
        list_templates(snippet_file);
    }

    // Handle 'show' command
    else if (command == "show") {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " show <template_name> <snippet_file>" << std::endl;
            return 1;
        }

        std::string template_name = argv[2];
        std::string snippet_file = argv[3];
        show(template_name, snippet_file);
    }

    // Handle 'delete' command
    else if (command == "delete") {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " delete <template_name> <snippet_file>" << std::endl;
            return 1;
        }

        std::string template_name = argv[2];
        std::string snippet_file = argv[3];
        delete_template(template_name, snippet_file);
    }

    // Handle 'rename' command
    else if (command == "rename") {
        if (argc < 5) {
            std::cerr << "Usage: " << argv[0] << " rename <old_template_name> <new_template_name> <snippet_file>" << std::endl;
            return 1;
        }

        std::string old_template_name = argv[2];
        std::string new_template_name = argv[3];
        std::string snippet_file = argv[4];
        rename(old_template_name, new_template_name, snippet_file);
    }

    // Unknown command handler
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0; // Program executed successfully
}