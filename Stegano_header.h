#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

//reading size of image file
int size_of_image(FILE *fp1);

//Entering Secret Message to file
void secret_text(FILE *fp2);

//Size Of The Secret Message
int secret_text_size(FILE *fp2);

//bits of text file
int get_bit(int byte, int bit);

//encription of message
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3, int fd);

//Encription For Strings
void string_encrypt(char *str,FILE *fp1, FILE *fp3, int fd);

//Encription For Numbers
void size_encrypt(int num,FILE *fp1, FILE *fp3, int fd);

/* decryption of sizes */
void size_decryption(FILE *pf1, int *size_txt, int fd);

/* decryption of strings*/
void string_decryption(FILE *pf1,char *strng,int size, int fd);

/* decryption of secret message*/
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2, int fd);

/*encryption of image*/
void stega_encrypt_image(FILE *fp1, FILE *fp2, FILE *fp3, int fd);

/*decryption of image.*/
void secret_decryption_image(int size_hidden_image, FILE *pf1, FILE *pf2, int fd);
