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

/* Function Prototypes */
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

/************************* getUnixInformation() ****************************
 *  brief : Get and print an assortment of current Unix user and group 
 *          information
 *  param : N/A
 *  return: N/A
 ***************************************************************************/
void getUnixInformation()
{
    // point to current user with system calls
    struct passwd pwd;
    char *me;
    me = getenv("USER");
    printf("\nAbout me (%s)\n", me);
    printf("--------------------------------------------------\n");

    // taken directly from manpage passwd example
    struct passwd *result;
    char *buf;
    long bufsize;
    int s;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)   /* Value was indeterminate */
        bufsize = 16384; /* Should be more than enough */

    // indicate a malloc failure
    buf = malloc(bufsize);
    if (buf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // gather necessary Unix information to store in struct s
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

    // use the group ID from pwnam to get group information
    struct group *grp;
    grp = getgrgid(pwd.pw_gid);

    printf("Unix User               : %s (%u)\n", pwd.pw_name, pwd.pw_uid);
    printf("Name                    : %s\n", pwd.pw_gecos);
    printf("Unix Group              : %s (%u)\n", grp->gr_name, grp->gr_gid);
    printf("Unix Home Directory     : %s\n", pwd.pw_dir);
    printf("Login Shell             : %s\n", pwd.pw_shell);
    
    // use lstat from the pwnam to get permissions information
    struct stat sb;
    if (lstat(pwd.pw_dir, &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    // custom chmod octal to display converter
    const int chmod_size = 10; // sufficient to dislay characters
    char chmod_buf[100] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'}; // large enough to not blow stack
    // deciding if input is a directory
    if (S_ISDIR(sb.st_mode) == 1)
        chmod_buf[0] = 'd';
    
    // looping through octal representation by 3 to determine permissions
    for (int i = 0; i < 10; i = i + 3)
    {
        if (sb.st_mode & 1 << i)
            chmod_buf[chmod_size - 1 - i] = 'x';
        if (sb.st_mode & 1 << (i + 1))
            chmod_buf[chmod_size - 2 - i] = 'w';
        if (sb.st_mode & 1 << (i + 2))
            chmod_buf[chmod_size - 3 - i] = 'r';
    }
    printf("Home Permission         : %s (%o) \n", chmod_buf, sb.st_mode);

   // free buf pointer
   free(buf);
}

/************************* getSystemInformation() **************************
 *  brief : Get and print an assortment of system information
 *  param : N/A
 *  return: N/A
 ***************************************************************************/
void getSystemInformation()
{
    // use utsname struct to get and store system information
    struct utsname unameSys;
    uname(&unameSys);

    printf("\nAbout my machine\n");
    printf("--------------------------------------------------\n");

    printf("Host                    : %s\n", unameSys.nodename);
    printf("System                  : %s %s\n\n", unameSys.sysname, unameSys.release);
}

/************************* getBirthdayInformation()**************************
 *  brief : Uses your hard coded date of birth as a reference to print
 *          your agek at the current time
 *  param : N/A
 *  return: N/A
 ***************************************************************************/
void getBirthdayInformation()
{
    // hardcoding March 20, 2001 birthday
    struct tm birthday;
    birthday.tm_year = 101; // tm_year, The number of years since 1900.
    birthday.tm_mon = 2; //0-11 format
    birthday.tm_mday = 20;

    // capturing the current time
    time_t nowEpoch = time(NULL);
    struct tm *nowStruct = gmtime(&nowEpoch);
    time_t t1 = mktime(nowStruct);
    time_t t2 = mktime(&birthday);
    time_t dt = difftime(t1, t2);

    // math necessary to convert seconds into days, months, years
    int days = dt / (60 * 60 * 24) - 1;
    int years = days / 365.25;
    int months = (days % 365) / 31; // Assuming a month is 31 days (avg is 30.437)
    days = days - (years * 365.25 + months * 30.437);

    printf("Hardcoded that you were born on March 20, 2001. \n");
    printf("You are %d years, %d months, and %d days old.\n", years, months, days);
}
