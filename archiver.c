#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "header.h"

int get_file_size(char *filename)
{
    struct stat st;
    if (stat(filename, &st) == 0)
    {
        return st.st_size;
    }
    return -1;
}

void initiate_archive_header(ArchiveHeader *arc_header)
{
    arc_header->magic_number = MAGIC_NUMBER;
    arc_header->version_number = VERSION_NUMBER;
    arc_header->files_count = 0;
}

void print_archive_header(ArchiveHeader *arc_header)
{
#define CLR_RESET "\x1b[0m"
#define CLR_LABEL "\x1b[1;36m"

    printf("  " CLR_LABEL "%-24s" CLR_RESET "0x%X\n", "Archive Magic Number:", arc_header->magic_number);
    printf("  " CLR_LABEL "%-24s" CLR_RESET "%d\n", "Archive Version:", arc_header->version_number);
    printf("  " CLR_LABEL "%-24s" CLR_RESET "%d\n", "Total Files Count:", arc_header->files_count);

#undef CLR_RESET
#undef CLR_LABEL
}

void print_file_header(FilesHeader *file_header)
{
#define CLR_RESET "\x1b[0m"
#define CLR_LABEL "\x1b[33m"

    printf("  " CLR_LABEL "%-24s" CLR_RESET "%u bytes\n", "File Size:", file_header->file_size);
    printf("  " CLR_LABEL "%-24s" CLR_RESET "%u bytes\n", "Filename Length:", file_header->filename_size);
    printf("  " CLR_LABEL "%-24s" CLR_RESET "%s\n", "Filename:", file_header->filename);

#undef CLR_RESET
#undef CLR_LABEL
}

void print_archive_data(ArchiveContext *archive)
{
#define CLR_RESET "\x1b[0m"
#define CLR_HEADER "\x1b[1;32m"
#define CLR_BORDER "\x1b[1;30m"

    printf("\n" CLR_BORDER "==================================================\n" CLR_RESET);
    printf(CLR_HEADER "ARCHIVE METADATA\n" CLR_RESET);
    printf(CLR_BORDER "--------------------------------------------------\n" CLR_RESET);
    print_archive_header(&(archive->arc_header));

    if (archive->arc_header.files_count > 0)
    {
        printf("\n" CLR_BORDER "==================================================\n" CLR_RESET);
        printf(CLR_HEADER "CONTAINED FILES\n" CLR_RESET);

        for (int i = 0; i < archive->arc_header.files_count; i++)
        {
            printf(CLR_BORDER "--------------------------------------------------\n" CLR_RESET);
            printf(CLR_HEADER "File [#%d]\n" CLR_RESET, i + 1);
            print_file_header(archive->files_headers + i);
        }
    }

    printf(CLR_BORDER "==================================================\n\n" CLR_RESET);

#undef CLR_RESET
#undef CLR_HEADER
#undef CLR_BORDER
}

int load_archive(char *archive_filename, ArchiveContext *archive)
{
    FILE *fh = fopen(archive_filename, "rb");
    if (fh == NULL)
        return 1;

    fread(&(archive->arc_header), sizeof(ArchiveHeader), 1, fh);

    int count = archive->arc_header.files_count;

    if (count == 0)
    {
        archive->files_headers = NULL;
        archive->payloads_arrays = NULL;
        fclose(fh);
        return 0;
    }

    archive->files_headers = malloc(count * sizeof(FilesHeader));
    archive->payloads_arrays = malloc(count * sizeof(unsigned char *));

    for (int i = 0; i < count; i++)
    {
        fread(&(archive->files_headers[i].file_size), sizeof(unsigned int), 1, fh);
        fread(&(archive->files_headers[i].filename_size), sizeof(unsigned int), 1, fh);

        unsigned int name_len = archive->files_headers[i].filename_size;

        archive->files_headers[i].filename = malloc(name_len + 1);
        fread(archive->files_headers[i].filename, sizeof(char), name_len, fh);
        archive->files_headers[i].filename[name_len] = '\0';
    }

    for (int i = 0; i < count; i++)
    {
        int size = archive->files_headers[i].file_size;
        archive->payloads_arrays[i] = malloc(size);

        fread(archive->payloads_arrays[i], size, 1, fh);
    }

    fclose(fh);
    return 0;
}

int insert_file(ArchiveContext *archive, FilesHeader *file_header, unsigned char *file)
{

    int count = archive->arc_header.files_count;

    FilesHeader *tmp_file = realloc(archive->files_headers, (count + 1) * sizeof(FilesHeader));
    if (tmp_file == NULL)
        return 1;

    archive->files_headers = tmp_file;
    archive->files_headers[count] = *file_header;
    archive->files_headers[count].filename = strdup(file_header->filename);

    unsigned char **tmp_payload = realloc(archive->payloads_arrays, (count + 1) * sizeof(unsigned char *));
    if (tmp_payload == NULL)
        return 1;

    archive->payloads_arrays = tmp_payload;
    archive->payloads_arrays[count] = file;

    archive->arc_header.files_count++;
    return 0;
}

int save_archive(char *archive_filename, ArchiveContext *archive)
{
    int count = archive->arc_header.files_count;

    FILE *fh = fopen(archive_filename, "wb");
    if (fh == NULL)
        return 1;
    fwrite(&(archive->arc_header), sizeof(ArchiveHeader), 1, fh);

    for (int i = 0; i < count; i++)
    {
        fwrite(&(archive->files_headers[i].file_size), sizeof(unsigned int), 1, fh);
        fwrite(&(archive->files_headers[i].filename_size), sizeof(unsigned int), 1, fh);

        unsigned int name_len = archive->files_headers[i].filename_size;
        fwrite(archive->files_headers[i].filename, sizeof(char), name_len, fh);
    }

    for (int i = 0; i < count; i++)
    {
        unsigned int size = archive->files_headers[i].file_size;
        fwrite(archive->payloads_arrays[i], size, 1, fh);
    }

    fclose(fh);
    return 0;
}

void free_pointers(ArchiveContext *archive)
{
    if (archive->payloads_arrays)
    {
        for (int i = 0; i < archive->arc_header.files_count; i++)
        {
            free(archive->payloads_arrays[i]);
        }
        free(archive->payloads_arrays);
    }

    if (archive->files_headers)
    {
        for (int i = 0; i < archive->arc_header.files_count; i++)
        {
            free(archive->files_headers[i].filename);
        }
        free(archive->files_headers);
    }
}

int targeted_index(ArchiveContext *archive, char *filename)
{
    int count = archive->arc_header.files_count;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(archive->files_headers[i].filename, filename) == 0)
        {
            return i;
        }
    }

    return -1;
}

int delete_file(char *archive_filename, ArchiveContext *archive, char *filename)
{
    int target_index = targeted_index(archive, filename);
    int count = archive->arc_header.files_count;

    if (target_index == -1)
    {
        return 1;
    }

    free(archive->files_headers[target_index].filename);
    free(archive->payloads_arrays[target_index]);

    int elements_to_move = count - target_index - 1;
    if (elements_to_move > 0)
    {
        memmove(&archive->files_headers[target_index],
                &archive->files_headers[target_index + 1],
                elements_to_move * sizeof(FilesHeader));

        memmove(&archive->payloads_arrays[target_index],
                &archive->payloads_arrays[target_index + 1],
                elements_to_move * sizeof(unsigned char *));
    }

    archive->arc_header.files_count--;

    return 0;
}

int extract_file(ArchiveContext *archive, char *filename)
{
    int target_index = -1;
    int count = archive->arc_header.files_count;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(archive->files_headers[i].filename, filename) == 0)
        {
            target_index = i;
            break;
        }
    }

    if (target_index == -1)
    {
        return 1;
    }

    FILE *fh = fopen(filename, "wb");
    if (fh == NULL)
        return 1;
    fwrite(archive->payloads_arrays[target_index], archive->files_headers[target_index].file_size, 1, fh);
    fclose(fh);

    return 0;
}

int main(int argc, char *argv[])
{
    ArchiveContext archive;
    char *archive_filename = argv[2];

    if (strcmp(argv[1], "-c") == 0)
    {

        initiate_archive_header(&(archive.arc_header));

        FILE *fh = fopen(archive_filename, "wb");
        if (!fh)
            return 1;

        fwrite(&(archive.arc_header), sizeof(ArchiveHeader), 1, fh);
        fclose(fh);

        archive.files_headers = NULL;
        archive.payloads_arrays = NULL;
    }

    else if (strcmp(argv[1], "-i") == 0)
    {
        load_archive(archive_filename, &archive);
        for (int i = 3; i < argc; i++)
        {
            char *filename = argv[i];

            int file_size = get_file_size(filename);

            FilesHeader file_header = {
                .file_size = file_size,
                .filename_size = strlen(filename),
                .filename = filename};

            unsigned char *ptr = malloc(file_size);

            FILE *fh = fopen(filename, "rb");
            if (!fh)
                return 1;
            fread(ptr, file_size, 1, fh);
            fclose(fh);

            insert_file(&archive, &file_header, ptr);
        }
        save_archive(archive_filename, &archive);

        free_pointers(&archive);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        load_archive(archive_filename, &archive);
        for (int i = 3; i < argc; i++)
        {
            char *filename = argv[i];
            delete_file(archive_filename, &archive, filename);
        }
        save_archive(archive_filename, &archive);
        free_pointers(&archive);
    }

    else if (strcmp(argv[1], "-l") == 0)
    {
        load_archive(archive_filename, &archive);
        print_archive_data(&archive);
    }

    else if (strcmp(argv[1], "-x") == 0)
    {
        char *filename = argv[3];
        load_archive(archive_filename, &archive);
        extract_file(&archive, filename);

        free_pointers(&archive);
    }

    else if (strcmp(argv[1], "-n") == 0)
    {
        load_archive(archive_filename, &archive);
        printf("The number of files stored in your Archive is/are: %d\n", archive.arc_header.files_count);
    }

    else if (strcmp(argv[1], "-r") == 0)
    {
        char *FileToBeReplacedName = argv[3];
        char *FileToBeInsertedName = argv[4];

        load_archive(archive_filename, &archive);

        int target_index = targeted_index(&archive, FileToBeReplacedName);
        if (target_index == -1)
            return 1;
        int file_size = get_file_size(FileToBeInsertedName);

        FilesHeader file_header = {
            .file_size = file_size,
            .filename_size = strlen(FileToBeInsertedName),
            .filename = strdup(FileToBeInsertedName)};

        unsigned char *ptr = malloc(file_size);

        FILE *fh = fopen(FileToBeInsertedName, "rb");
        if (!fh)
            return 1;
        fread(ptr, file_size, 1, fh);
        fclose(fh);

        free(archive.files_headers[target_index].filename);
        free(archive.payloads_arrays[target_index]);

        archive.files_headers[target_index] = file_header;
        archive.payloads_arrays[target_index] = ptr;

        save_archive(archive_filename, &archive);
        free_pointers(&archive);
    }

    else if (strcmp(argv[1], "-h") == 0)
    {
#define CLR_RESET "\x1b[0m"
#define CLR_HEADER "\x1b[1;36m"
#define CLR_FLAG "\x1b[1;32m"
#define CLR_SYNTAX "\x1b[33m"
#define CLR_FOOTER "\x1b[1;30m"

        printf(CLR_HEADER "Archiver Utility - Help Menu\n" CLR_RESET);
        printf(CLR_HEADER "====================================================================\n" CLR_RESET);
        printf("Usage: %s <option> <archive_name> [arguments...]\n\n", argv[0]);

        printf("Options:\n");
        printf("  " CLR_FLAG "-c" CLR_RESET "    Create a new empty archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -c <archive_name>\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-i" CLR_RESET "    Insert one or more files into the archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -i <archive_name> <file1> [file2 ...]\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-d" CLR_RESET "    Delete one or more files from the archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -d <archive_name> <file1> [file2 ...]\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-l" CLR_RESET "    List metadata for all files in the archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -l <archive_name>\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-x" CLR_RESET "    Extract a specific file from the archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -x <archive_name> <file_to_extract>\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-n" CLR_RESET "    Print the total number of files stored in the archive.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -n <archive_name>\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-r" CLR_RESET "    Replace an existing file in the archive with a new local file.\n");
        printf("%8sSyntax: " CLR_SYNTAX "%s -r <archive_name> <old_file> <new_file>\n\n" CLR_RESET, "", argv[0]);

        printf("  " CLR_FLAG "-h" CLR_RESET "    Show this help menu.\n");
        printf(CLR_HEADER "====================================================================\n" CLR_RESET);

        printf(CLR_FOOTER "(c) 2026 Team x86. All rights reserved.\n" CLR_RESET);

#undef CLR_RESET
#undef CLR_HEADER
#undef CLR_FLAG
#undef CLR_SYNTAX
#undef CLR_FOOTER
    }
    return 0;
}