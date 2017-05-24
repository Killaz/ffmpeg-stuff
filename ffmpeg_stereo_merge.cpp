// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
// For getch()
#include <string.h>
// For strlen()
#include <stdlib.h>
// For system()
#include <time.h>
// For time()
#include <ctype.h>

#include "ffmpeg_prog.h"

#define elif else if
#define quit(a) \
	printf("%s; Завершение программы.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("[-ffpath <string>] or [-ffpath <string with spaces>\"] - set path to ffmpeg's .exe file\n" \
	       "[-formatOut <string>] - force output format" \
	       "[-endless] - programm won't close after one sterepo-picture\n" \
	       "[-noUser] - menu won't be displayed, no questions will be asked\n" \
      );

#define lol() printf("Lol\n")

char format_in[15] = "", format_out[15] = "";

int main(int argc, char *argv[]) {
	char c = 0, tmp, file1[350] = "", file2[350] = "", str[2000], ffpath[300] = "C:\\ffmpeg\\bin\\ffmpeg.exe", outFilter[400] = "copy", outParams[400] = "-q:v 1", outDir[300] = "", postfix[30] = "-all";
	bool u = 0, user = 1, right_first = 1, forced_fout = 0;     // skip-parameter
	int i;
	char files = 0; // 1, если первый файл введен как параметр; 2, если второй
	time_t t;
//	Init:;
//	Start:
	system("cls");
// Разбор строки параметров
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "-endless") || StrCmp(argv[i], "/endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i], "-ffpath") || StrCmp(argv[i], "/ffpath"))
			StrCat(ffpath, argv[++i]);
		elif (StrCmp(argv[i], "-format_out") || StrCmp(argv[i], "/format_out"))
			StrCat(format_out, argv[++i]);
		elif (StrCmp(argv[i], "-r_first") || StrCmp(argv[i], "/r_first"))
			right_first = 0;
		elif (Analize(argv[i], format_in) || argv[i][0] == '"') {
			StrClear(argv[i], strlen(format_in) + 1);
			if (files == 0)
				StrCat(file1, argv[i]), files = 1;
			elif (files == 1)
				StrCat(file2, argv[i]), files = 2;
			else
				printf("Unknown parameter: %s (input files already: %s and %s)\n", argv[i], file1, file2);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	if (StrCmp(format_out, ""))
		StrCat(format_out, format_in);
	Params:;
	while(user) {
		printf("Сейчас строка вызова выглядит так:\n%s -i file1.format1 -i file2.format1 -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w[preout]; [preout] %s\" %s %sfile1%s.format2\n"
		       "Введите:\n\"path\" / \"p\", чтобы изменить путь до ffmpeg.exe\n"
		       "\"outFilter\" / \"outFilters\" / \"of\", чтобы изменить последний фильтр выходного файла\n"
		       "\"outParams\" / \"op\", чтобы изменить параметры выходного файла\n"
		       "\"outDir\" / \"od\", чтобы изменить папку вывода\n"
		       "\"postfix\" / \"pf\", чтобы изменить постфикс после имени первого файла в файле вывода\n"
		       "\"exit\" / \"stop\", чтобы выйти из программы\n"
		       "\"formatOut\" / \"fo\" / \"fout\" - принудительное изенение выходного формата\n"
		       "(пустая строка) / \"start\" / \"go\", чтобы перейти к вводу файлов\n",
		       ffpath, outFilter, outParams, outDir, postfix);
		c = readWord(str, stdin);
		for (size_t i = 0; i < strlen(str); i++)
			str[i] = tolower(str[i]);
		if (StrCmp(str, "") || StrCmp(str, "go") || StrCmp(str, "start"))
			break;
		elif (StrCmp(str, "exit") || StrCmp(str, "stop")) {
			quit("Exiting");
		} elif (StrCmp(str, "path") || StrCmp(str, "p")) {
			if (c == 10)
				printf("Введите путь до ffmpeg.exe (включая сам \"ffmpeg.exe\"): ");
			read(ffpath, stdin);
		} elif (StrCmp(str, "outfilter") || StrCmp(str, "of") || StrCmp(str, "outfilters")) {
			if (c == 10)
				printf("Введите фильтр(ы) выходного файла (для отсутствия фильтров - \"copy\"): ");
			read(outFilter, stdin);
		} elif (StrCmp(str, "outparams") || StrCmp(str, "op")) {
			if (c == 10)
				printf("Введите параметры выходного файла: ");
			read(outParams, stdin);
		} elif (StrCmp(str, "outdir") || StrCmp(str, "od")/* || StrCmp(str, "od_nr") || StrCmp(str, "outdir_norepair")*/) {
			if (c == 10)
				printf("Введите путь до папки вывода: ");
			read(outDir, stdin);
			if (outDir[strlen(outDir) - 1] != '\\') {
				outDir[strlen(outDir) + 1] = 0;
				outDir[strlen(outDir)] = '\\';
			}
		} elif (StrCmp(str, "formatout") || StrCmp(str, "fo") || StrCmp(str, "fout")) {
			if (c == 10)
				printf("Введите формат выходных файлов (без точки; пустая строка - не менять формат входных файлов): ");
			read(format_out, stdin);
			if (!StrCmp(format_out, ""))
				forced_fout = 1;
			else
				forced_fout = 0, StrCat(format_out, format_in);
		} elif (StrCmp(str, "postfix") || StrCmp(str, "pf")) {
			if (c == 10)
				printf("Введите строку, которая будет приписываться к имени выходного файла (пустая строка - имя совпадает с изображением для левого глаза): ");
			read(postfix, stdin);
		} else
			printf("Can't recognize command: %s\n", str);
		printf("\n");
	}

	Opening:;
	if (files == 2) {
		files = 0;
		goto FFtry;
	}
	printf("Пути, если необходимо, нужно вводить с кавычками. Можно \"перетаскивать\" файлы в консоль для получения пути\n");
	if (files == 1) {
		files = 0;
		goto SecondFile;
	}

	printf("Введите путь до изображения для левого глаза (или команду): ");
	read(file1, stdin);

	//search of commands:
	if (StrCmp(file1, "endless")) {
		u = !u, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (StrCmp(file1, "exit")) {
		quit("Exiting");
	} elif (StrCmp(file1, "help")) {
		printf("\"exit\" for exit, \"endless\" turns on/off endless mode, \"params\" returns you to parameters change menu\n");
		goto Opening;
	} elif (StrCmp(file1, "params")) {
		goto Params;
	} elif (!Analize(file1, format_in)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	} else
		StrClear(file1, strlen(format_in) + 1);

	SecondFile:;
	printf("Введите путь до изображения для правого глаза: ");
	read(file2, stdin);
	StrClear(file2, strlen(format_in) + 1);
	if (!forced_fout)
		StrCat(format_out, format_in);
/*	printf("Файлы:\n1: %s.%s\n2: %s.%s\n(N - заново написать пути, Esc - выход, Entrer / y - начать совмещение, \n",
	       right_first ? file1 : file2, format_in, right_first ? file2 : file1, format_in);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmp = c;
	if (c == 'N' || c == 'n')
		goto Opening;
	if (c == 'B' || c == 'b')
		goto Params;
	if (c != 'F' && c != 'f' && c != 'K' && c != 'k')
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
//	Work:;*/
	FFtry:;
	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format_in[strlen(format_in) + 1] = 0, format_in[strlen(format_in)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
		files = -1;                      // flag
	}
	if (file2[0] != '"') {
		if (files != -1) {
			printf("Пути должны быть либо оба в кавычках, либо оба без них. Откат.\n");
			goto Opening;
		}
		file2[strlen(file2) + 1] = 0;
		for (i = strlen(file2); i > 0; i--)
			file2[i] = file2[i - 1];
		file2[0] = '"';
	}
	files = 0;

	printf("file1 = %s\n", file1);
	printf("file2 = %s\n", file2);
	                                                                                                                //colorlevels=rimin=0.082:gimin=0.082:bimin=0.082:rimax=0.9:gimax=0.9:bimax=0.9
	sprintf(str, "%s -i %s.%s -i %s.%s -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w[preout]; [preout] %s\" %s %s%s%s.%s",
	              ffpath, right_first ? file1 : file2, format_in, right_first ? file2 : file1, format_in, outFilter, outParams, outDir, right_first ? file1 : file2, postfix, format_out);
	printf("%s\n", str);
	NormalRus(str);
	if (system(str)) {
		printf("Ошибка при работе ffmpeg'а.");
		if (u) {
			printf(" Откат.\n");
			goto Opening;
		} else
			printf("\n");
	}
	if (u)
		goto Opening;
/*	printf("\nНажмите U для продолжения создания стереопар с такими же параметрами, Любую другую клавишу для выхода\n");
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
	}*/
	return 0;
}
