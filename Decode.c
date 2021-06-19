//RSA decryption
int RSA1(int asci) {
	int p = 13, q = 11, e = 17, d, i, a[20][4], P;
	int n = p * q;
	int phi = (p - 1) * (q - 1);
	a[0][0] = a[1][1] = 1;
	a[0][1] = a[1][0] = 0;
	a[0][2] = phi;
	a[1][2] = e;
	a[1][3] = a[0][2] / a[1][2];
	for(i = 2; ; i++) {
		a[i][0] = a[i - 2][0] - (a[i - 1][0] * a[i - 1][3]);
		a[i][1] = a[i - 2][1] - (a[i - 1][1] * a[i - 1][3]);
		a[i][2] = a[i - 2][2] - (a[i - 1][2] * a[i - 1][3]);
		a[i][3] = a[i - 1][2] / a[i][2];
		if(a[i][2] == 1) {
			break;
		} 
	}
	if(a[i][1] < 0) {
		a[i][1] = phi + a[i][1];
	}
	d = a[i][1];
	
	P = mod(asci, n, d);
	
	return P + 32;
}

// Decryption of sizes.
void size_decryption(FILE *pf1, int *size_txt, int fd) {
	
	int file_buff = 0, i;
	int ch, bit_msg, t;
	
	for (i = 0; i < 8; i++) {
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
	}
	*size_txt = file_buff;
}

// Decryption of codeword and password.
void string_decryption(FILE *pf1,char *strng,int size, int fd) {

	int file_buff = 0, i, j = 0, k = 0;
	int ch, bit_msg, t;
	
	for (i = 0; i < (size * 8); i++) {
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
			file_buff = RSA1(file_buff); 
			strng[k] = (char)file_buff;
			j=0;
			k++;
			file_buff = 0;
		}
	}
	strng[k] = '\0';
}

// Decryption of secret message.
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2, int fd) {

	int file_buff = 0, i, j = 0;
	int ch, bit_msg, t;
	// char output[500] = {'\0'};
	//char *output;
	
	for (i = 0; i < (size_txt * 8); i++)
	{
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
			file_buff = RSA1(file_buff);
			fputc(file_buff, pf2);
		//	output = (char*)malloc(sizeof(char));
		//	output[k++] = (char)file_buff;
			j=0;
			file_buff = 0;
		}
	}
	//output = (char*)malloc(sizeof(char));
	//printf("k = %d\n", k);
	//output[k] = '\0';
	//printf("\n*** Secret Text Is ==> %s\n\n", output);
}

//Decoding function
int Decode(char *argv_1, char *argv_2, char *argv_3) {

	FILE *pf1, *pf2;
	int fd;
	int size, size1, size_txt, temp;

	//Opening the image file using argv_1
	if((pf1 = fopen(argv_1, "r")) == NULL) 
	{
		printf("Could not open file %s.\n", argv_1);
		return 1;
	}

	fseek(pf1, 54 ,SEEK_SET);
	
	//Opening the output text file using argv_2
	if((pf2 = fopen(argv_2, "w+")) == NULL) 
	{
		printf("Could not open file %s.\n", argv_2);
		return 1;
	}
	
	//Opening the file of random integers using argv_3
	if((fd = open(argv_3, O_RDONLY, S_IRUSR | S_IWUSR)) == -1) 
	{
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

	if(temp)
	{
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

	if(temp)
	{
		printf("\n\t*** You entered the wrong password! You are not eligible to have access to this data as it is classified ***\n\n");
		return 0;
	}
	else
		printf("\n\t*** Password Accepted ***\n");

	//Secret Text
	//size_decryption(pf1, &size_txt, fd);
	read(fd, &size_txt, sizeof(int));
	secret_decryption(size_txt, pf1, pf2, fd);

	printf("*** The Secret Text Is Copied To ==> %s\n\n", argv_2);

	//Closing files
	fclose(pf1);
	fclose(pf2);
	close(fd);
	
	return 0;
}
