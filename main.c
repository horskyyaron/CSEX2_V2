#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

#define MAC_OS_ID 1
#define WIN_OS_ID 2
#define UNIX_OS_ID 3


const int SRC_ARG = 1;
const int DST_ARG = 2;
const int SRC_OS_ARG = 3;
const int DST_OS_ARG = 4;
const int ENDIANNESS_ARG = 5;

const int LITTLE_ENDAIN_BOM = 0xFF;

const char ENDIANNESS_NO_CHANEG_FLAG[] = "-keep";

const bool SWAP_ENDIANNESS = true;
const bool KEEP_ENDIANNESS = false;

const char MAC_OS_FLAG[] = "-mac";
const char WIN_OS_FLAG[] = "-win";
const char UNIX_OS_FLAG[] = "-unix";

void simpleCopy(char *srcFilePath, char *dstFilePath);

bool isValidFile(char *path);

bool areFilesFeasible(char *srcPath, char *dstPath);

//new line significant byte in the different OSes.
const unsigned short UNIX_NL_BIG = 0X000A;
const unsigned short UNIX_NL_LITTLE = 0X0A00;
const unsigned short MAC_NL_BIG = 0X000D;
const unsigned short MAC_NL_LITTLE = 0X0D00;
const unsigned int WIN_NL_BIG  = 0X000A000D;
const unsigned int WIN_NL_LITTLE = 0X0A000D00;

const unsigned char UNIX_NL_MSB = 0X0A;
const unsigned char MAC_NL_MSB = 0X0D;

const char DOT = '.';
const char END_OF_STRING = '\0';


int main(int argc, char *argv[]) {

    int numOfCmdLineArguments = argc - 1;

    switch(numOfCmdLineArguments) {
        case 2: {
            if(areFilesFeasible(argv[SRC_ARG], argv[DST_ARG]))
                simpleCopy(argv[SRC_ARG], argv[DST_ARG]);
            break;
        }
        case 4: {
            //osSensitiveCopy(argv[SRC_ARG], argv[DST_ARG], argv[SRC_OS_ARG], argv[DST_OS_ARG], NULL);
            break;
        }
        case 5: {
            //osSensitiveCopy(argv[SRC_ARG], argv[DST_ARG], argv[SRC_OS_ARG], argv[DST_OS_ARG], argv[ENDIANNESS_ARG]);
            break;
        }
        default: {
            //invalid number of arguments.
            exit(-1);
        }
    }
    return 0;
}

//check that the src and dst files had valid names and that the src file exists.
bool areFilesFeasible(char *srcPath, char *dstPath) {
    //check that the names are valid.
    if (isValidFile(srcPath) && isValidFile(dstPath)) {
        //checks that the src file exists.
        FILE *f = fopen(srcPath, "r");
        if(f != NULL) {
            fclose(f);
            return true;
        }
    }
    return false;
}

//check that file is of the form "X.suffix"
bool isValidFile(char *argument) {
    //check if a given argument is a text file.
    int argLength = strlen(argument);

    //check for empty string case.
    if(argLength == 0) {
        return false;
    }

    int validityCounter = 0;

    //scanning the string and check if file is of the form "X.suffix".
    char *ptr = argument;
    while (*ptr != END_OF_STRING) {
        if(*ptr != DOT && validityCounter == 0) {
            validityCounter++;
        } else if (*ptr == DOT && validityCounter == 1) {
            validityCounter++;
        } else if (*ptr != DOT && validityCounter == 2) {
            validityCounter++;
        }
        ptr++;
    }
    //only when the 3 conditions are met, the string is valid.
    return (validityCounter == 3);
}

//copy src file to dst file.
void simpleCopy(char *srcFilePath, char *dstFilePath) {

    FILE *srcFile = fopen(srcFilePath, "rb");
    FILE *dstFile = fopen(dstFilePath, "wb");

    //byteRead will hold each char read from the src file. (UTF-16, each char will be 2 bytes long).
    unsigned char byteRead[2];

    //reading the first char.
    size_t byteReadCount = fread(byteRead, sizeof(byteRead), 1, srcFile);

    while(byteReadCount != 0) {
        fwrite(byteRead, sizeof(byteRead), 1, dstFile);
        byteReadCount = fread(byteRead, sizeof(byteRead), 1, srcFile);
    }

}

