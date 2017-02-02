#include <stdio.h>

char * print_binary(char code, char *s)
{
    for(int i = 0; i < 8; i++)
        s[7-i] = (char)((code >> i) & 1) + 48;
	return s;
}

int main(void) 
{
	char c = (unsigned char)3;
	char s[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
	/*   
	char s[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    for(int i = 0; i < 8; i++)
	{
		//printf("%c", (char)(((int)c >> i) & 1) + 48);
		s[7-i] = (char)((c >> i) & 1) + 48;
	}
	*/
	//for(int i = 0; i < 9; i++) printf("%c", s[i]);
	printf("%s\n", print_binary(c, s));
   	return 0;
}

