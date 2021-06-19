//Encryption of message
void stega_encrypt_image(FILE *fp1, FILE *fp2, FILE *fp3, int fd) {
	char file_buff, msg_buff;
	int i;
	int msb_msg, file_byte_lsb, t;
	// Used current time as a seed for the random generator.
	srand(time(0));
	
	while(!feof(fp2)) {
		fread(&msg_buff, 1, 1, fp2);
	
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
			msb_msg = get_bit(msg_buff, i);

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

	//Copying rest of the data.
	while(!feof(fp1)) {
		char tmp_cpy = fgetc(fp1);
		fputc(tmp_cpy,fp3);

	}

	printf("Image hidden successfully!!\n");
}


//Encoding function
int Encoding_image(char *argv_1, char *argv_2, char *argv_3) {

	FILE *fp1, *fp2, *fp3;
	int fd;
	
	//Opening the cover image file
	if((fp1 = fopen(argv_1, "r")) == NULL) {
		printf("Cover file\n");
		printf("Could not open file %s.\n", argv_1);
		return 1;
	}
	
	printf("\n\n*****************************************************************************************************************************************");
	printf("\nWELCOME TO MULTI LEVEL ENCRYPTER!\nIN THIS PROGRAM YOU WILL WITNESS THE ENCRYPTION AS WELL AS THE ENCODING OF YOUR SECRET MESSAGE.\n");
	printf("*****************************************************************************************************************************************\n");
	
	//Calculating size of cover image and the number of characters that can be stored in the cover image.
	int size_image = size_of_image(fp1);
	printf("Total %d characters can be stored in %s.\n", size_image, argv_1);

	//Opening the image file which is to be hidden.
	fp2 = fopen(argv_2, "r");
	if(fp2 == NULL) {
		printf("Image to be hidden\n");
		printf("Could not open file %s.\n", argv_2);
		return 1;
	}
	
	//Opening destination file.
	fp3 = fopen(argv_3, "w+");
	if (fp3== NULL) {
		fprintf(stderr, "Cannot create output file %s\n",argv_3);
		exit(1);
	}
	
	//Copying 54 header bytes from original cover file.
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
	
	//Opening file to store random integers during encoding.
	fd = open("random.txt", O_CREAT | O_WRONLY | O_RDONLY, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		fprintf(stderr, "Cannot create random.txt\n");
		exit(1);
	}
	
	//For code word/phrase
	char codeword[21];
	char code;
	int size;

	printf("\nEnter a code word/phrase of your choice(not more than 20 characters) :   ");

	for(i = 0; (code = getchar()) != '\n'; i++) {
		codeword[i] = code;
	}

	codeword[i] = '\0';
	size = strlen(codeword);
	printf("Size of the code word/phrase is ==> %d\n", size);
	size_encrypt(size,fp1,fp3, fd);
	string_encrypt(codeword,fp1,fp3, fd);
	printf("\n\t*** Code word/phrase encrypted ***\n");

	//For password
	char str[20];
	printf("\nEnter a password(max 20 characters) :   ");

	for (i=0;((str[i] = getchar()) != '\n');i++);
	str[i]= '\0';

	int size1;	
	size1 = strlen(str);
	printf("Size Of The Password is ==> %d\n", size1);
	size_encrypt(size1,fp1,fp3, fd);
	string_encrypt(str,fp1,fp3, fd);
	printf("\n\t*** Password Encrypted ***\n");

	//Encryption for secret message
	int size_img = 0;
	char ch;
	while(!feof(fp2)) {
		fread(&ch, 1, 1, fp2);
		size_img++;
	}

	write(fd, &size_img, sizeof(int));
	fseek(fp2, 0, SEEK_SET);
	
	stega_encrypt_image(fp1, fp2, fp3, fd);

	//Closing all files 
	fclose(fp1);	
	fclose(fp2);
	fclose(fp3);
	close(fd);
	return 0;
}
