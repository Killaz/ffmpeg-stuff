#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printRusComment(a); \
	printRusComment("; Завершение программы. Нажмите любую кнопку для подтверждения.\n"); \
	getch(); \
	return 0;

#define lol() printf("Lol\n")

char format[9] = "";

void printRusComment(char *str) {
	int len = strlen(str);
	char s[800];
//	char *s = (char *) malloc(len * sizeof(char));
	if (s == NULL)
		return (void) -1;
	for (int i = 0; i <= len; i++)
		s[i] = str[i];
	for (int i = 0; i < len; i++) {
		if ((s[i] >= 'А'&& s[i] <= 'Я') || (s[i] >= 'а' && s[i] <= 'п'))
			s[i] += 192;
		else if (s[i] >= 'р' && s[i] <= 'я')
			s[i] += 240;
		else if (s[i] == 'ё')
			s[i] += 57;
		else if (s[i] == 'Ё')
			s[i] += 72;
	}
	printf("%s", s);
//	free(s);
}

void StrCat(char *where, char *from) {
	int i = 0, j = 0;
	while (where[i] != 0)
		i++;
	do {
		where[i++] = from[j++];
	} while (from[j] != 0);
}

bool StrCompare(char *where, char *what, int from) {
	int len = strlen(what), len1 = strlen(where);
	for (int i = 0; i < len; i++) {
		if (from + i >= len1 || where[from + i] != what[i])
			return 0;
	}
	return 1;
}

bool StrClear(char *from, int hmuch) { // Отрезать от конца строки from hmuch символов
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

bool Analize(char *from) {
	memset(format, 0, sizeof(format));
	for (int i = strlen(from) - 1; i > 0; i--)
		if (from[i] == '.') {
			StrCat(format, from + i + 1);
			StrClear(from, strlen(format) + 1);
			return 1;
		}
	return 0;
}

bool read(char *s, FILE *f) {
	int i = 0;
	while (1) {
		fscanf(f, "%c", &s[i]);
		if (s[i] == '\n') {
			s[i] = 0;
			break;
		} else
			i++;
	}
	return StrCompare(s, format, strlen(s) - strlen(format));
}

int main() {
	char c = 0, tmpC, file1[120], file2[120], str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1 -qmax 10", tmp[800];
	bool u = 0; // skip-parameter
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
	memset(file2, 0, sizeof(file2));
//	Start:
	system("cls");
	printRusComment("Пути, если необходимо, нужно вводить с ковычками. Можно \"перетаскивать\" файлы в консоль для получения пути.\n");
	Opening:;
	printRusComment("Введите путь до первого файла (изображение для левого глаза): ");
	read(file1, stdin);
	if (!Analize(file1)) {
		quit("Ошибка: не удается распознать формат файла");
	}
	printRusComment("Введите путь до второго файла (изображение для правого глаза): ");
	read(file2, stdin);
	StrClear(file2, strlen(format) + 1);
	if (u)
		goto FFtry;
	sprintf(tmp, "Файлы:\n1: %s.%s\n2: %s.%s\n(N - заново написать пути, Esc - выход, F - указать расположение ffmpeg'а (если не C:\\ffmpeg\\bin)"
	       ", B - Задать строку параметров (если не -q:v 1), K - F+B:\n", file1, format, file2, format);
	printRusComment(tmp);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmpC = c;
	if (c == 'N' || c == 'n' || c == 'т' || c == 'Т')
		goto Opening;		
	if (c == 'B' || c == 'b' || c == 'и' || c == 'И')
		goto Params;
	if (c != 'F' && c != 'f' && c != 'а' && c != 'А' && c != 'K' && c != 'k' && c != 'л' && c != 'Л')
		goto FFtry;
//	Prepare:;
	Pathing:;
	sprintf(tmp, "ffmpeg.exe можно найти по адресу \"%s\"?\n", ffpath);
	printRusComment(tmp);
	c = getch();
    printf("%c\n", c);
    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
    	printRusComment("Напишите путь до ffmpeg.exe: ");
    	read(ffpath, stdin);
    	goto Pathing;
	}	
	if (tmpC != 'K' && tmpC != 'k' && tmpC != 'л' && tmpC != 'Л')
		goto FFtry;
//	Last_prepare:;
	Params:;
	sprintf(tmp, "Сейчас параметры выглядят так: %s\nИспользовать эту строку? В случае отказа (не Y или Enter) "
	             "вам будет предложено ввести свою строку параметров; Esc - выход\n", param);
	printRusComment(tmp);
	c = getch();
	if (c == 27) {
		quit("Esc");
	} else
		printf("%c\n", c);
	if (c == 'n' || c == 'N' || c == 'т' || c == 'Т') {
		printRusComment("Введите новую строку параметров: ");
		read(param, stdin);
		goto Params;
	}
//	Work:;
	FFtry:;
	memset(str, 0, sizeof(str));
	sprintf(str, "%s -i %s.%s -i %s.%s -filter_complex \"[0:v]pad=iw*2:ih[bg]; [bg][1:v]overlay=w,colorlevels="
	             "rimin=0.082:gimin=0.082:bimin=0.082:rimax=0.9:gimax=0.9:bimax=0.9\" %s %s-all.%s", ffpath, file2, format, file1, format, param, file1, format);
	printf("%s\n", str);
	if (system(str)) {
		quit("Ошибка при работе ffmpeg'а");
	}
	if (u)
		goto Opening;
	printRusComment("\nНажмите U для продолжения создания стереопар с такими же параметрами, Любую другую клавишу для выхода\n");
	t = time(0);
	while (time(0) - t < 10) {
		if (kbhit()) {
			c = getch();
			if (c == 'u' || c == 'U' || c == 'г' || c == 'Г') {
				u = 1;
				goto Opening;
			}
			return 0;
		}
	}
	return 0;
}
