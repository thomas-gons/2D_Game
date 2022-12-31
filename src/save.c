#include "save.h"


bool parse_file_ext(const struct dirent *read_file, const char *ext) {
    if (!read_file) {
       return false;
    }
    // Get only regular files that are not read_fileectories
    if (read_file->d_type == DT_REG && read_file->d_type != DT_DIR) {
        // Extract the file extension to compare
        const char *f_ext = strrchr(read_file->d_name, '.');
        if ((!f_ext) || (f_ext == read_file->d_name)) {
            return false;
        } else {
            if (strcmp(f_ext, ext) == 0) {
                return true;
            }
        }
    }
    return false;
}

uint8_t get_nb_files(const char *dir_path, const char *ext) {
    uint8_t nb_files = 0;
    // Read the entire directory and parse files based on the extension
    struct dirent *read_file;
    DIR *dir = opendir(dir_path);
    if (dir) {
        while ((read_file = readdir(dir)) != NULL) {
            if (parse_file_ext(read_file, ext)) {
                nb_files++;
            }
        }
        closedir(dir);
    }
    return nb_files;
}

void get_files(const char *dir_path, const char *ext, char **arr_files) {
    uint8_t i = 0;
    uint8_t name_len = 0;
    // Read the entire directory and parse files based on the extension
    struct dirent *read_file;
    DIR *dir = opendir(dir_path);
    if (dir) {
        while ((read_file = readdir(dir)) != NULL) {
            if (parse_file_ext(read_file, ext)) {
                // Get lenght of file name
                name_len = 1 + strlen(read_file->d_name);
                // Copy string in allocated array
                arr_files[i] = (char *) calloc(name_len, sizeof (char));
                strncpy(arr_files[i], read_file->d_name, name_len);
                i++;
            }
        }
        closedir(dir);
        // Add "Return to Title Menu" string to end of array
        arr_files[i] = (char *) calloc(21, sizeof (char));
        strncpy(arr_files[i], "Return to Title Menu", 21);
    }
}

void str_2d_array_free(char **arr, uint8_t arr_len) {
    for (uint8_t i = 0; i < arr_len; i++) {
        free(arr[i]);
    }
    free(arr);
}