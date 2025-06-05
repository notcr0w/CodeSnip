# codesnip

`codesnip` is a minimalist command-line tool for managing and reusing code snippets and templates. Snippets are stored in plain-text files, each containing multiple templates, and can be inserted or extracted from your source code files with simple commands.

---

## 📦 Features

- Store multiple named templates in a single snippet file
- Insert templates into any file at a specific line
- Extract lines from your code and save them as new templates
- List, show, delete, and rename templates
- A terminal and a text editor is all thats required.

---

## 📝 Snippet File Format

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
