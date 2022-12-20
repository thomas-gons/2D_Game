#include "save.h"


bool parse_file_ext(const struct dirent *dir, const char *ext) {
    if (!dir) {
       return false;
    }
    if (dir->d_type == DT_REG && dir->d_type != DT_DIR) {
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
        while ((dp = readdir(dir)) != NULL) {
            if (parse_file_ext(dp, ext)) {
                printw("%s\n", dp->d_name);
                refresh();
            }
        }
        closedir(dir);
    }
}