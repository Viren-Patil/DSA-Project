//Repeated square algorithm to calculate mod faster.
int mod(int base, int n, int exp) {
	int  prev, arr[8] = {0}, bin[8] = {0}, i, j = 1, res = 1;
	prev = base % n;
	arr[0] = prev;
	for(i = 2; i <= exp; i = i * 2, j++) {
		prev = (prev * prev) % n;
		arr[j] = prev;
	}
	
	for(i = 0; exp != 1; i++) {
		bin[i] = exp % 2;
		exp = exp / 2;
	}
	bin[i] = 1;
	
	for(i = 0; i < 8; i++) {
		if(bin[i] == 1) {
			res = res * arr[i];
		}
	}
	
	res = res % n;
	return res;
}

//RSA encryption
int RSA(int asci) {
	int p = 13, q = 11, e = 17, P, C;
	int n = p * q;
	
	if(asci >= 32 && asci <= 63) {
		P = asci % 32;
		C = mod(P, n, e);
	}
	else if(asci >= 64 && asci <= 95) {
		P = (asci % 64) + 32;
		C = mod(P, n, e);
	}
	else if(asci >= 96 && asci <= 126) {
		P = (asci % 96) + 64;
		C = mod(P, n, e);
	}
	
	return C;
}

//Reading size of image file
int size_of_image(FILE *fp1) {
	int width, height;

	fseek(fp1, 0x12, SEEK_SET);	//Seek to the Width position
	fread(&width, sizeof(int), 1, fp1);	//Reading Width
	fread(&height, sizeof(int), 1, fp1);	//Read height
	printf("\n\nDimensions of the Image is (%dpx) x (%dpx) \n", width, height);
	fseek(fp1, 0L, SEEK_SET);	//Seek to the start position

	return ((width * height * 3) / 8);
}

//Entering Secret Message to file
void secret_text(FILE *fp2) {
	char ch;
	while ((ch = getchar())!='\n') {
		putc(ch,fp2);	//puts secret text to file
	}
}

//Size Of The Secret Message
int secret_text_size(FILE *fp2) {
	int size_txt;
	fseek(fp2, 0L, SEEK_END);
	size_txt = ftell(fp2);
	fseek(fp2, 0L, SEEK_SET);
	return size_txt;
}

//Getting the bits of the text.
int get_bit(int byte, int bit) {
	return ((byte >> (8 - bit)) & 1);
}

//Encryption of message
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3, int fd) {
	char file_buff, msg_buff;
	int i;
	int msb_msg, file_byte_lsb, x, asci, t;
	// Used current time as a seed for the random generator.
	srand(time(0));
	
	while((msg_buff = fgetc(fp2)) != EOF) {
		asci = msg_buff;
		// Encrypting my text.
		x = RSA(asci);
		for(i = 1; i <= 8; i++) {
		
			file_buff = fgetc(fp1);
			
			t = (rand() % 901) + 100 ;
			write(fd, &t, sizeof(int));
			
			if(t % 2 == 0) {
				file_byte_lsb = (file_buff & 2) / 2;
			}
			else if(t % 2 != 0){
				file_byte_lsb = file_buff & 1;
			}
			// Getting the bits of the message starting from msb.
			msb_msg = get_bit(x, i);

			if(file_byte_lsb == msb_msg) {
				fputc(file_buff, fp3);
			}
			else {
				if(file_byte_lsb == 0) {
					if(t % 2 == 0) {
						file_buff = file_buff | 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff | 1;
					}
				}
				else {
					if(t % 2 == 0) {
						file_buff = file_buff ^ 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff ^ 1;
					}
				}

				fputc(file_buff, fp3);
			}
		}
	}

	/*copying rest of data */
	while(!feof(fp1)) {
		char tmp_cpy = fgetc(fp1);
		fputc(tmp_cpy,fp3);

	}

	if(msg_buff == EOF) {
		printf("\n\t*** Secret Message Encrypted Successfully ***\n\n");
	}
	else {
		printf("\n*** Failed to Encrypt ***\n");
	}
}

//Encryption for strings (codeword/password).
void string_encrypt(char *str, FILE *fp1, FILE *fp3, int fd) {

	char file_buff, msg_buff;
	int i, j = 0;
	int msb_msg, file_byte_lsb, x, asci, t;
	// Used current time as a seed for the random generator.
	srand(time(0));
	
	while((msg_buff = str[j]) != '\0') {
		asci = msg_buff;
		//Encrypting the secret string (codeword/password).
		x = RSA(asci);
		for(i = 1; i <= 8; i++) {
		
			file_buff = fgetc(fp1);
			
			t = (rand() % 901) + 100;
			write(fd, &t, sizeof(int));
			if(t % 2 == 0) {
				file_byte_lsb = (file_buff & 2) / 2;
			}
			else if(t % 2 != 0){
				file_byte_lsb = file_buff & 1;
			}
			// Getting the bits of the message starting from msb.
			msb_msg = get_bit(x, i);

			if(file_byte_lsb == msb_msg) {
				fputc(file_buff, fp3);
			}
			else {
				if(file_byte_lsb == 0) {
					if(t % 2 == 0) {
						file_buff = file_buff | 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff | 1;
					}
				}
				else {
					if(t % 2 == 0) {
						file_buff = file_buff ^ 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff ^ 1;
					}
				}

				fputc(file_buff, fp3);
			}
		}
		j++;
	}
}

//Encryption for size.
void size_encrypt(int num, FILE *fp1, FILE *fp3, int fd) {

	char file_buff;	
	int i;
	int msb_msg, file_byte_lsb, t;
	// Used current time as a seed for the random generator.
	srand(time(0));
	
	for(i = 1; i <= 8; i++) {
		
			file_buff = fgetc(fp1);
			
			t = (rand() % 901) + 100;
			write(fd, &t, sizeof(int));
			if(t % 2 == 0) {
				file_byte_lsb = (file_buff & 2) / 2;
			}
			else if(t % 2 != 0){
				file_byte_lsb = file_buff & 1;
			}
			// Getting the bits of the message starting from msb.
			msb_msg = get_bit(num, i);

			if(file_byte_lsb == msb_msg) {
				fputc(file_buff, fp3);
			}
			else {
				if(file_byte_lsb == 0) {
					if(t % 2 == 0) {
						file_buff = file_buff | 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff | 1;
					}
				}
				else {
					if(t % 2 == 0) {
						file_buff = file_buff ^ 2;
					}
					else if(t % 2 != 0) {
						file_buff = file_buff ^ 1;
					}
				}

				fputc(file_buff, fp3);
			}
	}

}


//Encoding function
int Encoding(char *argv_1, char *argv_2, char *argv_3) {

	FILE *fp1, *fp2, *fp3;
	int fd;
	//Opening the image file
	if((fp1 = fopen(argv_1, "r+")) == NULL) {
		printf("Could not open file %s.\n", argv_1);
		return 1;
	}
	printf("\n\n*****************************************************************************************************************************************");
	printf("\nWELCOME TO MULTI LEVEL ENCRYPTER!\nIN THIS PROGRAM YOU WILL WITNESS THE ENCRYPTION\nAS WELL AS THE ENCODING OF YOUR SECRET MESSAGE.\n");
	printf("*****************************************************************************************************************************************");

	//Calculating size of image and the number of characters that can be stored in the cover image.
	int size_image = size_of_image(fp1);
	printf("Total %d characters can be stored in %s.\n", size_image, argv_1);

	//Opening secret text file
	fp2 = fopen(argv_2, "w+");
	if(fp2 == NULL) {
		printf("Could not open file %s.\n", argv_2);
		return 1;
	}
	
	//Entering secret text to file
	printf("Enter your secret text below and Press ENTER To Stop:\t");
	secret_text(fp2);
	
	//Calculating size of secret text
	int size_txt = secret_text_size(fp2);
	printf("\nSize of The Secret Message is ==> %d\n", size_txt);

	//Comparing Image Size With Text
	if(size_image < size_txt) {
		printf("\n*** The Size Of Message Exceeds the Size of image ***\n");
		return 1;
	}

	//Opening destination file
	fp3 = fopen(argv_3, "w+");
	if (fp3== NULL) {
		fprintf(stderr, "Cannot create output file %s\n",argv_3);
		exit(1);
	}

	//Copying 54 header bytes from original file.
	int i, c = 0;
	char tmp_cpy;
	for(i = 0;i < 54; i++) {
		tmp_cpy = fgetc(fp1);
		fputc(tmp_cpy,fp3);
		c++;
	}

	//Checking 54 bytes copied or not
	if(i == c) {
		printf("\n\t*** Sucessfully copied 54 header bytes ***\n");
	}
	else {
		printf("\n*** Failed to copy 54 header bytes ***\n");
		return 0;
	}
	
	fd = open("random.txt", O_CREAT | O_WRONLY | O_RDONLY, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		fprintf(stderr, "Cannot create random.txt\n");
		exit(1);
	}
	
	//For code word/phrase
	char magic_str[10];
	char magic;
	int size;

	printf("\nEnter a code word/phrase : \t");

	for(i = 0; (magic = getchar()) != '\n'; i++) {
		magic_str[i] = magic;
	}

	magic_str[i] = '\0';
	size = strlen(magic_str);
	printf("Size of the code word/phrase is ==> %d\n", size);
	size_encrypt(size,fp1,fp3, fd);
	string_encrypt(magic_str,fp1,fp3, fd);
	printf("\n\t*** Code word/phrase encrypted ***\n");

	//For password
	char str[20];
	printf("\nEnter the password : \t");

	for (i=0;((str[i] = getchar()) != '\n');i++);
	str[i]= '\0';

	int size1;	
	size1 = strlen(str);
	printf("Size Of The Password is ==> %d\n", size1);
	size_encrypt(size1,fp1,fp3, fd);
	string_encrypt(str,fp1,fp3, fd);
	printf("\n\t*** Password Encrypted ***\n");

	//Encryption for secret message
	//size_encrypt(size_txt,fp1,fp3, fd);
	write(fd, &size_txt, sizeof(int));
	stega_encrypt(fp1,fp2,fp3, fd);

	//Closing all files 
	fclose(fp1);	
	fclose(fp2);
	fclose(fp3);
	close(fd);
	return 0;
}
