#include "save.h"


bool parse_file_ext(const struct dirent *dir, const char *ext) {
    if (!dir) {
       return false;
    }
    // Get only regular files that are not directories
    if (dir->d_type == DT_REG && dir->d_type != DT_DIR) {
        // Extract the file extension to compare
        const char *f_ext = strrchr(dir->d_name, '.');
        if ((!f_ext) || (f_ext == dir->d_name)) {
            return false;
        } else {
            if (strcmp(f_ext, ext) == 0) {
                return true;
            }
        }
    }
    return false;
}

void get_files(const char *path, const char *ext) {
    struct dirent *dp;
    DIR *dir = opendir(path);
    if (dir) {
        // Read the entire directory and parse based on the extension
        while ((dp = readdir(dir)) != NULL) {
            if (parse_file_ext(dp, ext)) {
                printw("%s\n", dp->d_name);
                refresh();
            }
        }
        closedir(dir);
    }
}