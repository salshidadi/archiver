# Archiver Utility

## Project Overview

The Archiver Utility is a lightweight, low-level command-line tool written in C for packing, managing, and extracting multiple files within a single custom binary archive. Designed with a focus on manual memory management and binary file structures, it reads and writes raw metadata headers and variable data payloads directly to disk. 

The utility utilizes a custom binary format characterized by a specific Magic Number (`0x41524348`) and versioning control, ensuring data integrity when managing file payloads and structure padding.

<img src="icon.png" width="50" alt="Icon" />
**(c) 2026 Team x86. All rights reserved.**

---

## Installation

### Prerequisites
To build and run this project, you will need a C compiler such as `gcc`.

### Step-by-Step Build Instructions

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Compile the source code:**
   Using `gcc`, compile the `archiver.c` file. The `header.h` file will be included automatically.
   ```bash
   gcc archiver.c -o archiver
   ```

3. **Verify the build:**
   Run the help command to ensure the utility was compiled successfully.
   ```bash
   ./archiver -h
   ```

---

## Usage Guide

The Archiver Utility operates via command-line flags. The general syntax is:
`./archiver <option> <archive_name> [arguments...]`

### Available Commands

* **Create a new empty archive:**
  ```bash
  ./archiver -c <archive_name>
  ```
* **Insert one or more files:**
  ```bash
  ./archiver -i <archive_name> <file1> [file2 ...]
  ```
* **List metadata for all contained files:**
  ```bash
  ./archiver -l <archive_name>
  ```
* **Extract a specific file:**
  ```bash
  ./archiver -x <archive_name> <file_to_extract>
  ```
* **Delete a file or more from the archive:**
  ```bash
  ./archiver -d <archive_name> <file1_to_remove> [file2_to_remove ...]
  ```
* **Replace an existing file with a new local file:**
  ```bash
  ./archiver -r <archive_name> <old_file> <new_file>
  ```
* **Print total number of files in the archive:**
  ```bash
  ./archiver -n <archive_name>
  ```
* **Show the help menu:**
  ```bash
  ./archiver -h
  ```

---
