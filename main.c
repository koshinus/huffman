#include"huffman_tree.h"

char * to_binary(char code, char *s, unsigned long code_size)
{
    for(int i = 0; i < code_size; i++)
        s[7-i] = (char)((code >> i) & 1) + 48;
	return s;
}

int main()
{
    const char *filename;
    FILE *f;
    printf("Enter file name:\n");
    scanf("%ms", &filename);
    if ( (f = fopen(filename, "r")) == NULL )
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }

	//Getting frequency table from binary file
	unsigned long long buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(unsigned long long)*BUFFER_SIZE);
    int c = EOF;
    while ((c = fgetc(f)) != EOF) buffer[(unsigned char)c]++;
	
	//https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_tree(het, buffer, BUFFER_SIZE);			// 1
    qsort(het->tree, het->nodes_number, sizeof(huffman_encode_node), cmp);
	//for(int i = 0; i < het->nodes_number; i++)
        //printf("%c: %ld\n", het->tree[i].symbol, het->tree[i].frequency);

    while(2*BUFFER_SIZE - het->nodes_number > 1)							// 2
    {
        short left = 0, right = 0;  					            	
        left  = get_minimum(het);											//2.1
        het->tree[ left].free_and_is_leaf |= 2;								//2.1
        right = get_minimum(het);											//2.1
        het->tree[right].free_and_is_leaf |= 2;								//2.1
        huffman_encode_node node
                = make_node(het->tree[ left].frequency+						// 2.2
                            het->tree[right].frequency,						// 2.2
                            0, left, right, 0, 0, '-');						// 2.2
        het->tree[het->nodes_number] = node;					    		// 2.3
        het->nodes_number++;
    }
	
	//Printing nodes frequency
    for(int i = 0; i < het->nodes_number; i++)
        printf("%d: {%d, %d}->%ld\n", i, het->tree[i].left, het->tree[i].right, het->tree[i].frequency);
	
	//Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-(short)1);
	
	//Printing huffman codes in binary view	    
	for(int i = 0; i < het->nodes_number/2 + 1; i++)
	{
		char s[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '\0'};
        printf("%c: %s\n", (unsigned char)i, to_binary(het->tree[i].code, s, het->tree[i].code_size));
	}
    free(het);
    fclose(f);
	
    return 0;
}


	//unsigned long long *buf = (unsigned long long *)malloc(sizeof(unsigned long long)*5);
    //buf[0] = 12; buf[1] = 7; buf[2] = 6; buf[3] = 6; buf[4] = 5;

    /*frequency
    unsigned long check_res = 0;
    for(int i = 0; i < 256; i++)
    {
        printf("%c - %d\n", (char)i, (int)buffer[i]);
        check_res += buffer[i];
    }
    printf("%ld - number of reading bytes\n", check_res);
	*/
