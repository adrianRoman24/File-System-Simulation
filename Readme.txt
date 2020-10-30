	Bash-like command line interface
	
	This is an implementation of a bash command line simulation
which supports Linux-like file system usage.

	The program uses a structure of a tree with a given number of
children represented by a vector of pointers to structure. The strucure
is an inode which can be either a directory or a file. The program
checks the type of an inode by verifying if the last character is '/'
or not.

	The application has functionalities such as:
- printing the current directory
- adding files or directories to the current directory
- changing path to a relative or absolute path
- printing the absolute path
- printing directory recursively
- removing file or directory
- sorting directory
- finding all occurences of a file or directory
- moving source directory to a destination directory