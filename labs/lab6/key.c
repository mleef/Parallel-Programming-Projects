#include "key.h"

#include <sys/stat.h>

//return byte from key at offset "position"
char getBit(xorKey skey, int position) {
   return skey.myKey[position%skey.myKeyLength];
} 

//utility function to generate a key of length "length"
void genKey( xorKey *pkey,int length) {
  pkey->myKeyLength=length;
  pkey->myKey=(char*)malloc(sizeof(char)*length);
  int i=0;
  for(i=0;i<length;i++) {
     pkey->myKey[i]=(char)(rand()%cursysCharLen);
  } 
}

// utility function to get the length of a file -- very important since
//     some actual data bytes may be 0 (the null char indicating the end
//     of a string.
// generously donated by Stackoverflow: http://stackoverflow.com/questions/8236/how-do-you-determine-the-size-of-a-file-in-c
off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0) {
        return st.st_size;
    }

    return -1; 
}
