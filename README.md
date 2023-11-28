# Project Title: War and Peace Analysis

This project analyzes the chapters of "War and Peace" by Leo Tolstoy, categorizing them as war-related or peace-related based on the density of specific terms.

---

## Requirements

- C++ Compiler (g++ or similar)
- C++11 Standard (or later)

---

## Compiling the Project

### 1. Using g++

Open a terminal (Command Prompt on Windows, Terminal on macOS/Linux) and navigate to the project directory.
Run the following command:
```bash
g++ -std=c++11 -o WarAndPeaceAnalysis FPROG_war_and_peace.cpp
```

Replace `FPROG_war_and_peace.cpp` with the actual name of your source files. The `-std=c++11` flag ensures that the C++11 standard is used, which may be necessary for certain features of the program.

### 2. Using an IDE

If you are using an Integrated Development Environment (IDE) like Visual Studio, Code::Blocks, or CLion, import the project and use the IDE's build system to compile.

#### 2.1. Creating a new cMake Project in Visual Studio (2022) (recommended)

Create a new cMake Project in VS2022, copy the `FPROG_war_and_peace.cpp` file and create the files folder appropriately with the needed .txt files.

---

## Running the Program

After successful compilation, an executable file named `WarAndPeaceAnalysis` (or `WarAndPeaceAnalysis.exe` on Windows) will be created.

### On Windows

Double-click on `WarAndPeaceAnalysis.exe` in the file explorer, or run it from the command prompt:

```shell
WarAndPeaceAnalysis.exe
```

### On macOS/Linux

Open a terminal and run:

```bash
./WarAndPeaceAnalysis
```

Ensure that the text files (e.g., `war_and_peace.txt`, `war_terms.txt`, `peace_terms.txt`) are located in the correct directory relative to the executable, as expected by the program.

---

## Notes

- The program expects specific input files and may need to be modified if the file names or formats are different.
- For detailed documentation, refer to comments within the source code.

---

## Troubleshooting

- If you encounter any compilation errors, make sure your compiler supports the C++11 standard (or later).
- Ensure that all necessary files are present and correctly named in the project directory.
