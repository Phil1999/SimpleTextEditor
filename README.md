# SimpleTextEditor

A lightweight, Qt-based text editor with basic file operations and a clean interface.

## Features

- Create, open, and save text files
- Basic text editing operations (cut, copy, paste, undo/redo, select all, delete selected)
- Unsaved changes detection
- Cross-platform compatibility (thanks to Qt)

## Getting Started

### Prerequisites

- Qt 5.15 or later
- C++17 compatible compiler
- Visual Studio 2019 or later (for Windows development)

### Building the Project

1. Open the `SimpleTextEditor.sln` file in Visual Studio.
2. Build the solution (F7 or Ctrl+Shift+B).
3. Run the application (F5).

### Running Tests

The project includes a separate test project using Google Test.

1. Ensure Google Test is properly set up in your environment.
2. Build the `SimpleTextEditorTests` project.
	- You need a copy of the SimpleTextEditor source files in the project as well (design constraint).

## Project Structure

- `SimpleTextEditor/`: Main application project
  - `SimpleTextEditor.h/.cpp`: Core editor functionality
  - `main.cpp`: Application entry point
- `SimpleTextEditorTests/`: Test project
  - `test.cpp`: Test cases for the editor

## Design Constraints and Future Improvements

The current design, where there are separate projects for the main application and tests, can be tedious for testing. A potential improvement would be to implement a 
core library that both the main application and tests can reference. Using Cmake as a build system could also be beneficial.


## Roadmap
This project will likely stay simple for awhile, but I will come back to it from time to time to introduce more features.

- [ ] Spell checking
- [ ] Syntax highlighting for popular programming languages
- [ ] Find and replace functionality
- [ ] Multi-tab interface for editing multiple files
- [ ] Auto-save feature
- [ ] Line numbering and word count
- [ ] Customizable themes (light/dark mode)
- [ ] Undo/Redo functionality
- [ ] Code folding for programming languages
- [ ] Split-view editing (tabs)
- [ ] Auto-completion and code snippets
- [ ] File comparison tool


