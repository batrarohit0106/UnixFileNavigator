#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void readFile(int argc, char *argv[])
{
    //./assgn read file.txt start bytes
    if (argc != 5)
    {
        printf("Invalid number of arguments. \n");
    }
    else
    {
        // Open file in read only mode
        int fd = open(argv[2], O_RDONLY);
        if (fd == -1)
        {
            printf("Error in opening file. \n");
        }
        else
        {
            // Convert string to integer for start,bytes
            int start = atoi(argv[3]);
            int nbytes = atoi(argv[4]);
            // Move pointer for random read
            if (start != 0)
            {
                lseek(fd, start, SEEK_SET);
            }
            int n, bytes_read = 0;
            char buff[1]; // Initialize buffer to store data
            printf("\nThe bytes read are: ");
            // Keep reading till end of file or max bytes to be read is reached
            while (((n = read(fd, buff, 1)) > 0) && (bytes_read < nbytes))
            {
                printf("%s", buff);
                bytes_read += n;
            }
            // print total bytes read
            printf("\n\nTotal bytes read: %d \n", bytes_read);
            // Close file
            if ((close(fd)) < 0)
            {
                printf("Error in closing file. \n");
            }
        }
    }
}

void infoFile(int argc, char *argv[])
{
    //./assgn info file.txt
    if (argc != 3)
    {
        printf("Inavalid number of arguments.");
    }
    else
    {
        // Define stat to store file info
        struct stat sfile;

        if (stat(argv[2], &sfile) == -1)
        {
            printf("Error Occurred\n");
        }
        printf("\nInformation for file: %s \n", argv[2]);

        // Accessing data members of stat struct
        printf("\nUser ID of owner: %d", sfile.st_uid);
        printf("\nBlocksize for system I/O: %ld", sfile.st_blksize);
        printf("\nGroup ID of owner: %d", sfile.st_gid);
        printf("\nNumber of blocks allocated: %ld", sfile.st_blocks);
        printf("\nTotal size, in bytes: %ld", sfile.st_size);
        printf("\nNumber of hard links: %u", (unsigned int)sfile.st_nlink);
        printf("\nTime of last access: %s", ctime(&sfile.st_atim.tv_sec));
        printf("\nTime of last modification: %s", ctime(&sfile.st_mtim.tv_sec));
        printf("\nTime of last status change: %s", ctime(&sfile.st_ctim.tv_sec));
        printf("\nFile Permissions for User: ");
        printf((sfile.st_mode & S_IRUSR) ? "r" : "-");
        printf((sfile.st_mode & S_IWUSR) ? "w" : "-");
        printf((sfile.st_mode & S_IXUSR) ? "x" : "-");
        printf("\nFile Permissions for Group: ");
        printf((sfile.st_mode & S_IRGRP) ? "r" : "-");
        printf((sfile.st_mode & S_IWGRP) ? "w" : "-");
        printf((sfile.st_mode & S_IXGRP) ? "x" : "-");
        printf("\nFile Permissions for Other: ");
        printf((sfile.st_mode & S_IROTH) ? "r" : "-");
        printf((sfile.st_mode & S_IWOTH) ? "w" : "-");
        printf((sfile.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");
    }
}

mode_t Mode(char *perm)
{
    // Converts string to mode_t
    mode_t mode = 0;
    if (perm[0] == 'r')
        mode |= 0400;
    if (perm[1] == 'w')
        mode |= 0200;
    if (perm[2] == 'x')
        mode |= 0100;
    if (perm[3] == 'r')
        mode |= 0040;
    if (perm[4] == 'w')
        mode |= 0020;
    if (perm[5] == 'x')
        mode |= 0010;
    if (perm[6] == 'r')
        mode |= 0004;
    if (perm[7] == 'w')
        mode |= 0002;
    if (perm[8] == 'x')
        mode |= 0001;
    return mode;
}

void createFile(int argc, char *argv[])
{
    //./assgn create file.txt rwxrwxrwx
    mode_t old_mask = umask(0); // For providing write permission to others
    if (argc != 4)
    {
        printf("Invalid number of arguments.");
    }
    else
    {
        // Check if file exists by opening it first
        int op = open(argv[2], O_RDONLY);
        if (op != -1)
        {
            close(op);
            printf("File already exists. Do you want to truncate it(y/n)?");
            char ch;
            scanf("%c", &ch);
            if (ch == 'n')
            {
                return;
            }
        }
        mode_t mode = Mode(argv[4]);
        // Create file or truncate it
        int fd = creat(argv[3], mode);
        if (fd == -1)
        {
            printf("Error while creating a file. \n");
        }
        else
        {
            if (op != -1)
            {
                printf("File truncated. \n");
            }
            else
            {
                printf("File created successfully. \n");
            }
        }
    }
    umask(old_mask);
}

int findOpenMode(char *m)
{
    int x;
    if (strcmp(m, "read") == 0)
    {
        x = O_RDONLY;
    }
    else if (strcmp(m, "write") == 0)
    {
        x = O_WRONLY;
    }
    else if (strcmp(m, "readwrite") == 0)
    {
        x = O_RDWR;
    }
    else
    {
        x = -1;
    }
    return x;
}

void openAndCloseFile(int argc, char *argv[])
{
    //./assgn openandclose file.txt mode
    if (argc != 4)
    {
        printf("Invalid number of arguments. \n");
    }
    else
    {
        // convert string mode to integer
        int mode = findOpenMode(argv[3]);
        if (mode == -1)
        {
            printf("/nInvalid opening mode");
            return;
        }
        // Open a file
        int fd = open(argv[2], mode);
        if (fd == -1)
        {
            printf("Error in opening file. \n");
        }
        else
        {
            printf("File opened successfully in %s mode. \n", argv[3]);
            // Close a file
            if ((close(fd)) < 0)
            {
                printf("Error in closing file. \n");
            }
            else
            {
                printf("File closed successfully. \n");
            }
        }
    }
}

void writeFile(int argc, char *argv[])
{
    //./assgn write file.txt start bytes data
    if (argc != 6 && argc != 5)
    {
        printf("Invalid number of arguments. \n");
    }
    else
    {
        // Open a file in write mode. Create if not present
        int fd = open(argv[2], O_WRONLY | O_CREAT);
        if (fd == -1)
        {
            printf("Error in opening file. \n");
        }
        else
        {
            // Create buffer
            char data[512];
            // If data is not present as command line argument take input from user
            if (argc == 6)
            {
                printf("Enter the data to be wriiten(press ';' to end input)\n");
                scanf("%[^;]s", data); // Multi-line input allowed
            }
            else
            {
                // Copy command line data in buffer
                strncpy(data, argv[5], sizeof(argv[5]));
            }
            int size = 0;
            while (data[size] != '\0')
            {
                size++;
            } // Convert string to integer for start and bytes
            int start = atoi(argv[3]);
            int bytes = atoi(argv[4]);
            // If less data is avialble in buffer then change bytes to size of data
            if (bytes > size)
            {
                bytes = size;
            }
            // For random access
            lseek(fd, start, SEEK_SET);
            // Print total bytes written
            int bytes_written = write(fd, data, bytes);
            printf("\n\nTotal bytes written: %d \n", bytes_written);
            // Close file
            if ((close(fd)) < 0)
            {
                printf("Error in closing file. \n");
            }
        }
    }
}

void main(int argc, char *argv[])
{

    printf("Welcome to program: %s\n\n", argv[0]);

    if (argc < 2)
    { //./assgn type operation
        printf("Please pass appropriate number of command line arguments for executing a function.\n");
    }
    else
    {
        if (strcmp(argv[1], "create") == 0)
        {
            createFile(argc, argv);
        }
        else if (strcmp(argv[1], "read") == 0)
        {
            readFile(argc, argv);
        }
        else if (strcmp(argv[1], "write") == 0)
        {
            writeFile(argc, argv);
        }
        else if (strcmp(argv[1], "open") == 0)
        {
            openAndCloseFile(argc, argv);
        }
        else if (strcmp(argv[1], "info") == 0)
        {
            infoFile(argc, argv);
        }
        else
        {
            printf("Invalid command for files.\n");
        }
    }
}