# Header

| Name | Description |
|------|-------------|
| [`FilesHeader`](#filesheader) | Represents the metadata for a single file within the archive. |
| [`ArchiveHeader`](#archiveheader) | Represents the main header of the archive file. |
| [`ArchiveContext`](#archivecontext) | In-memory representation of the entire loaded archive. |

## Macros

#### MAGIC_NUMBER

```cpp
MAGIC_NUMBER()
```

The magic number used to identify valid archive files (ASCII for "ARCH").

#### VERSION_NUMBER

```cpp
VERSION_NUMBER()
```

The current version of the archive format.

## Functions

#### get_file_size

```cpp
int get_file_size(char * filename)
```

Retrieves the size of a file on the disk.

* 
#### Parameters
* `filename` The path to the file. 

#### Returns
The size of the file in bytes, or -1 if the file cannot be accessed.

#### initiate_archive_header

```cpp
void initiate_archive_header(ArchiveHeader * arc_header)
```

Initializes a new archive header with default values.

* 
#### Parameters
* `arc_header` Pointer to the [ArchiveHeader](#archiveheader) to initialize.

#### print_archive_header

```cpp
void print_archive_header(ArchiveHeader * arc_header)
```

Prints the main archive metadata to the standard output.

* 
#### Parameters
* `arc_header` Pointer to the [ArchiveHeader](#archiveheader) to print.

#### print_file_header

```cpp
void print_file_header(FilesHeader * file_header)
```

Prints the metadata of a single file stored in the archive.

* 
#### Parameters
* `file_header` Pointer to the [FilesHeader](#filesheader) to print.

#### print_archive_data

```cpp
void print_archive_data(ArchiveContext * archive)
```

Prints the full metadata of the archive and all its contained files.

* 
#### Parameters
* `archive` Pointer to the loaded [ArchiveContext](#archivecontext).

#### load_archive

```cpp
int load_archive(char * archive_filename, ArchiveContext * archive)
```

Loads an archive file from disk into memory.

* 
#### Parameters
* `archive_filename` The path to the archive file to read. 

* `archive` Pointer to the [ArchiveContext](#archivecontext) structure to populate. 

#### Returns
0 on success, 1 on failure (e.g., file not found).

#### insert_file

```cpp
int insert_file(ArchiveContext * archive, FilesHeader * file_header, unsigned char * file)
```

Appends a new file into the in-memory archive context.

* 
#### Parameters
* `archive` Pointer to the active [ArchiveContext](#archivecontext). 

* `file_header` Pointer to the [FilesHeader](#filesheader) containing the new file's metadata. 

* `file` Pointer to the raw payload data of the new file. 

#### Returns
0 on success, 1 on failure (e.g., memory allocation error).

#### save_archive

```cpp
int save_archive(char * archive_filename, ArchiveContext * archive)
```

Writes the current in-memory archive context back to disk.

* 
#### Parameters
* `archive_filename` The path where the archive should be saved. 

* `archive` Pointer to the active [ArchiveContext](#archivecontext) to serialize. 

#### Returns
0 on success, 1 on failure (e.g., cannot open file for writing).

#### free_pointers

```cpp
void free_pointers(ArchiveContext * archive)
```

Frees all dynamically allocated memory associated with an [ArchiveContext](#archivecontext).

* 
#### Parameters
* `archive` Pointer to the [ArchiveContext](#archivecontext) to clean up.

#### targeted_index

```cpp
int targeted_index(ArchiveContext * archive, char * filename)
```

Finds the index of a specific file within the archive.

* 
#### Parameters
* `archive` Pointer to the loaded [ArchiveContext](#archivecontext). 

* `filename` The name of the file to search for. 

#### Returns
The zero-based index of the file, or -1 if the file is not found.

#### delete_file

```cpp
int delete_file(char * archive_filename, ArchiveContext * archive, char * filename)
```

Deletes a specific file from the in-memory archive.

* 
#### Parameters
* `archive_filename` The path to the archive file (unused internally, kept for signature). 

* `archive` Pointer to the loaded [ArchiveContext](#archivecontext). 

* `filename` The name of the file to remove. 

#### Returns
0 on success, 1 on failure (e.g., file not found).

#### extract_file

```cpp
int extract_file(ArchiveContext * archive, char * filename)
```

Extracts a specific file from the archive to the local disk.

* 
#### Parameters
* `archive` Pointer to the loaded [ArchiveContext](#archivecontext). 

* `filename` The name of the file to extract. 

#### Returns
0 on success, 1 on failure (e.g., file not found or write error).

#### main

```cpp
int main(int argc, char * argv)
```

Main entry point for the Archiver Utility.

* 
#### Parameters
* `argc` The number of command-line arguments. 

* `argv` Array of command-line argument strings. 

#### Returns
0 on normal execution, 1 on error.



## FilesHeader

```cpp
#include <header.h>
```

```cpp
struct FilesHeader
```

Defined in header.h:34

Represents the metadata for a single file within the archive.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`file_size`](#file_size) | `variable` | Declared here |
| [`filename_size`](#filename_size) | `variable` | Declared here |
| [`filename`](#filename) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned int` | [`file_size`](#file_size)  | Size of the file payload in bytes. |
| `unsigned int` | [`filename_size`](#filename_size)  | Length of the filename string (excluding null terminator). |
| `char *` | [`filename`](#filename)  | Pointer to a dynamically allocated string containing the filename. |

---

#### file_size

```cpp
unsigned int file_size
```

Defined in header.h:37

Size of the file payload in bytes.

---

#### filename_size

```cpp
unsigned int filename_size
```

Defined in header.h:39

Length of the filename string (excluding null terminator).

---

#### filename

```cpp
char * filename
```

Defined in header.h:41

Pointer to a dynamically allocated string containing the filename.



## ArchiveHeader

```cpp
#include <header.h>
```

```cpp
struct ArchiveHeader
```

Defined in header.h:20

Represents the main header of the archive file.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`magic_number`](#magic_number-1) | `variable` | Declared here |
| [`version_number`](#version_number-1) | `variable` | Declared here |
| [`files_count`](#files_count) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned int` | [`magic_number`](#magic_number-1)  | Unique identifier for the archive format. |
| `unsigned int` | [`version_number`](#version_number-1)  | Version of the archive format. |
| `unsigned int` | [`files_count`](#files_count)  | Total number of files stored in the archive. |

---

#### magic_number

```cpp
unsigned int magic_number
```

Defined in header.h:23

Unique identifier for the archive format.

---

#### version_number

```cpp
unsigned int version_number
```

Defined in header.h:25

Version of the archive format.

---

#### files_count

```cpp
unsigned int files_count
```

Defined in header.h:27

Total number of files stored in the archive.



## ArchiveContext

```cpp
#include <header.h>
```

```cpp
struct ArchiveContext
```

Defined in header.h:48

In-memory representation of the entire loaded archive.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`arc_header`](#arc_header) | `variable` | Declared here |
| [`files_headers`](#files_headers) | `variable` | Declared here |
| [`payloads_arrays`](#payloads_arrays) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ArchiveHeader` | [`arc_header`](#arc_header)  | The main archive header. |
| `FilesHeader *` | [`files_headers`](#files_headers)  | Array of headers for each file in the archive. |
| `unsigned char **` | [`payloads_arrays`](#payloads_arrays)  | Array of pointers, where each pointer points to a file's raw data block. |

---

#### arc_header

```cpp
ArchiveHeader arc_header
```

Defined in header.h:50

The main archive header.

---

#### files_headers

```cpp
FilesHeader * files_headers
```

Defined in header.h:52

Array of headers for each file in the archive.

---

#### payloads_arrays

```cpp
unsigned char ** payloads_arrays
```

Defined in header.h:54

Array of pointers, where each pointer points to a file's raw data block.
