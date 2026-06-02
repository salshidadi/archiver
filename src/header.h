#define MAGIC_NUMBER 0x41524348
#define VERSION_NUMBER 1

typedef struct
{
    unsigned int magic_number;
    unsigned int version_number;
    unsigned int files_count;
} ArchiveHeader;

typedef struct
{
    unsigned int file_size;
    unsigned int filename_size;
    char *filename;
} FilesHeader;

typedef struct {
    ArchiveHeader arc_header;
    FilesHeader *files_headers;
    unsigned char **payloads_arrays; // pointer to a set of pointers for the files raw data blocks
} ArchiveContext;