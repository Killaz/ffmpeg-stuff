#ifndef FFMPEG_PROG_H
#define FFMPEG_PROG_H

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

// Copy <from>'s part upto <n>'s character to <where>
void StrCatN(char *where, const char *from, int n) {
	StrCatN(where, from, 0, n);
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

// Compares str1[from..to] with str2
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

// Copys string <from> to string <to>
void StrCopy(char *to, char *from) {
	StrClear(to);
	StrCat(to, from);
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
