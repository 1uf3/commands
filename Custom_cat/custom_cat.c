#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define N 100000

int line;

int main(int argc, char** argv){

	FILE *file;
	int chr;
	int count;
	int n = 0; // FALSE
	int c = 0; // FALSE
	int i = 1; //行数
	int k;

	for(; argc > 1 && argv[1][0] == '-'; argc--,argv++){
		switch(argv[1][1]){
			case 0:
				break;
			case 'n':
				//puts("オプションn");
				n++; //TRUE
				continue;
			case 'c':
				c++; //TRUE
				continue;
		}
		break;
	}

	if(argc < 2){
		fprintf(stderr, "Usage: ./custom_cat <filename>\n");
		return 1;
	}
	
	if(c){
		puts("ROT");
		scanf("%d", &k);
	}
		
	for(count = 1; count < argc; count++){
		if((file = fopen(argv[count], "r")) == NULL) {
			fprintf(stderr, "This file does not exist\n");
		}

		while((chr = getc(file)) != EOF){
			if(n && line == 0){
				printf("%6d\t",i++); //行番号を出力
				line = 1; //一文字ごとに行番号を出力しないように。
			}
		
			if(c){
				printf("%c", cipher(chr, k));
			}else{
				printf("%c", chr); //一文字事出力
			}

			if(chr == '\n'){
				line = 0; //一行出力したら行番号を出力するように。
			}
		}
		fclose(file);
	}
	return 0;
}
