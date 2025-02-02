// Enable EFS
// If compiled as an exe: gcc efs.c -o efs.exe 
// If compiled as a bof:  gcc efs.c -c -o efs.o -DCOMPILE_BOF

#include <windows.h>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wint-conversion"
#define VERBOSE TRUE

#ifdef COMPILE_BOF

#warning "Compiling the BOF version of the code"

#include "beacon.h"

#define CSIDL_LOCAL_APPDATA 0x001c

#define IMPORT_RESOLVE FARPROC GetLastError = Resolver("kernel32", "GetLastError");  \
    FARPROC CreateFile = Resolver("kernel32", "CreateFileA"); \
    FARPROC CloseHandle = Resolver("kernel32", "CloseHandle"); \


#define printf(format, args...) { BeaconPrintf(CALLBACK_OUTPUT, format, ## args); }
DECLSPEC_IMPORT FARPROC WINAPI kernel32$GetProcAddress(HANDLE, CHAR*);
DECLSPEC_IMPORT HANDLE WINAPI kernel32$LoadLibraryA(CHAR*);

#else

#warning "Compiling the EXE version of the code"

#define IMPORT_RESOLVE ""

#endif

FARPROC Resolver(CHAR *lib, CHAR *func) {
#ifdef COMPILE_BOF
    FARPROC ptr = kernel32$GetProcAddress(kernel32$LoadLibraryA(lib), func);
    if(VERBOSE) {
        printf("%s$%s located at 0x%p\n", lib, func, ptr);
    }
    return ptr;
#else
    FARPROC ptr = GetProcAddress(LoadLibraryA(lib), func);
    if(VERBOSE) {
        printf("%s!%s located at 0x%p\n", lib, func, ptr);
    }
    return ptr;
#endif
}

VOID MakeFile() {
    IMPORT_RESOLVE;
    HANDLE file = CreateFile(".\\test.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ENCRYPTED|FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (!file || file == INVALID_HANDLE_VALUE) {
        printf("Failed to get a file HANDLE on file. ERROR: %d\n", GetLastError());
        return;
    }
    CloseHandle(file);
}

VOID go(char *argv, int argc) {
    printf("Enabling EFS Service...\n\n");
    MakeFile();
}

#ifndef COMPILE_BOF

int main() {
    go(NULL, 0);
    return 0;
}

#endif