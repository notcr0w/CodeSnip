# codesnip

`codesnip` is a minimalist command-line tool for managing and reusing code snippets and templates. Snippets are stored in plain-text files, each containing multiple templates, and can be inserted or extracted from your source code files with simple commands.

---

## Features

- Store multiple named templates in a single snippet file
- Insert templates into any file at a specific line
- Extract lines from your code and save them as new templates
- List, show, delete, and rename templates

---

## Snippet File Format

Each snippet is enclosed between `#-- name: <template_name>` and `#-- end`. Multiple snippets can be stored in a single `.txt` file.

Example (`cpp_snippets.txt`):

```cpp
#-- name: for_loop
for (int i = 0; i < n; ++i) {
    // TODO
}
#-- end

#-- name: class_template
class MyClass {
public:
    MyClass();
    ~MyClass();
private:
    int x;
};
#-- end
```

---

## Command Usage

# Insert a snippet into a file at a given line
codesnip insert --name <template_name> --into <target_file> --at <line_number> --file <snippet_file>

# Extract lines from a file and save as a new snippet
codesnip extract --from <source_file> --lines <start_line>-<end_line> --name <new_template_name> --file <snippet_file>

# List all available templates in a snippet file
codesnip list --file <snippet_file>

# View the contents of a specific template
codesnip show --name <template_name> --file <snippet_file>

# Delete a template by name
codesnip delete --name <template_name> --file <snippet_file>

# Rename a template
codesnip rename --name <old_name> --to <new_name> --file <snippet_file>

# (Optional) Preview snippet insertion without modifying anything
codesnip insert --name <template_name> --into <target_file> --at <line_number> --file <snippet_file> --dry-run

