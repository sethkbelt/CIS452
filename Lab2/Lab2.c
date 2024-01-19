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
#include <math.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

void getUnixInformation();
void getSystemInformation();
void getBirthdayInformation();

int main()
{

    getUnixInformation();
    getSystemInformation();
    getBirthdayInformation();
    exit(EXIT_SUCCESS);
}
// Use your date of birth as a reference and print your age at the current time in the following format
// hardcoded birthday of March 20, 2001
void getBirthdayInformation()
{
    struct tm birthday;
    birthday.tm_year = 101; // tm_year, The number of years since 1900.
    birthday.tm_mon = 2;
    birthday.tm_mday = 20;

    time_t nowEpoch = time(NULL);
    struct tm *nowStruct = gmtime(&nowEpoch);
    time_t t1 = mktime(nowStruct);
    time_t t2 = mktime(&birthday);
    time_t dt = difftime(t1, t2);

    int days = dt / (60 * 60 * 24) - 1;
    int years = days / 365.25;
    int months = (days % 365) / 31; // Assuming a month is 30 days
    days = days - (years * 365.25 + months * 30.437);

    printf("Hardcoded that you were born on March 20, 2001. \n");
    printf("You are %d years, %d months, and %d days old.\n", years, months, days);
}

void getUnixInformation()
{
    // pointer to current user with system calls
    struct passwd pwd;
    char *me;
    me = getenv("USER");
    printf("\nAbout me\n");
    printf("--------------------------------------------------\n");

    // taken directly from manpage passwd example
    struct passwd *result;
    char *buf;
    long bufsize;
    int s;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)   /* Value was indeterminate */
        bufsize = 16384; /* Should be more than enough */

    buf = malloc(bufsize);
    if (buf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    s = getpwnam_r(me, &pwd, buf, bufsize, &result);
    if (result == NULL)
    {
        if (s == 0)
            printf("Not found\n");
        else
        {
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
    if (lstat(pwd.pw_dir, &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    char chmod_buf[100] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
    if (S_ISDIR(sb.st_mode) == 1)
        chmod_buf[0] = 'd';
    for (int i = 0; i < 10; i = i + 3)
    {
        if (sb.st_mode & 1 << i)
            chmod_buf[9 - i] = 'x';
        if (sb.st_mode & 1 << (i + 1))
            chmod_buf[8 - i] = 'w';
        if (sb.st_mode & 1 << (i + 2))
            chmod_buf[7 - i] = 'r';
    }
    printf("Home Permission         : %s (%o) \n", chmod_buf, sb.st_mode);

   free(buf);
}

void getSystemInformation()
{
    struct utsname unameSys;
    uname(&unameSys);

    printf("\nAbout my machine\n");
    printf("--------------------------------------------------\n");

    printf("Host                    : %s\n", unameSys.nodename);
    printf("System                  : %s %s\n\n", unameSys.sysname, unameSys.release);
}