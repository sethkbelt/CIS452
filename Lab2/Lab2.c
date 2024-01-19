/****************************************************************************
 * Title: Lab 2
 * Author: Seth Konynenbelt
 * Created: June 20, 2023
 * Description: This lab exists to refresh C programming skills in Unix
 * system calls, library functions, pointers, and structures. The C program
 * displays information about the current user and the current machine 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
// #include <dirent.h>
// #include <string.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>

/* Structure Definitions*/
typedef struct
{
    char file_name[256];
    int file_size;
} FileLarge;

typedef struct
{
    char file_name[256];
    int file_access;
} FileAccess;

/* Macro Definitions*/
#define NUM_FILES 5

int main()
{    
    // pointer to current user with system calls
    char *me;
    me = getenv("USER");
    printf("\nAbout me\n");
    printf("--------------------------------------------------\n");

    struct passwd pwd;
    struct passwd *result;
    char *buf;
    long bufsize;
    int s;

        //    if (argc != 2) {
        //        fprintf(stderr, "Usage: %s username\n", argv[0]);
        //        exit(EXIT_FAILURE);
        //    }

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)          /* Value was indeterminate */
        bufsize = 16384;        /* Should be more than enough */
           
    buf = malloc(bufsize);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    s = getpwnam_r(me, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        if (s == 0)
            printf("Not found\n");
        else {
            errno = s;
            perror("getpwnam_r");
        }
        exit(EXIT_FAILURE);
    }

struct group *grp;
grp = getgrgid(pwd.pw_gid);


    printf("Unix User               : %s (%u)\n", pwd.pw_name, pwd.pw_uid);
    printf("Name                    : %s\n", pwd.pw_gecos);
    printf("Unix Group              : %s (%u)\n", grp->gr_name, grp->gr_gid);
    printf("Unix Home Directory     : %s\n", pwd.pw_dir);
    printf("Login Shell             : %s\n", pwd.pw_shell);


    struct stat sb;
    if (lstat(pwd.pw_dir, &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }


    char chmod_buf[] = {'-','-','-','-','-','-','-','-','-'};
    if(S_ISDIR(sb.st_mode) == 1)
        chmod_buf[0] = 'd';
    for (int i = 0; i < sizeof(chmod_buf); i = i + 3)
    {
        if(sb.st_mode & 1 << i)
            chmod_buf[9 - i] = 'x';
        if(sb.st_mode & 1 << (i+1))
            chmod_buf[8 - i] = 'w';
        if(sb.st_mode & 1 << (i+2))
            chmod_buf[7 - i] = 'r';
    }
    printf("Home Permission         : %s (%o) \n", chmod_buf, sb.st_mode);
    struct utsname unameSys;
    uname(&unameSys);

    printf("\nAbout my machine\n");
    printf("--------------------------------------------------\n");

    printf("Host                    : %s\n", unameSys.nodename);
    printf("System                  : %s %s\n\n", unameSys.sysname ,unameSys.release);
    free(buf);   
 
 //Use your date of birth as a reference and print your age at the current time in the following format   
// hardcoded birthday of March 20, 2001
    struct tm birthday;
    char time_buffer[80];

    birthday.tm_year = 101; // tm_year, The number of years since 1900.
    birthday.tm_mon = 2;
    birthday.tm_mday = 20; 
    
    strftime(time_buffer, sizeof(time_buffer), "%D", &birthday);
    printf("Time buffer is %s", time_buffer);
    
    strftime(time_buffer, sizeof(time_buffer), "%s", &birthday);
    printf("Time buffer is %s", time_buffer);


    int x = time(NULL);
    printf(" \n%d", x); 

    int buffer_int = atoi(time_buffer);
    //int year_old = (x - buffer_int) / 60/60/24/365.24;
    // int month_old = 60/60/24
    // int day_old =  60/60;

    int year_old = difftime(x, buffer_int) / 60/60/24/365.24;
    double year_old_double = difftime(x, buffer_int) / 60/60/24/365.24;
    int month_old = 
    int day_old = difftime(x, buffer_int) / 60 / 60 / 24 / 7;

//   struct tm tm = *gmtime(&seconds);
//   printf("%4d-%02d-%02d %02d:%02d:%02d.%03lu\n",
//       tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
//       tm.tm_hour, tm.tm_min, tm.tm_sec,
//       fraction/10000);

    printf(" \n%d", year_old); 
    printf(" \n%d", month_old); 
    printf(" \n%d", day_old); 

    exit(EXIT_SUCCESS);
}

