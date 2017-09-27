#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG

typedef struct{
    int length;
    char* s;
}string;

typedef struct{
    int line_num;
    string* lines;
}text;

/** returns the size of raw file
 *
 * @param[in] f - input file
 * @return file size
 */
int fsize(FILE* f){
    assert(f != NULL);

    long length;
    fseek(f, 0L, SEEK_END);
    length = ftell(f);
    rewind(f);
    return length;
}

/** count how many split chars do we have
 *
 * @param[in] string string to split
 * @param[in] c split character
 * @return number of strings
 */
int sub_str_number(string s, char c){
    assert(s.s != nullptr);
    assert(s.length > 0);

    int counter = 1;
    int sub_str_length = 0;
    while(*s.s != '\0'){
        ++sub_str_length;
        if (*s.s == c){
            *s.s = '\0';
            if (sub_str_length){
                sub_str_length = 0;
                ++counter;
            }
        }
        ++s.s;
        //printf("%c %d %d\n", *s.s, counter, sub_str_length);
    }
    if (!sub_str_length) --counter;
    return counter;
}

/** splits input string by character
 *
 * @param[in] s - input string
 * @param[in] c - split character
 * @param[out] t - array of strings
 * @return
 */
text split(string s, char c){
    assert(s.s != nullptr);
    assert(s.length > 0);

    text t = {0, nullptr};

    t.line_num = sub_str_number(s, c);
    //printf("%d", t.line_num);
    t.lines = (string*) calloc(t.line_num, sizeof(string));
    if (t.lines == nullptr) {
        fprintf(stderr, "Bad alloc in function split\n");
        getchar();
        exit(1);
    }

    int index = 0;
    while(index < t.line_num){
        t.lines[index].s = s.s;
        while(*s.s++ != '\0');
        t.lines[index].length = s.s - t.lines[index].s;
        while(*s.s == '\0') ++s.s;
        //printf("%s\n", t.lines[index].s);
        //printf("%d %d\n", t.lines[index].length, index);
        ++index;
    }

    return t;
}


/** reads file into string
 *
 * @param[in] name - file name
 * @return string that contains text
 */
string read(char* name){
    assert(name != nullptr);

    FILE* input = fopen(name, "r");
    if (input == NULL){
        fprintf(stderr, "Can't open input file.\n");
        getchar();
        exit(1);
    }

    int file_len = fsize(input) + 10;

    string text = {};
    text.s = (char*) calloc(file_len, sizeof(char));
    if (text.s == nullptr) {
        fprintf(stderr, "Bad alloc in function read\n");
        getchar();
        exit(1);
    }
    text.length = fread(text.s, sizeof(char), file_len, input);
    text.s[text.length] = '\0';

    fclose(input);
    return text;
}

/** writes sorted text into file
 *
 * @param[in] name - file name
 * @param[in] t - sorted array of strings
 */
void write(char* name, text t){
    assert(name != nullptr);
    assert(t.line_num > 0);
    assert(t.lines != nullptr);

    FILE* output = fopen(name, "w");
    if (output == NULL){
        fprintf(stderr, "Can't open output file.\n");
        getchar();
        exit(1);
    }

    for(int i = 0; i < t.line_num; ++i){
        fprintf(output, "%s\n", t.lines[i].s);
    }

    fclose(output);
}

/** reverse strcmp function
 *
 */
int strrcmp(const string* left, const string* right){
    const char* left_it = left->s + left->length - 3;
    const char* right_it = right->s + right->length - 3;
    while((left_it >= left->s)&&(right_it >= right->s)) {
        if (*left_it != *right_it) break;
        --left_it;
        --right_it;
    }
    return *left_it - *right_it;
}

/** sort function for qsort.
 *
 */
int sort_func( const void *left_raw, const void *right_raw)
{
    string* left = (string*) left_raw;
    string* right = (string*) right_raw;
#ifdef DEBUG
    printf ("Comparing left to right.\nleft: %s\nright: %s\n", left->s, right->s);
#endif
    return strrcmp(left, right);
}

int main()
{
    string fstring = read((char*)"in.txt");
    text file = split(fstring, '\n');
    qsort((void*)file.lines, file.line_num, sizeof(string), sort_func);
    getchar();
    write((char*)"out.txt", file);
    return 0;
}