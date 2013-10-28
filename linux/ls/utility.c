#include "myls.h"

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


/* compare functions */
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


/* fileinfo help functions */
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
