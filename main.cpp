#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG

typedef struct{
    int length;
    char* s;
}string;

typedef struct{
    int line_num;
    string* lines;
}text;

int fsize(FILE* f){
    long length;
    fseek(f, 0L, SEEK_END);
    length = ftell(f);
    rewind(f);
    return length;
}

//input:
//s - string to split
//c - split charachter
//output:
//s_new - pointer to array of strings
//return value - number of strings
int split_str_number(string s, char c){
    int counter = 1;
    int sub_str_length = 0;
    char* string_end = s.s + s.length;
    while(s.s < string_end){
        ++sub_str_length;
        if ((s.s == c)&&(sub_str_length))
        {*s.s = '\0';sub_str_length = 0;++counter;}
        ++s.s;
    }
    if (!sub_str_length) --counter;
    return counter;
}

int split(string s, char c, string* s_new){
    int number = split_str_number(s, c);
    s_new = (string) calloc(number, sizeof(string));
    char* string_end = s.s. + s.length;
    while(s.s < string_end) {
    }
}

text read(){
    FILE* input = fopen("in.txt", "r");
    if (input == NULL){
        fprintf(stderr, "Can't open input file.\n");
        getchar();
        exit(1);
    }

    int file_len = fsize(input);
    char* fstring = (char*) calloc(file_len, sizeof(char));
    fread(fstring, sizeof(char), file_len, input);

    fclose(input);

    text file = { .lines = set_lines(fstring, fstring + file_len),
            (char**) calloc(file.line_num, sizeof(char*)) };

    for(int index = 0; index < file.line_num; ++index){
        file.lines[index] = fstring;
        while(*fstring++);
    }

    return file;
}

void write(text t){
    FILE* output = fopen("out.txt", "w");
    if (output == NULL){
        fprintf(stderr, "Can't open output file.\n");
        getchar();
        exit(1);
    }

    for(int i = 0; i < t.line_num; ++i){
        fprintf(output, "%s\n", t.lines[i]);
    }

    fclose(output);

    //free(t.lines[0]);
}

int sort_func( const void *a, const void *b)
{
#ifdef DEBUG
    printf ("Comparing a to b.\na: %s\nb: %s\n", *(const char**)a, *(const char**)b);
#endif
    return( strcmp(*(char **)a,*(char **)b) );
}

int main()
{
    text file = read();
    //char* a[5] = {"aaa","bbb","ccc","ddd","eee"};
    //qsort((void*) a, 5, sizeof(char*), sort_func);
    qsort((void*)file.lines, file.line_num, sizeof(char*), sort_func);
    getchar();
    write(file);
    return 0;
}