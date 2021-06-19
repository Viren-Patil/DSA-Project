// Decryption of secret message.
void secret_decryption_image(int size_hidden_image, FILE *pf1, FILE *pf2, int fd) {

	int file_buff = 0, i, j = 0;
	int ch, bit_msg, t;
	
	for (i = 0; i < (size_hidden_image * 8); i++) {
		j++;
		ch = fgetc(pf1);
		read(fd, &t, sizeof(int));
		
		if(t % 2 == 0) {
			bit_msg = (ch & 2) / 2;
		}
		else if(t % 2 != 0) {
			bit_msg = ch & 1;
		}
		
		if (bit_msg) {
			file_buff = (file_buff << 1) | 1;
		}
		else {
			file_buff = file_buff << 1;
		}
		
		if (j == 8) {
			fputc(file_buff, pf2);
			j=0;
			file_buff = 0;
		}
	}
}

//Decoding function
int Decode_image(char *argv_1, char *argv_2, char *argv_3) {

	FILE *pf1, *pf2;
	int fd;
	int size, size1, temp;

	//Opening the Stego image.
	if((pf1 = fopen(argv_1, "r")) == NULL) {
		printf("Could not open file %s.\n", argv_1);
		return 1;
	}

	fseek(pf1, 54, SEEK_SET);
	
	//Opening the original file which is actually hidden under the cover image.
	if((pf2 = fopen(argv_2, "w+")) == NULL) {
		printf("Could not open file\n");
		return 1;
	}
	
	//Opening the file of random integers
	if((fd = open(argv_3, O_RDONLY, S_IRUSR | S_IWUSR)) == -1) {
		printf("Could not open file %s.\n", argv_3);
		return 1;
	}
	
	//Code word/phrase
	char codeword[30], codeword1[30];
	int j;

	size_decryption(pf1, &size, fd);
	string_decryption(pf1, codeword, size, fd);

	printf("\nEnter the  code word/phrase : \t");
	
	for(j = 0; (codeword1[j] = getchar()) != '\n'; j++);
	
	codeword1[j]= '\0';
	
	temp = strcmp(codeword1, codeword);

	if(temp) {
		printf("\n\t*** Entered wrong code word/phrase! You are not eligible to have access to this data as it is classified ***\n\n");
		return 0;
	}
	else {
		printf("\n\t*** Code word/phrase accepted ***\n\n");
	}

	//Password
	char passwd[20], passwd_str[20];

	size_decryption(pf1, &size1, fd);
	string_decryption(pf1, passwd_str, size1, fd);

	printf("Enter the password : \t");
	scanf("%s", passwd);
	
	temp = strcmp(passwd,passwd_str);

	if(temp) {
		printf("\n\t*** You entered the wrong password! You are not eligible to have access to this data as it is classified ***\n");
		return 0;
	}
	else {
		printf("\n\t*** Password Accepted ***\n");
	}

	//Decryption of the stego image
	int size_img;
	read(fd, &size_img, sizeof(int));
	secret_decryption_image(size_img, pf1, pf2, fd);

	//Closing files
	fclose(pf1);
	fclose(pf2);
	close(fd);
	return 0;
}
