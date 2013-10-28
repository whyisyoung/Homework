#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Simple implementation of linux command ls.
 *
 * myls supports the following command-line arguments:
 *   -t - sort by modification time
 *   -r - reverse output
 *   -a - do not hide entries starting with "."
 *   -R - list subdirectories recursively
 *   -l - use a long listing format
 * also support colors to distinguish dir and file :D
*/

#define SORT_BY_MTIME (1<<0) /* -t */
#define REVERSE       (1<<1) /* -r */
#define ALL           (1<<2) /* -a */
#define RECURSIVE     (1<<3) /* -R */
#define DETAIL        (1<<4) /* -l */
#define MAX_FILE_COUNT   1024
#define MAX_FILENAME_LEN 200
#define MAX_PATH_LEN     500

struct FileList {
        char name[MAX_FILENAME_LEN]; /* filename, or subdirectory name */
        struct stat info;            /* file info                      */
} file_list[MAX_FILE_COUNT];

static const char *optString = "traRl";

int  get_file_list(char dirname[], struct FileList *file_list, int mode);
void reverse_file_list(struct FileList *file_list, int count);

/* print functions */
void display(struct FileList *file_list, int count, int mode);
void display_file_simply(struct FileList *file_list, int count);
void display_file_detail(struct FileList *file_list, int count);
void display_file_recursively(struct FileList *file_list, int count, int mode);

/* compare functions used by qsort() */
int  name_cmp(const void *a, const void *b);
void lower_case(const char *filename, char *new_name);
int  mtime_cmp(const void *a, const void *b);

/* fileinfo help functions */
void file_mode_to_string(int mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);


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
                file_count = get_file_list(".", &file_list, mode);
                display(&file_list, file_count, mode);
        }
        else {              /* specify one or more directories */
                for(i = optind; i < argc; ++i) {
                        if( optind + 1 != argc) /* more than one dir */
                                printf("%s: \n", argv[i]);
                        file_count = get_file_list(argv[i], &file_list, mode);
                        display(&file_list, file_count, mode);
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

int name_cmp(const void *a, const void *b)
/*
 *      for qsort with filename.
 *      just like ls -- case insensitive
 */
{
        struct FileList *c = (struct FileList *) a;
        struct FileList *d = (struct FileList *) b;
        char cnametmp[MAX_FILENAME_LEN], dnametmp[MAX_FILENAME_LEN];

        lower_case(c->name, cnametmp);
        lower_case(d->name, dnametmp);

        return strcmp(cnametmp, dnametmp);
}

void lower_case(const char *filename, char *new_name)
/*
 * change filename to lower case for better sort.
 */
{
        int len = strlen(filename);
        int i;
        for(i = 0; i < len; ++i)
                new_name[i] = tolower(filename[i]);
        new_name[len] = '\0';
}

int mtime_cmp(const void *a, const void *b)
/*
 *      for qsort with modification time. default the newest first.
 */
{
        struct FileList *c = (struct FileList *) a;
        struct FileList *d = (struct FileList *) b;

        return d->info.st_mtime - c->info.st_mtime; /*compare time is so easy!*/
}

void reverse_file_list(struct FileList *file_list, int count)
{
        int i;
        char nametmp[MAX_FILENAME_LEN];
        struct stat infotmp;

        for(i = 0; i < count / 2; ++i) {
                strcpy(nametmp, file_list[i].name);
                strcpy(file_list[i].name, file_list[count - i - 1].name);
                strcpy(file_list[count - i -1].name, nametmp);
                infotmp = file_list[i].info;
                file_list[i].info = file_list[count - i - 1].info;
                file_list[count -i -1].info = infotmp;
        }
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

void display_file_recursively(struct FileList *file_list, int count, int mode)
/*
 * display for -R option.
 * NOTE: for multiple directories, please use absolute path.
 */
{
        char path[MAX_PATH_LEN] = { 0 };
        char temp[MAX_PATH_LEN] = { 0 };
        char filename[MAX_FILENAME_LEN] = { 0 };
        struct FileList list[MAX_FILE_COUNT];
        int i = 0, size = 0;

        puts(get_current_dir_name()); /* absolute path */
        strcpy(path, get_current_dir_name());

        if(mode & DETAIL)
                display_file_detail(file_list, count);
        else
                display_file_simply(file_list, count);
        printf("\n");

        for(i = 0; i < count; ++i) {
                strcpy(filename, file_list[i].name);

                /* NOTE: "." and ".." is directory, skip them! */
                if((strcmp(filename, ".") == 0) || (strcmp(filename, "..") == 0))
                        continue;
                if(S_ISDIR(file_list[i].info.st_mode)) { /*if a directory*/
                        strcpy(temp, path);
                        strcat(path, "/");
                        strcat(path, filename); /*store absolute path*/
                        size = get_file_list(path, list, mode);
                        display_file_recursively(list, size, mode);
                        strcpy(path, temp);
                }
        }
}

void display_file_detail(struct FileList *file_list, int count)
/*
 * display for -l option.
 */
{
        char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
        char modestr[11];
        struct stat *info_p;
        int i;

        for(i = 0; i < count; ++i) {
                info_p = &file_list[i].info;

                file_mode_to_string(info_p->st_mode, modestr);

                printf("%s",      modestr);
                printf("%4d ",    (int)info_p->st_nlink);
                printf("%-8s ",   uid_to_name(info_p->st_uid));
                printf("%-8s ",   gid_to_name(info_p->st_gid));
                printf("%8ld ",   (long)info_p->st_size);
                printf("%.12s ",  4 + ctime(&info_p->st_mtime));

                if(S_ISDIR(file_list[i].info.st_mode)) /* dir   show in blue  */
                        printf("\e[34m%s", file_list[i].name);
                else                                   /* other show in green */
                        printf("\e[92m%s", file_list[i].name);
                printf("\e[39m\n"); /* default white */
        }
}

void display_file_simply(struct FileList *file_list, int count)
/*
 * display without -R or -l option.
 */
{
        struct winsize size;
        int max_name_length = 0, i;
        int len = 0;
        int cols ; /*columns splited */

        for(i = 0; i < count; ++i) {
                len = strlen(file_list[i].name);
                if( len > max_name_length)
                        max_name_length = len;
        }

        /* get terminal size with ioctl (2) */
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        cols = size.ws_col;
        cols /= (max_name_length + 1);

        for(i = 0; i < count; ++i) {
                if(i != 0 && (i % cols == 0))
                        puts(" ");

                /*
                 * use "%*s": the precision is not specified in the format
                 * string, but as an additional integer value argument
                 * preceding the argument that has to be formatted.
                 */
                if(S_ISDIR(file_list[i].info.st_mode))
                        printf("\e[34m%*s", -(max_name_length + 1),
                                file_list[i].name);
                else
                        printf("\e[92m%*s", -(max_name_length + 1),
                                file_list[i].name);
        }
        printf("\e[39m\n");
}


void file_mode_to_string(int mode, char str[])
/*
 * NOTE: It does not code setuid, setgid, and sticky codes.
 */
{
        strcpy(str, "----------");          /* default=no perms */

        if(S_ISDIR(mode))  str[0] = 'd';    /* directory?       */
        if(S_ISCHR(mode))  str[0] = 'c';    /* char devices     */
        if(S_ISBLK(mode))  str[0] = 'b';    /* block device     */

        if(mode & S_IRUSR) str[1] = 'r';    /* 3 bits for user  */
        if(mode & S_IWUSR) str[2] = 'r';
        if(mode & S_IXUSR) str[3] = 'r';

        if(mode & S_IRGRP) str[4] = 'r';    /* 3 bits for group */
        if(mode & S_IWGRP) str[5] = 'w';
        if(mode & S_IXGRP) str[6] = 'x';

        if(mode & S_IROTH) str[7] = 'r';    /* 3 bits for other */
        if(mode & S_IWOTH) str[8] = 'w';
        if(mode & S_IXOTH) str[9] = 'x';
}


char *uid_to_name(uid_t uid)
/*
 *      returns pointer to username associated with uid, uses getpwuid()
 */
{
        struct passwd *getpwuid(), *pw_ptr;
        static char numstr[10];

        if((pw_ptr = getpwuid(uid)) == NULL) {
                sprintf(numstr, "%d", uid);
                return numstr;
        }
        else
                return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
/*
 *      returns pointer to group number gid. used getgrgid(3)
 */
{
        struct group *getgrgid(), *grp_ptr;
        static char numstr[10];

        if((grp_ptr = getgrgid(gid)) == NULL) {
                sprintf(numstr, "%d", gid);
                return numstr;
        }
        else
                return grp_ptr->gr_name;
}
