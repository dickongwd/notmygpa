#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <pwd.h>
#include <sys/sysmacros.h>                          /* To get major() and minor() */
#include <time.h>

/*
Information needed 
User and Group names of user group owner
Type of file 
Full access permissions 
Size of File 
I-Node Number 
Device Number of device in which the file is stored 
Number of links 
Last Access
Modification 
File status Changes 
*/


/*
           struct stat {
               dev_t     st_dev;         /* ID of device containing file 
               ino_t     st_ino;         /* Inode number 
               mode_t    st_mode;        /* File type and mode 
               nlink_t   st_nlink;       /* Number of hard links 
               uid_t     st_uid;         /* User ID of owner 
               gid_t     st_gid;         /* Group ID of owner 
               dev_t     st_rdev;        /* Device ID (if special file) 
               off_t     st_size;        /* Total size, in bytes 
               blksize_t st_blksize;     /* Block size for filesystem I/O 
               blkcnt_t  st_blocks;      /* Number of 512B blocks allocated 

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. 

               struct timespec st_atim;  /* Time of last access 
               struct timespec st_mtim;  /* Time of last modification 
               struct timespec st_ctim;  /* Time of last status change 

           #define st_atime st_atim.tv_sec       Backward compatibility 
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };

*/

int main(int argc, char const *argv[])
{
	struct stat file_info;
	struct passwd *pwd;


	if (stat(argv[1], &file_info) == 0)
	{	
		int file_permission;

		//User Group names
		if ((pwd = getpwuid(file_info.st_uid)) != NULL){
			printf("User ID: %8s\n", pwd->pw_name);
		}
		if ((pwd = getpwuid(file_info.st_gid)) != NULL){
			printf("Group ID: %8s\n", pwd->pw_name);
		}

		//File Type 
		printf("File Type: ");
		switch (file_info.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
        }
		
		//Full access permissions 
		printf("File permissions: ");
        file_permission = file_info.st_mode;
        for (int i = 0; i < 3; i++){
        	if (file_permission & S_IRUSR)
        		printf("r");
        	else
        		printf("-");
        	if (file_permission & S_IWUSR)
        		printf("w");
        	else
        		printf("-");
        	if (file_permission & S_IXUSR)
        		printf("x");
        	else 
        		printf("-");
        	file_permission = file_permission << 1;
        }
        printf("\n");

	}

	//Size of file 
	printf("File size: %jd bytes\n", (intmax_t) file_info.st_size);

	//Inode Number 
	printf("I-node number: %ju\n", (uintmax_t) file_info.st_ino);

	//ID of containing device 
	printf("ID of containing device:  [%jx,%jx]\n", (uintmax_t) major(file_info.st_dev), (uintmax_t) minor(file_info.st_dev));

	//Number of links 
	printf("Link count: %ju\n", (uintmax_t) file_info.st_nlink);

	//File access/ Modification/ Status Change 
	printf("Last file access:         %s", ctime(&file_info.st_atime));
    printf("Last file modification:   %s", ctime(&file_info.st_mtime));
    printf("Last status change:       %s", ctime(&file_info.st_ctime));

	return 0;
}

