#ifndef FFMPEG_PROG_H
#define FFMPEG_PROG_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Copys <from> to the end of <where>
void StrCat(char *where, const char *from) {
	int i = 0, j = 0;
	while (where[i] != 0)
		i++;
	do {
		where[i++] = from[j];
	} while (from[j++] != 0);
}

// Copys <from>'s part from <start> to <finish> to the end of <where> 
void StrCatN(char *where, const char *from, int start, int finish) {
	int i = 0, j = start;
	while (where[i] != 0)
		i++;
	while (from[j] != 0 && j < finish)
		where[i++] = from[j++];
	where[i] = 0;
}

// Copy <from>'s part upto <n>'s character to <where>: returns 0, if n < 1, 0 otherwise
bool StrCatN(char *where, const char *from, int n) {
	if (n <= 0)
		return 0;
	StrCatN(where, from, 0, n);
	return 1;
}

// Сравнивает окончание строки <where> со строкой <what>
// Compares ending of string <where> and string <what>: returns 1 if they're qeual
bool StrCompareEndings(const char *where, const char *what) {
	int len = strlen(where), len1 = strlen(what);
	int from = len - len1;
	for (int i = 0; i < len1; i++) {
		if (where[from + i] != what[i])
			return 0;
	}
	return 1;
}

// Compares <str2> with <str1>[<from>..end]: returns 1 if <str1> contains <str2>, 0 otherwise
bool StrCompare(const char *str1, const char *str2, int from) {
	int len1 = strlen(str1), len2 = strlen(str2);
	if (from < 0 || len1 - from < len2)
		return 0;
	for (int i = 0; i < len2 - 1; i++)
		if (str1[from + i] != str2[i])
			return 0;
	return 1;
}

// Отрезатет от конца строки <from> <hmuch> символов, возвращает 0, если это невозможно
// Cuts <hmuch> characters from <from>: returns 0 if cut is impossible, <from:len> < <hmuch>
bool StrClear(char *from, int hmuch) {
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

// Очищает строку <str>
// Clears string <str>
void StrClear(char *str) {
	StrClear(str, strlen(str));
}

// Copys string <from> to string <to>, returns 0 if at least one of pointers is NULL
bool StrCopy(char *to, char *from) {
	if (to == NULL || from == NULL)
		return 0;
	StrClear(to);
	StrCat(to, from);
	return 1;
}

// Сравнивает сроку s1 со строкой s2
// Compares string <s1> and <s2>: returns 1 if they're equal
bool StrCmp(const char *s1, const char *s2) {
	int len = strlen(s1);
	if (len != (int) strlen(s2))
		return 0;
	for (int i = 0; i < len; i++)
		if (s1[i] != s2[i])
			return 0;
	return 1;
}

// Compares string <str> with <num> strings going after, returns 1 if it's equal to one of them
bool StrCmp(const char *str, int num, ...) {
	char *tmp;
	va_list ap;
	int len = strlen(str);
	va_start (ap, num);
	while (num--) {
		tmp = va_arg(ap, char*);
		if (len != strlen(tmp))
			continue;
		bool flag = 1;
		for (int i = 0; i < len; i++)
			if (str[i] != tmp[i]) {
				flag = 0;
				break;
			}
		if (flag) {
			int i = 0;
			while (va_arg(ap, char*))
				;
			va_end(ap);
			return 1;
		}
	}
	va_end(ap);
	return 0;
}

// Reads string (upto "Enter" symbol or EOF) from <f> to <s>: returns 1 if read was sucsessful
bool read(char *s, FILE *f) {
	int len = 0;
	if (f == NULL)
		return 0;
	while (1) {
		if (fscanf(f, "%c", &s[len]) < 1)
			break;
		if (s[len] == '\n') {
			s[len] = 0;
			break;
		} else
			len++;
	}
	if (len == 0)
		return 0;
	else
		return 1;
}

// Reads one word from file (finish is space, enter or EOF): returns 1 if word was read
char readWord(char *s, FILE *f) {
	int len = 0;
	char last = -1;
	if (f == NULL)
		return -1;
	while (1) {
		if (fscanf(f, "%c", &s[len]) < 1)
			break;
		if (s[len] == ' ' || s[len] == '\n') {
			last = s[len];
			s[len] = 0;
			break;
		} else
			len++;
	}
	if (len == 0)
		return -1;
	else
		return last;
}

// Reads string between quotes, or reads upto \n. Returns: 0 - nothing to read, 1 - reading upto \n (no starting quote),
//                                                         2 - OK, 3 - no closing quote, 4 - EOF after quote
int readQuote(char *s, FILE *f) {
	char c;
	if (f == NULL)
		return 0;
	c = fgetc(f);
	if (c == EOF)
		return 0;
	if (c != '"') {
		ungetc(c, f);
		read(s, f);
		return 1;
	}
	int len = 0;
	while (1) {
		if (fscanf(f, "%c", &s[len]) < 1)
			return 4;
		if (s[len] == '"') {
			s[len] = 0;
			return 2;
		} else if (s[len] == '\n') {
			s[len] = 0;
			return 3;
		} else
			len++;
	}
}

// Возвращает 1, если есть формат файла, и 0, если его нет.
// В первом случае заменяет строку format на данный формат.
bool Analize(char *from, char *format) {
	memset(format, 0, sizeof(format));
	for (int i = strlen(from) - 1; i > 0; i--)
		if (from[i] == '.') {
			StrCat(format, from + i + 1);
			return 1;
		}
	return 0;
}

// "Repairs" string <s> for ffmpeg to work with files with russian names (ANSI code)
void NormalRus(char *s) {
	for (size_t i = 0; i < strlen(s); i++) {
		if ((s[i] >= 'А' && s[i] <= 'Я') || (s[i] >= 'а' && s[i] <= 'п'))
			s[i] -= 192;
		else if (s[i] >= 'р' && s[i] <= 'я')
			s[i] -= 240;
		else if (s[i] == 'ё')
			s[i] -= 57;
		else if (s[i] == 'Ё')
			s[i] -= 72;
	}
}
#endif
