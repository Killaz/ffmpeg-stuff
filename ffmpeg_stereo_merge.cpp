// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ffmpeg_prog.h"

#define elif else if
#define quit(a) \
	printf("%s; Завершение программы.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("[endless] - programm won't close after two pictures\n" \
           "[exit] - close thr programm\n" \
           "[help] - print this help\n" \
      );

#define lol() printf("Lol\n")

char format[9] = "";

int main(int argc, char *argv[]) {
	char c = 0, tmp, file1[120], file2[120], str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1";
	bool u = 0;     // skip-parameter
	int i;
	char first = 0; // 1, если первый файл введен как параметр; 2, если второй
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
	memset(file2, 0, sizeof(file2));
//	Start:
	system("cls");
// Разбор строки параметров
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i], "ffpath"))
			if (i < argc - 1)
				StrCat(ffpath, argv[++i]);
			else
				printf("ffpath-parameter is used without path. Ignored.\n");
		elif (Analize(argv[i], format) || argv[i][0] == '"') {
			StrClear(argv[i], strlen(format) + 1);
			if (first == 0)
				StrCat(file1, argv[i]), first = 1;
			elif (first == 1)
				StrCat(file2, argv[i]), first = 2;
			else
				printf("Unknown parameter: %s\n", argv[i]);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	printf("Пути, если необходимо, нужно вводить с кавычками. Можно \"перетаскивать\" файлы в консоль для получения пути\n");
	Opening:;
	
	if (first == 2) {
		first = 0;
		goto FFtry;
	}
	if (first == 1) {
		first = 0;
		goto SecondFile;
	}

	printf("Введите путь до изображения для левого глаза (или команду): ");
	read(file1, stdin);

	//search of commands:
	if (StrCmp(file1, "endless")) {
		u ^= 1, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (StrCmp(file1, "exit")) {
		return 0;
	} elif (StrCmp(file1, "help")) {
		printHelp();
		goto Opening;
	} elif (!Analize(file1, format)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	} else
		StrClear(file1, strlen(format) + 1);

	SecondFile:;
	printf("Введите путь до изображения для правого глаза: ");
	read(file2, stdin);
	StrClear(file2, strlen(format) + 1);
	if (u)
		goto FFtry;
	printf("Файлы:\n1: %s.%s\n2: %s.%s\n(N - заново написать пути, Esc - выход, F - указать расположение ffmpeg'а (если не C:\\ffmpeg\\bin), \n"
	       "B - Задать строку параметров (если не -q:v 1), K - F+B:\n", file1, format, file2, format);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmp = c;
	if (c == 'N' || c == 'n' || c == 'т' || c == 'Т')
		goto Opening;
	if (c == 'B' || c == 'b' || c == 'и' || c == 'И')
		goto Params;
	if (c != 'F' && c != 'f' && c != 'а' && c != 'А' && c != 'K' && c != 'k' && c != 'л' && c != 'Л')
		goto FFtry;
//	Prepare:;
	Pathing:;
	printf("ffmpeg.exe можно найти по адресу \"%s\"?\n", ffpath);
	c = getch();
    printf("%c\n", c);
    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
    	printf("Напишите путь до ffmpeg.exe: ");
    	read(ffpath, stdin);
    	goto Pathing;
	}	
	if (tmp != 'K' && tmp != 'k' && tmp != 'л' && tmp != 'Л')
		goto FFtry;
//	Last_prepare:;
	Params:;
	printf("Сейчас параметры выглядят так: %s\nИспользовать эту строку? В случае отказа (не Y или Enter) "
	       "вам будет предложено ввести свою строку параметров; Esc - выход\n", param);
	c = getch();
	if (c == 27) {
		quit("Esc");
	} else
		printf("%c\n", c);
	if (c == 'n' || c == 'N' || c == 'т' || c == 'Т') {
		printf("Введите новую строку параметров: ");
		read(param, stdin);
		goto Params;
	}
//	Work:;
	FFtry:;
	memset(str, 0, sizeof(str));
	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format[strlen(format) + 1] = 0,format[strlen(format)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
		first = -1;                      // flag
	}
	if (file2[0] != '"') {
		if (first != -1) {
			printf("Пути должны быть либо оба в кавычках, либо оба без них. Откат.\n");
			goto Opening;
		}
		file2[strlen(file2) + 1] = 0;
		for (i = strlen(file2); i > 0; i--)
			file2[i] = file2[i - 1];
		file2[0] = '"';
	}
	first = 0;

	printf("file1 = %s\n", file1);
	printf("file2 = %s\n", file2);
	sprintf(str, "%s -i %s.%s -i %s.%s -filter_complex \"[0:v]pad=iw*2:ih[bg]; [bg][1:v]overlay=w,colorlevels=rimin=0.082:"
	             "gimin=0.082:bimin=0.082:rimax=0.9:gimax=0.9:bimax=0.9\" %s %s-all.%s", ffpath, file2, format, file1, format, param, file1, format);
	printf("%s\n", str);
	if (system(str)) {
		printf("Ошибка при работе ffmpeg'а.");
		if (u) {
			printf(" Откат.");
			goto Opening;
		} else
			printf("\n");
	}
	if (u)
		goto Opening;
	printf("\nНажмите U для продолжения создания стереопар с такими же параметрами, Любую другую клавишу для выхода\n");
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
