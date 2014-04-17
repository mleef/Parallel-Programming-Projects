// While not particulary secure... it turns out this strategy also isn't too 
// bad (as long as the keys are kept secret and have different lengths and 
// aren't reused together)... or at least that is what a member of the security
// group that works in crypto told me.

#include "key.h"
#include <stdio.h>
#include <string.h>
#define RAND_SEED 2701
#include <sys/stat.h>

// utility function: given a list of keys, a list of files to pull them from, 
// and the number of keys -> pull the keys out of the files, allocating memory 
// as needed
void getKeys(xorKey** keyList, FILE** fileList, int numKeys)
{
  int keyLoop=0;
  for(keyLoop=0;keyLoop<numKeys;keyLoop++)
  {
     keyList[keyLoop]=(xorKey*)malloc(sizeof(xorKey));
     keyList[keyLoop]->myKey=(char*)malloc(sizeof(char)*MAXKEYLENGTH);
     fscanf(fileList[keyLoop],"%s",keyList[keyLoop]->myKey);
     keyList[keyLoop]->myKeyLength=strlen(keyList[keyLoop]->myKey);
     fclose(fileList[keyLoop]);
  }
}
//Given text, a list of keys, the length of the text, and the number of keys, encodes the text
void encode(char* plainText, xorKey** keyList, int ptextlen, int numKeys)
{
  int keyLoop=0;
  int charLoop=0;
  for(charLoop=0;charLoop<ptextlen;charLoop++)
  {
    char cipherChar=plainText[charLoop]; 
    for(keyLoop=0;keyLoop<numKeys;keyLoop++)
    {
       cipherChar=cipherChar ^ getBit(*(keyList[keyLoop]),charLoop);
    }
    plainText[charLoop]=cipherChar;
  }
}

void decode(char* plainText, xorKey** keyList, int ptextlen, int numKeys)
{
  encode(plainText,keyList,ptextlen,numKeys); //isn't symmetric key cryptography awesome? 
}
int main(int argc, char* argv[])
{
  if(argc<=2)
  {
      printf("Usage: %s <fileToEncrypt> <key1> <key2> ... <key_n>\n",argv[0]);
  }
  
  off_t textLength=fsize(argv[1]); //length of our text
  FILE* ptextFile=(FILE*)fopen(argv[1],"rb"); //The intel in plaintext
  FILE* encryptedFile=(FILE*)fopen("encryptedOut","wb");
  FILE* decryptedFile=(FILE*)fopen("decryptedOut","wb");
  char* ptextString=(char*)malloc(1+sizeof(char)*textLength);
  fread(ptextString,textLength,1,ptextFile);
  ptextString[textLength]='\0';
  int ptextlen=strlen(ptextString); //length of that string
  printf("FILE LENGTH: %d\n",ptextlen);
  srand(RAND_SEED); //seed the random number generator
  int numKeys=argc-2;
  FILE** fileList=(FILE**)malloc(sizeof(FILE*)*numKeys);
  
  xorKey** keyList=(xorKey**)malloc(sizeof(xorKey*)*numKeys); //form a list of keys of that size
  int keyLoop=0; //loop counter
  for(keyLoop=0;keyLoop<numKeys;keyLoop++)
  {
    fileList[keyLoop]=(FILE*)fopen(argv[keyLoop+2],"r");
  }
  getKeys(keyList,fileList,numKeys); 
  encode(ptextString,keyList,ptextlen,numKeys); //store encoded result back in ptextString
  fwrite(ptextString,textLength,1,encryptedFile);
  decode(ptextString,keyList,ptextlen,numKeys);
  fwrite(ptextString,textLength,1,decryptedFile);
  fclose(encryptedFile);
  fclose(decryptedFile);
  
  return 0;

}
