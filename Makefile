all: 
	gcc -Wall Steganography.c -o Stegano
	
encode: all
	./Stegano -E -i input_image.bmp -s secret_text.txt -o output_image.bmp

decode:
	./Stegano -D -i output_image.bmp -r random.txt -o output_text.txt

encode_image: all
	./Stegano -EI -i cover_image.bmp -s input_image.bmp -o stego_image.bmp 
	
decode_image: 
	./Stegano -DI -i stego_image.bmp -r random.txt -o hidden_image.bmp
	
clean: 
	rm output_image.bmp
	rm output_text.txt
	rm random.txt
	rm secret_text.txt
	rm Stegano

clean_image:
	rm stego_image.bmp
	rm random.txt
	rm Stegano
	rm hidden_image.bmp
