/*
 * packer.c
 * Archive a directory into a very simple archive format.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// 128MB maximum filesize
#define MAX_FILESIZE (1024*1024*128)

// Current filesize
int archive_size = 0;

/*
 * Archive format for each entry:
 * - 256 bytes containing filename
 * - 4 bytes containing filesize N
 * - N bytes containing file entry
 */

int archive_directory(FILE *file, const char *path, const char *relative)
{
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    char fullpath[256] = "";
    char relpath[256] = "";
    FILE *ent_file;
    int length;
    int result = 0;
    char *blob;

    dir = opendir(path);
    if (dir) {
        while ((ent = readdir(dir))) {
            if (ent->d_name[0] == '.')
                continue;

            strcpy(fullpath, path);
            strcat(fullpath, "/");
            strcat(fullpath, ent->d_name);

            strcpy(relpath, "");
            if (strlen(relative) > 0) {
                strcpy(relpath, relative);
                strcat(relpath, "/");
            }
            strcat(relpath, ent->d_name);
            printf("%s", relpath);

            stat(fullpath, &st);

            if (st.st_mode & S_IFREG) {
                /* Write this file to the archive */
                ent_file = fopen(fullpath, "rb");
                fseek(ent_file, 0, SEEK_END);
                length = ftell(ent_file);
                fseek(ent_file, 0, SEEK_SET);

                printf(" (%d bytes)\n", length);

                archive_size += length;
                if (archive_size > MAX_FILESIZE) {
                    printf("Archive exceeds maximum size, aborting\n");
                    result = 1;
                    goto done;
                }

                blob = malloc(length);
                fread(blob, 1, length, ent_file);
                fwrite(relpath, 1, 256, file);
                fwrite(&length, 4, 1, file);
                fwrite(blob, 1, length, file);
                free(blob);
                fclose(ent_file);
            } else if (st.st_mode & S_IFDIR) {
                printf("\n");

                /* Recurse into this directory */
                if (archive_directory(file, fullpath, relpath) != 0) {
                    result = 1;
                    goto done;
                }
            }
        }
done:
        closedir(dir);
        return result;
    } else {
        printf("FAILED!\n");
        return 1;
    }
}

int main(int argc, char *argv[])
{
    const char *dir, *out;
    FILE *file;

    if (argc < 3) {
        printf("Usage: packer [dir] [out]\n");
        exit(1);
    }

    dir = argv[1];
    out = argv[2];

    file = fopen(out, "wb");
    if (!file) {
        printf("Error opening '%s', aborting\n", out);
        exit(1);
    }

    archive_directory(file, dir, "");
    fclose(file);

    return 0;
}