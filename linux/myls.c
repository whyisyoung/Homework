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
 * -t - sort by modification time
 * -p - sort by type
 * -a - do not hide entries starting with .
 * -R - list subdirectories recursively
 * -l - use a long listing format
*/

#define SORT_BY_MTIME (1<<0) /* -t */
#define SORT_BY_TYPE  (1<<1) /* -p */
#define ALL           (1<<2) /* -a */
#define RECURSIVE     (1<<3) /* -R */
#define DETAIL        (1<<4) /* -l */
#define MAX_FILE_COUNT   1024
#define MAX_FILENAME_LEN 80

struct FileList {
        char name[MAX_FILENAME_LEN];
        struct stat info;
} file_list[MAX_FILE_COUNT];

static const char *optString = "tpaRl";

int  get_file_list(char dirname[], struct FileList *file_list, int mode);
void display_files_of(struct FileList *file_list, int count, int mode);
void display_file_simply(struct FileList *file_list, int count);
void display_file_detail(struct FileList *file_list, int count);
int  name_cmp(const void *a, const void *b);
int  mtime_cmp(const void *a, const void *b);
int  type_cmp(const void *a, const void *b);

void file_mode_to_string(int mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);

int main(int argc, char *argv[])
{
        int i;
        int mode = 0;       /* default no parameters */
        int opt = 0;        /* get command options */
        int file_count = 0; /* get the file count of a specefic directory. */

        while((opt = getopt(argc, argv, optString)) != -1) {
                switch(opt) {
                case 't': mode |= SORT_BY_MTIME; break;
                case 'p': mode |= SORT_BY_TYPE;  break;
                case 'a': mode |= ALL;           break;
                case 'R': mode |= RECURSIVE;     break;
                case 'l': mode |= DETAIL;        break;
                default:
                        exit(EXIT_FAILURE);
                }
        }

        if(optind == argc) { /* current directory */
                file_count = get_file_list(".", &file_list, mode);
                display_files_of(&file_list, file_count, mode);
        }
        else {              /* specify one or more directories */
                for(i = optind; i < argc; ++i) {
                        if( optind + 1 != argc) /* more than one dir */
                                printf("%s: \n", argv[i]);
                        file_count = get_file_list(argv[i], &file_list, mode);
                        display_files_of(&file_list, file_count, mode);
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
        int count = 0, i;
        char filename[MAX_FILENAME_LEN];

        if((dir_pointer = opendir(dirname)) == NULL) {
                fprintf(stderr, "ls: can not open %s\n", dirname);
                exit(EXIT_FAILURE);
        }
        else {
                chdir(dirname); /* change working directory, or stat will not work well. */

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

                closedir(dir_pointer);
        }
        return count;
}

int name_cmp(const void *a, const void *b)
/*
 *      for qsort with filename.
 *      FIXME: case-sensitive(it's a little bug).
 */
{
        struct FileList *c = (struct FileList *) a;
        struct FileList *d = (struct FileList *) b;

        return strcmp(c->name, d->name);
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

void display_files_of(struct FileList *file_list, int count, int mode)
{
        if(mode & DETAIL)
                display_file_detail(file_list, count);
        else
                display_file_simply(file_list, count);
}


void display_file_simply(struct FileList *file_list, int count)
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

        /* get terminal size */
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        cols = size.ws_col;
        cols /= (max_name_length + 1);

        for(i = 0; i < count; ++i) {
                if(i != 0 && (i % cols == 0))
                        puts(" ");
                printf("%*s", -(max_name_length + 1), file_list[i].name);
        }
        printf("\n");
}


void display_file_detail(struct FileList *file_list, int count)
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
                printf("%s\n",    file_list[i].name);
        }
}

/* utility functions */
void file_mode_to_string(int mode, char str[])
{
        strcpy(str, "----------");

        if(S_ISDIR(mode))  str[0] = 'd';
        if(S_ISCHR(mode))  str[0] = 'c';
        if(S_ISBLK(mode))  str[0] = 'b';

        if(mode & S_IRUSR) str[1] = 'r';
        if(mode & S_IWUSR) str[2] = 'r';
        if(mode & S_IXUSR) str[3] = 'r';

        if(mode & S_IRGRP) str[4] = 'r';
        if(mode & S_IWGRP) str[5] = 'w';
        if(mode & S_IXGRP) str[6] = 'x';

        if(mode & S_IROTH) str[7] = 'r';
        if(mode & S_IWOTH) str[8] = 'w';
        if(mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid)
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
