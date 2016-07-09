// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ffmpeg_programms/ffmpeg_prog.h"

#define elif else if
#define quit(a) \
	printf("%s; Завершение программы.\n", a); \
	if (off) { \
		system("shutdown /s /t 120"); \
		printf("Компьютер будет выключен через 120 секунд.\n"); \
	} \
	system("pause"); \
	return 0;
//#define file "%userprofile%/Documents/Tree.txth"

#define printHelp() \
	printf("[-param <string>] or [-param \"<string with spaces>\"] - give parameters to ffmpeg\n" \
	       "[-formats <format_from> <format_to>] - set coding from format_from into format_to\n" \
	       "[-ffpath <string>] or [-ffpath <string with spaces>\"] - set ffmpeg.exe's directory\n" \
	       "[-off] - turns off computer after finishing\n" \
	       "[-u] - skip errors automaticly (automaticly turned on with [-off])\n" \
	      );

#define lol() printf("Lol\n")

char format1[9] = "mp3", format2[9] = "mp3";

// Склеить строки из args[] в where, начиная с from-того и заканчивая len-тым элементом args[],
// если они находятся между стартовым и конечным символами, либо первый символ - не стартовый. from меняется.
bool mergeStrings(char **args, char *where, int &from, int len, char startSymb, char finishSymb) {
	if (from >= len)
		return 0;
	where[0] = 0;
	if (args[from][0] != startSymb) {
		StrCat(where, args[from]);
		return 1;
	}
	StrCat(where, &args[from++][1]);
	for (; from < len; from++) {
		int lenWhere = strlen(where);
		if (where[lenWhere - 1] == finishSymb) {
			StrClear(where, 1);
			return 1;
		}
		where[lenWhere++] = ' ';
		where[lenWhere++] = 0;
		StrCat(where, args[from]);
	}
	where[strlen(where) - 1] = 0;
	return 0;
}

int main(int argc, char *argv[]) {
	bool u = 0, off = 0, user = 1;
	int i, cnt;
	char c = 0, prof[140], file[150], str[280], ffpath[140], param[140] = "-map 0:a -b:a 112k -ac 2";// = "C:\\ffmpeg\\bin\\ffmpeg.exe";
	time_t t, t1;
	FILE *in;
//	Init:;
    t = time(0);
	memset(ffpath, 0, sizeof(ffpath));
	StrCat(ffpath, "C:\\ffmpeg\\bin\\ffmpeg.exe");
	memset(file, 0, sizeof(file));
	sprintf(file, "dir_%I64d.txth", t);
//	Start:
	system("cls");

	// Разбор строки параметров
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "-u"))
			u = 1;
		elif (StrCmp(argv[i], "-param"))
			mergeStrings(argv, param, ++i, argc, '"', '"');
		elif (StrCmp(argv[i], "-formats")) {
			mergeStrings(argv, format1, ++i, argc, '"', '"');
			mergeStrings(argv, format2, ++i, argc, '"', '"');
		} elif (StrCmp(argv[i], "-user"))
			user = 0, u = 1;
		elif (StrCmp(argv[i], "-help")) {
			printHelp();
			return 0;
		} elif (StrCmp(argv[i], "-off"))
			off = 1;
		elif (StrCmp(argv[i], "-ffpath"))
			mergeStrings(argv, ffpath, ++i, argc, '"', '"');
	}

/*	printf("Программа находится в папке с музыкой?\n");
	c = getch();
	if (c == 27) {
		printf("Esc\n");
		quit("Нажатие Esc");
	} elif (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
		printf("%c\n", c);
		quit("Не было получено подтверждение");
	}
	printf("%c\n", c);*/
	Format:;
	if (user) {
		printf("Будут перегоняться файлы из *.%s в *.%s?\n", format1, format2);
		c = getch();
		printf("%c\n", c);
		if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
			printf("Введите расширение (например, \"flac mp3\", \"wma m4a\" без кавычек): ");
			scanf("%s %s", format1, format2);
			getchar();                                                  // magic, eats Enter
			goto Format;
		}
	}
	Opening:;
	memset(str, 0, sizeof(str));
	StrCat(str, "dir /B /A-D >> ");
	StrCat(str, file);
	system(str);
	if ((in = fopen(file, "rt")) == NULL) {
//		fclose(stdin);
		printf("Отказано в доступе к папке для создания временного файла. Нажмите 'Y', если хотите ввести путь "
		       "к папке с доступом, или любую другую клавишу в другом случае.\n");
		if (!user)
			return 1;
		c = getch();
		printf("%c\n", c);
		if (c == 'y' || c == 'Y' || c == 'н' || c == 'Н' || c == 13) {
			memset(file, 0, sizeof(file));
			i = c = 0;
			printf("Введите путь до папки: ");
			while (c != '\n' /*  Or Esc  */) {
				scanf("%c", &file[i++]);
			}
			if (c == '\n') {
				file[i - 1] = 0;
				sprintf(file, "%s\\Tree_%I64d.txth", file, t);
				goto Opening;
			} else {
				quit("Нажатие Esc");
			}
		} else {
			quit("Не получено подтверждение");
		}
	}
//	Prepare:;
	i = 0;
    cnt = 0;
    while (read(str, in)) {
    	if (StrCompare(str, format1, strlen(str) - strlen(format1)))
    		cnt++;
    }
    if (user) {
	    printf("Обнаружено %d файлов. Верно?\n", cnt);
	    c = getch();
	    printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
	    	printf("Не было получено разрешение начать\n");
	    	goto Deleting;
		}
	}
	Pathing:;
	if (user) {
		printf("ffmpeg.exe можно найти по адресу \"%s\"?\n", ffpath);
		c = getch();
	    printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
	    	printf("Напишите путь до ffmpeg.exe: ");
	    	read(ffpath, stdin);
	    	goto Pathing;
		}	
	}
//	Last_prepare:;
	rewind(in);
    i = 0;
    memset(str, 0, sizeof(str));
    sprintf(str, "mkdir \"Output %I64d\"", t);
    if (system(str)) {
    	printf("Ошибка при создании папки выходных файлов.\n");
    	goto Deleting;
	}
	Params:;
	if (user) {
		printf("Сейчас строка компиляции выглядит так: %s -i \"<filename>.%s\" %s \"Output %I64d\\<filename>.%s\"\nИспользовать эту строку? "
		       "В случае отказа (не Y или Enter) вам будет предложено ввести свою строку параметров; Esc - выход\n", ffpath, format1, param, t, format2);
		c = getch();
		if (c == 27) {
			printf("Esc\n");
			goto FullDeleting;
		} else {
			printf("%c\n", c);
			if (c == 'n' || c == 'N') {
				printf("Введите новую строку параметров: ");
				read(param, stdin);
				goto Params;
			}
		}
		printf("Не выключать компьютер после завершения? (Esc, N - выключение)\n");
		c = getch();
		if (c == 'n' || c == 27 || c == 'N' || c == 'т' || c == 'Т')
			off = 1;
	}
	//	Work:;
    i = 0;
    while (read(prof, in)) {
    	if (StrCompare(prof, format1, strlen(prof) - strlen(format1))) {         //prof - name.format
    		system("cls");
    		StrClear(prof, strlen(format1) + 1);                                 //prof - name
    		FFtry:;
    		printf("%d из %d перекодированы, начинаем %d:\n", i, cnt, i + 1);
    		sprintf(str, "title \"now %d of %d\"", i + 1, cnt);
    		system(str);
    		memset(str, 0, sizeof(str));
    		sprintf(str, "%s -i \"%s.%s\" %s \"Output %I64d\\%s.%s\"", ffpath, prof, format1, param, t, prof, format2);
    		printf("%s\n", str);
    		NormalRus(str);
    		if (system(str) && !u) {
    			printf("Ошибка при работе ffmpeg'а. Нажмите R для повтора, Esc для выхода, любую клавишу для пропуска и U для пропуска всех ошибок\n");
    			t1 = time(0);
    			c = 0;
    			while (time(0) - t1 < 20)
	    			if (kbhit()) {
		    			c = getch();
		    			if (c != 27)
		    				printf("%c\n", c);
						if (c == 'r' || c == 'R' || c == 'к' || c == 'К')
							goto FFtry;
						elif (c == 27) {
							printf("Esc\nУдалить перекодированные на данный момент файлы и папку?\n");
							c = getch();
							if (c == 'y' || c == 'Y' || c == 13)
								goto FullDeleting;
							else
								goto Deleting;
						} elif (c == 'u' || c == 'U')
							u = 1;
						else
							break;
						c = 1;
					}
				if (c == 0)
					u = 1, i++;
	    	} else
    			i++;
    	}
    }
    printf("%d из %d\n", i, cnt);
    if (0) {
    	FullDeleting:;
    	sprintf(str, "rmdir \"Output %I64d\" /Q /S", t);
		system(str);
    }
	Deleting:;
	fclose(in);
	if (off) {
		if ((in = fopen("shutdown.no", "rt")) != NULL) {
			off = 0;
			fclose(in);
			system("rm shutdown.no");
		}
	} else
		if ((in = fopen("shutdown.yes", "rt")) != NULL) {
			off = 1;
			fclose(in);
			system("rm shutdown.yes");
		}
	memset(str, 0, sizeof(str));
	StrCat(str, "del ");
	StrCat(str, file);
	if (!system(str)) {
		quit("Временный файл был удален");
	} else {
		quit("Ошибка при удалении временного файла");
	}
	return 0;
}
