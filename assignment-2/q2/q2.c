#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <pwd.h>
#include <sys/sysmacros.h>       /* To get major() and minor() */
#include <time.h>
#include <dirent.h> //For reading directory 

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

void print_stat(struct stat * file_info,const char * filename){
	struct passwd *pwd;
	int file_permission;

	printf("--------------------\n");
	//Filename 
	printf("Filename: %s\n", filename);
	//User Group names
	if ((pwd = getpwuid(file_info->st_uid)) != NULL){
		printf("User ID: %8s\n", pwd->pw_name);
	}
	if ((pwd = getpwuid(file_info->st_gid)) != NULL){
		printf("Group ID: %8s\n", pwd->pw_name);
	}

		//File Type 
	printf("File Type: ");
	switch (file_info->st_mode & S_IFMT) {
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
	file_permission = file_info->st_mode;
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



	//Size of file 
	printf("File size: %jd bytes\n", (intmax_t) file_info->st_size);

	//Inode Number 
	printf("I-node number: %ju\n", (uintmax_t) file_info->st_ino);

	//ID of containing device 
	printf("ID of containing device:  [%jx,%jx]\n", (uintmax_t) major(file_info->st_dev), (uintmax_t) minor(file_info->st_dev));

	//Number of links 
	printf("Link count: %ju\n", (uintmax_t) file_info->st_nlink);

	//File access/ Modification/ Status Change 
	printf("Last file access:         %s", ctime(&file_info->st_atime));
	printf("Last file modification:   %s", ctime(&file_info->st_mtime));
	printf("Last status change:       %s", ctime(&file_info->st_ctime));

}

int main(int argc, char const *argv[])
{
	struct stat file_info;
	DIR *d;
	struct dirent *dir;	


	if (argc > 1){
		//Loop through the argv and print the input files
		for (int i = 1; i < argc; i++){
			if (stat(argv[i], &file_info) == 0){
				print_stat(&file_info, argv[i]);
			} else {
				printf("%s file not found.\n", argv[i]);
			}
		}
	} else {
		d = opendir(".");
		if (d){
			//Loop through the directory and print the file information
			while ((dir = readdir(d)) != NULL){
				if (dir->d_type == DT_REG){
					if (stat(dir->d_name, &file_info) == 0){
						print_stat(&file_info, dir->d_name);
					} else {
						return EXIT_FAILURE;
					}
				}
			}
		}
		closedir(d);
	}
		
	return 0;
}

