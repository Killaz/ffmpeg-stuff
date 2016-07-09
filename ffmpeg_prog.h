#ifndef FFMPEG_PROG_H
#define FFMPEG_PROG_H

void StrCat(char *where, const char *from) {
	int i = 0, j = 0;
	while (where[i] != 0)
		i++;
	do {
		where[i++] = from[j++];
	} while (from[j] != 0);
}

void StrCatN(char *where, char *from, int start, int finish) {
	int i = 0, j = start;
	while (where[i] != 0)
		i++;
	while (from[j] != 0 && j < finish)
		where[i++] = from[j++];
	where[i] = 0;
}

// Сравнивает сроку s1 со строкой s2
bool StrCmp(const char *s1, const char *s2) {
	int len = strlen(s1);
	if (len != (int) strlen(s2))
		return 0;
	for (int i = 0; i < len; i++)
		if (s1[i] != s2[i])
			return 0;
	return 1;
}

// Сравнивает строку where со строкой from,начиная с from-того символа
bool StrCompare(const char *where, const char *what, int from) {
	int len = strlen(what), len1 = strlen(where);
	for (int i = 0; i < len; i++) {
		if (from + i >= len1 || where[from + i] != what[i])
			return 0;
	}
	return 1;
}

// Отрезатет от конца строки from hmuch символов
bool StrClear(char *from, int hmuch) {
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

bool read(char *s, FILE *f) {
	int i = 0;
	if (f == NULL)
		return 0;
	else
	while (1) {
		if (fscanf(f, "%c", &s[i]) < 1)
			break;
		if (s[i] == '\n') {
			s[i] = 0;
			break;
		} else
			i++;
	}
	if (i == 0)
		return 0;
	else
		return 1;
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
