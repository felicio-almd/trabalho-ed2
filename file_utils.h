#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#define MAX_TEXT_SIZE 1000000
#define MAX_KEYWORDS 100
#define MAX_KEYWORD_LENGTH 50

extern char keywords[MAX_KEYWORDS][MAX_KEYWORD_LENGTH];
extern int keyword_count; // Declare como extern

char *read_file(const char *filename);
void load_keywords(const char *filename);

#endif // FILE_UTILS_H