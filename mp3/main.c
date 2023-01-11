#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _node {
	char ch;
	int frequency;
	struct _node* left;
	struct _node* right;
} node;



typedef struct _heap{
	node* heap[128];
	int size;
} heap;

node* create_node(int);
node* create_huffman_tree(heap*);
void insert_node(heap*,node*);
node* delete_node(heap*);
void buildhuffmancode(node *, int , int );



FILE *fp;
FILE *comp_file;
FILE *decomp_file;
char **ascii_char;
int input_freq[128]; 
int input_cnt = 0;

int char_wb = 0;
int char_wb_len = 0;

int main(int argc, char **argv){
	char file_char;
	
	
	
	//Error message 출력
	
	if (argc != 3){
        printf("ERROR : argument error\n");
		exit(0);
	}


	else if (strcmp(argv[1], "-c") == 0){


		fp = fopen(argv[2], "r+");
		//파일없을때
		if (fp == NULL){
			printf("ERROR : file not found \n");
			exit(0);
		}

		heap* c_heap = (heap*)malloc(sizeof(heap));
		c_heap->size = 0;

		for(input_cnt=0;fread(&file_char, sizeof(char), 1, fp); input_cnt++){
			input_freq[file_char]+= 1;
			input_cnt--;
			if (input_freq[file_char] == 1) input_cnt++;

		}

	
		
		//노드생성 및 초기화
		int ch = 0;
		while(ch < 128){
			
			// node *item = (node*)malloc(sizeof(node));
			// if (!input_freq[ch]) return NULL;

			
			// item->ch = ch;
			// item->frequency = input_freq[ch];
			// item->left = NULL;
			// item->right = NULL;
			node* item = create_node(ch);

			if (item){
			c_heap->size++;
			
			int i = c_heap->size;
			for(i = c_heap->size;(i != 1) && (item->frequency < c_heap->heap[i/2]->frequency);i /= 2){
				c_heap->heap[i] = c_heap->heap[i/2];
			}

			c_heap->heap[i] = item; 
			}
			ch++;
		}


		ascii_char = (char**)malloc(sizeof(char*)*128);
		for (int i = 0; i < 128; i++) ascii_char[i] = NULL;
		
		node* huffRoot = create_huffman_tree(c_heap);
		
		char input_file[80];
		strcat(input_file, argv[2]);
		strncat(input_file, ".zz",3);
		comp_file = fopen(input_file, "w+");
		// char de_input_file[80];
		// strcat(de_input_file, argv[2]);
		// strcat(de_input_file, ".yy");
		// decomp_file = fopen(de_input_file, "w");
		
		//파일 없을때
		if (comp_file == NULL){
			printf("ERROR : file not found\n");
			exit(0);
		}

		
		fwrite(&input_cnt, sizeof(int), 1, comp_file);
		
		buildhuffmancode(huffRoot, 0, 0);

		compression(argv[2]);

		fclose(comp_file);
		
		
		fclose(fp);
	}

	else if (strcmp(argv[1], "-d") == 0){
		
	
		fp = fopen(argv[2], "rb");
		
		
		if (fp == NULL){
			printf("ERROR : file not found\n");
			exit(0);
		}

		else{
		
		char de_input_file[80];
		strcat(de_input_file, argv[2]);
		strncat(de_input_file, ".yy",3);
		decomp_file = fopen(de_input_file, "w");
		
		
		if (decomp_file == NULL){
			printf("ERROR : file not found\n");
			exit(0);
		}

		decompression(argv[2]);

		fclose(decomp_file);
		}
		fclose(fp);
	}


	else printf("ERROR : wrong format\n");
	
	
	return 0;
}




void compression(char *textFile){
	char ch;
	int bit_num = 0;
	FILE* fp = fopen(textFile, "r");
	
	
	if (fp == NULL){
        printf("ERROR : file not found\n");
		exit(0);
	}

	else{
	

	int i = 0;
	while(i<128){
		if (input_freq[i]) bit_num += input_freq[i] * strlen(ascii_char[i]);
		i++;
	}


	fwrite(&bit_num, sizeof(int), 1, comp_file);
	while ((ch = fgetc(fp)) != EOF) {
		
		int check_wb = 0;
		int char_len = strlen(ascii_char[ch]);
		int i = char_len - 1;

		while(i >= 0){
			check_wb += (int)pow(2, i) * (ascii_char[ch][char_len - 1 - i] - 48);
			i--;
		}

		
		char_wb = (char_wb << char_len) + check_wb;
		

		for(char_wb_len += char_len; char_wb_len >= 8;char_wb_len -= 8){
			char temp = char_wb >> (char_wb_len - 8);
			fwrite(&temp, sizeof(char), 1, comp_file);
			char_wb = char_wb % (int)pow(2, (char_wb_len - 8));
		}

	}

	if (char_wb_len > 0){
		char temp = char_wb << (8 - char_wb_len);
		fwrite(&temp, sizeof(char), 1, comp_file);
		char_wb = 0;
		char_wb_len = 0;
	}
	}
}




void decompression(char *textFile){
	
	char decomp_char[8];
	int ascii_num = 0;
	int bit_count;
	unsigned char temp;

	fread(&ascii_num, sizeof(int), 1, fp);
	
	node* savehuffmanroot = (node*)malloc(sizeof(node));
	node* decomp_node = savehuffmanroot;
	node* huffman_tmp = savehuffmanroot;

	savehuffmanroot->left = NULL;
	savehuffmanroot->right = NULL;

	int i = 0;
	while(i < ascii_num){
		char asciicode_len[2];
		char wordcode[80];

		fread(&asciicode_len, 2, 1, fp);
		
		int temp_len = asciicode_len[1];
		
		fread(&wordcode, sizeof(char), temp_len, fp);
		
		wordcode[temp_len] = 0;

		decomp_node = savehuffmanroot;
		
		int j = 0;
		while(j < temp_len){
			
			if (wordcode[j] == '0'){ //왼
				decomptree(decomp_node);
				decomp_node = decomp_node->left;
			}

			else if (wordcode[j] == '1'){//오
				decomptree(decomp_node);
				decomp_node = decomp_node->right;
			}
			j++;
		}

		decomp_node->ch = asciicode_len[0];
		i++;
	}
	

	
	fread(&bit_count, sizeof(int), 1, fp);
	fread(&temp, sizeof(char), 1, fp);
	
	
	i =0;
	while(i<8){
			decomp_char[i] = (temp >> (7 - i)) & 1;
			i++;
		}

	int flag = 0;
	for(;;){

		for (int i = 0; i < 8; i++){
			if (!decomp_char[i]) 
				huffman_tmp = huffman_tmp->left;
			if (decomp_char[i]) 
				huffman_tmp = huffman_tmp->right;

			bit_count--;

			if (!huffman_tmp->left) {
				fprintf(decomp_file, "%c", huffman_tmp->ch);
				huffman_tmp = savehuffmanroot;
			}

			if (!bit_count) {
				flag = 1;
				break;
			}
		}

		if (flag) 
			break;

		else{
			fread(&temp, sizeof(char), 1, fp);

			i =0;
			while(i<8){
				decomp_char[i] = (temp >> (7 - i)) & 1;
				i++;
			}

		}

	}
	
}


void decomptree(node* decomp_node)
{
	if (decomp_node->left == NULL){
					decomp_node->left = (node*)malloc(sizeof(node));
					
					decomp_node->left->left = NULL;
					decomp_node->left->right = NULL;
				}
				if (decomp_node->right == NULL){
					decomp_node->right = (node*)malloc(sizeof(node));
					
					decomp_node->right->left = NULL;
					decomp_node->right->right = NULL;
				}
}


void insert_node(heap *h, node *item){

  h->size++;
			
  int i = h->size;
  for(i = h->size;(i != 1) && (item->frequency < h->heap[i/2]->frequency);i /= 2){
  h->heap[i] = h->heap[i/2];
	}

  h->heap[i] = item; 
}

node* delete_node(heap *h){
  int parent, child;
  node* item;
  node* temp;

  if (h->size == 0) return NULL;

  item = h->heap[1];
  temp = h->heap[(h->size)--]; 
  parent = 1;

  for(child=2;child <= h->size;child *= 2){
	
   
    if( temp->frequency <= h->heap[child]->frequency ){
      break;
    }
	if( (child < h->size) && ((h->heap[child]->frequency) > h->heap[child+1]->frequency) ){
		child++;
		}
    h->heap[parent] = h->heap[child];
    
    parent = child;
  }

  h->heap[parent] = temp;
  
  return item;
}

void buildhuffmancode(node *item, int wordcode, int height)
{ 
	if (!item->right) {
		
		char char_len = (char)height + 1;
		fwrite(&item->ch, sizeof(char), 1, comp_file);
		fwrite(&char_len, sizeof(char), 1, comp_file);
		ascii_char[item->ch] = (char*)malloc(sizeof(char)*(height+2));
		
		int i = height;
		while(i>=0){
			ascii_char[item->ch][height - i] = (char)((wordcode >> i) & 1) + 48;
			
			fwrite(&ascii_char[item->ch][height - i], sizeof(char), 1, comp_file);
			i--;
		}


		ascii_char[item->ch][height+1] = '\0';
		return;
	}
	int height_tmp = height +1;
	buildhuffmancode(item->left, wordcode << 1, height_tmp); 
	buildhuffmancode(item->right, (wordcode << 1) + 1, height_tmp);
}

node* create_huffman_tree(heap *h){

	
	while (h->size > 1){
		node* item1 = delete_node(h);
		node* item2 = delete_node(h);

		node* merged_item = (node*)malloc(sizeof(node));
		
		merged_item->frequency = item1->frequency + item2->frequency;
		merged_item->left = item1;
		merged_item->right = item2;

		
		h->size++;
					
		int i = h->size;
		for(i = h->size;(i != 1) && (merged_item->frequency < h->heap[i/2]->frequency);i /= 2){
		h->heap[i] = h->heap[i/2];
			}

		h->heap[i] = merged_item;
	}

	return delete_node(h);
}


node* create_node(int ch){

	if (!input_freq[ch]) return NULL;

			node *item = (node*)malloc(sizeof(node));
			item->ch = ch;
			item->frequency = input_freq[ch];
			item->left = NULL;
			item->right = NULL;

	return item;
}
