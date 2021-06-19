#include "Stegano_header.h"
#include "Encode.c"
#include "Encode_image.c"
#include "Decode.c"
#include "Decode_image.c"

int main(int argc,char *argv[]) {
    int i;
    char *argv_1,*argv_2,*argv_3;
    switch(argc) {
		case 8 :	if(!strcmp(argv[1],"-E") || !strcmp(argv[1], "-EI")) {
			    		for (i = 2; i < 8; i++) {
							if (!strcmp(argv[i],"-i")) {
				    			argv_1 = argv[i + 1];
				    		}
							if (!strcmp(argv[i],"-s")) {
				    			argv_2 = argv[i + 1];
				   			}
							if (!strcmp(argv[i],"-o")) {
			    				argv_3 = argv[i + 1];
			    			}
		    			}	
		    			if(!strcmp(argv[1],"-E")) {		    
		    				Encoding(argv_1, argv_2, argv_3);
			    		}
			    		else if(!strcmp(argv[1],"-EI")) {
			    			Encoding_image(argv_1, argv_2, argv_3);
			    		}
			    	}

					else if(!strcmp(argv[1], "-D") || !strcmp(argv[1], "-DI")) {
			    		for (i = 2; i < 8; i++) {
							if (!strcmp(argv[i],"-i")) {
				   				argv_1 = argv[i + 1];
				   			}
							if (!strcmp(argv[i],"-o")) {
				   				argv_2 = argv[i + 1];
				   			}
			   				if(!strcmp(argv[i], "-r")) {
								argv_3 = argv[i + 1];
							}
						}
						if(!strcmp(argv[1],"-D")) {
							Decode(argv_1, argv_2, argv_3);
						}
						else if(!strcmp(argv[1],"-DI")) {
							Decode_image(argv_1, argv_2, argv_3);
						}
					}
			    	else {
			    		printf("\n\t*** !! Error !! ERROR !! Error !!***\n\n");
						printf("\t*** EXECUTION ==> \n");
						printf("\n\t\t*** ENCODING *** ==> \t./a.out -E -i <input.bmp> -s <secret.txt> -o <output.bmp>\n\n");
						printf("\t\t*** DECODING *** ==> \t./a.out -D -i <output.bmp> -o <output_text.txt>\n\n");
					}
					
			    	break;

		default : 
			printf("\t*** !! Error !! ERROR !! Error !!***\n\n");
			printf("*** EXECUTION ==> \n");
			printf("\n\t\t*** ENCODING *** ==> \t./a.out -E -i <input.bmp> -s <secret.txt> -o <output.bmp>\n\n");
			printf("\t\t*** DECODING *** ==> \t./a.out -D -i <output.bmp> -r <random.txt> -o <output_text.txt>\n\n");
    }
    
    return 0;
}

