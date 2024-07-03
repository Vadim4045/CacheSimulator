#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

int get_current_dir(char *buff, int size){
	char exe_path[size];

#ifdef __linux__
	ssize_t len = readlink("/proc/self/exe", exe_path, size - 1);
	if (len != -1){
		exe_path[len] = '\0';
	}else{
		perror("readlink");
		return 1;
	}
#elif __APPLE__
	uint32_t bufsize = size;
	if (_NSGetExecutablePath(exe_path, &bufsize)){
		fprintf(stderr, "Buffer too small; need size %u\n", bufsize);
		return 1;
	}
#elif _WIN32
	DWORD len = GetModuleFileName(NULL, exe_path, size);
	if (len == 0){
		fprintf(stderr, "GetModuleFileName failed with error %lu\n", GetLastError());
		return 1;
	}
#else
	fprintf(stderr, "Unsupported platform\n");
	return 1;
#endif

	strncpy(buff, dirname(exe_path), size);
	return 0;
}

int get_all_files_in_dir(char *path, char **names_arr, int arr_size){
	DIR *dir;
	struct dirent *entry;
	int count = 0;

	if ((dir = opendir(path)) == NULL){
		perror("opendir() error");
		return -1;
	}else{
		while ((entry = readdir(dir)) != NULL && count < arr_size){
			if (entry->d_type == DT_REG){
				names_arr[count] = malloc(strlen(entry->d_name) + 1);
				if (names_arr[count] == NULL){
					perror("malloc() error");
					closedir(dir);
					return -1;
				}
				strcpy(names_arr[count], entry->d_name);
				++count;
			}
		}
		closedir(dir);
	}
	return count;
}