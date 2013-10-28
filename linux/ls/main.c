#include "myls.h"

/*
 * Simple implementation of linux command ls.
 *
 * myls supports the following command-line arguments:
 *   -t - sort by modification time
 *   -r - reverse output
 *   -a - do not hide entries starting with "."
 *   -R - list subdirectories recursively
 *   -l - use a long listing format
 * support mix of these options like -traRl
 * also support colors to distinguish dir and file :D
 *
 * FIXME: when comes to a filename very long(maybe >=80)
 *        it will get a floating point exception :(
*/

static const char *optString = "traRl";

int main(int argc, char *argv[])
/*
 * use getopt() to access command option.
 * you can use multiple options once like:
 * myls -atlrR
 * so convenient, just like real ls :)
 */
{
        int i;
        int mode = 0;       /* default no parameters */
        int opt = 0;        /* get command options */
        int file_count = 0; /* get the file count of a specefic directory. */

        while((opt = getopt(argc, argv, optString)) != -1) {
                switch(opt) {
                case 't': mode |= SORT_BY_MTIME; break;
                case 'r': mode |= REVERSE;       break;
                case 'a': mode |= ALL;           break;
                case 'R': mode |= RECURSIVE;     break;
                case 'l': mode |= DETAIL;        break;
                default:
                        exit(EXIT_FAILURE);
                }
        }

        if(optind == argc) { /* current directory */
                file_count = get_file_list(".", (struct FileList *)&file_list, mode);
                display((struct FileList *)&file_list, file_count, mode);
        }
        else {              /* specify one or more directories */
                for(i = optind; i < argc; ++i) {
                        if( optind + 1 != argc) /* more than one dir */
                                printf("%s: \n", argv[i]);
                        file_count = get_file_list(argv[i],
                                                   (struct FileList *)&file_list,
                                                   mode);
                        display((struct FileList *)&file_list, file_count, mode);
                }
        }

        return EXIT_SUCCESS;
}

int get_file_list(char dirname[], struct FileList *file_list, int mode)
/*
 * Store all the files under the dir in file_list,
 * sort them alphabetically default.
 * and return the count of files.
*/
{
        DIR           *dir_pointer;  /* the directory*/
        struct dirent *dirent_ptr;   /* each entry */
        int count = 0;
        char filename[MAX_FILENAME_LEN];

        if((dir_pointer = opendir(dirname)) == NULL) {
                fprintf(stderr, "ls: can not open %s\n", dirname);
                exit(EXIT_FAILURE);
        }
        else {
                /*
                 * change working directory, as stat() accesss relative path
                 * more reason can be found here: http://goo.gl/gO1zKd
                 */
                chdir(dirname);

                /* collect file name and info */
                while((dirent_ptr = readdir(dir_pointer)) != NULL) {
                        strcpy(filename, dirent_ptr->d_name);

                        if(filename[0] == '.' && !(mode & ALL))
                                continue;
                        strcpy(file_list[count].name, filename);

                        /*stats the file and fills in info. */
                        if(stat(filename, &file_list[count].info) == -1)
                                perror(filename);

                        ++count;
                }

                qsort(file_list, count, sizeof(file_list[0]), name_cmp);

                if(mode & SORT_BY_MTIME)
                        qsort(file_list, count, sizeof(file_list[0]), mtime_cmp);

                if(mode & REVERSE)
                        reverse_file_list(file_list, count);

                closedir(dir_pointer);
        }
        return count;
}

void display(struct FileList *file_list, int count, int mode)
/*
 * show file with specified mode(command option).
 */
{
        if(mode & RECURSIVE)
                display_file_recursively(file_list, count, mode);
        else if(mode & DETAIL)
                display_file_detail(file_list, count);
        else
                display_file_simply(file_list, count);
}
