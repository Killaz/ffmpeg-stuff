#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printRusComment(a); \
	printRusComment("; Завершение программы.\n"); \
	if (off) { \
		system("shutdown /s /t 120"); \
		printRusComment("Компьютер будет выключен через 120 секунд.\n"); \
	} \
	system("pause"); \
	return 0;
#define lol() printf("Lol\n")

char format1[9] = "mp3", format2[9] = "mp3";

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

bool StrClear(char *from, int hmuch) { // Отрезать от конца строки <from> <hmuch> символов
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
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
	return StrCompare(s, format1, strlen(s) - strlen(format1));
}

int main() {
	bool u = 0, off = 0;
	int i, cnt;
	char c = 0, prof[90], file[90], str[250], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[80] = "-map 0:a -b:a 112k -ac 2", tmp[800];
	time_t t, t1;
	FILE *in = NULL;
//	Init:;
    t = time(0);
	memset(prof, 0, sizeof(prof));
	StrCat(prof, getenv("USERPROFILE"));
	memset(file, 0, sizeof(file));
	sprintf(file, "%s\\Documents\\Tree_%ld.txth", prof, t);
//	Start:
	system("cls");
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
	printRusComment("Будут перегоняться файлы ");
	printf("*.%s -> *.%s?\n", format1, format2);
	c = getch();
	printf("%c\n", c);
	if (c == 27)
		goto Deleting;
	if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
		printRusComment("Введите расширение (например, \"flac mp3\", \"wma m4a\" без кавычек): ");
		scanf("%s %s", format1, format2);
		goto Format;
	}
	Opening:;
	memset(str, 0, sizeof(str));
	StrCat(str, "tree /f /a >> ");
	StrCat(str, file);
	system(str);
	if ((in = fopen(file, "rt")) == NULL) {
		fclose(stdin);
		printRusComment("Отказано в доступе к папке для создания временного файла. Нажмите 'Y', "
		                "если хотите ввести путь к папке с доступом, и любую другую клавишу в другом случае.\n");
		c = getch();
		printf("%c\n", c);
		if (c == 'y' || c == 'Y' || c == 'н' || c == 'Н' || c == 13) {
			memset(file, 0, sizeof(file));
			i = c = 0;
			printRusComment("Введите путь до папки: ");
			while (c != '\n' /*  Or Esc  */) {
				scanf("%c", &file[i++]);
			}
			if (c == '\n') {
				file[i - 1] = 0;
				sprintf(file, "%s\\Tree_%ld.txth", file, t);
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
    while (i < 3) {
    	fscanf(in, "%c", &c);
    	if (c == '\n')
    		i++;
    }
    cnt = 0;
    while (1) {
    	fscanf(in, "%c%*c%*c%*c", &c);
    	if (c != '|' && c != ' ')
    		break;
    	if (read(str, in))
    		cnt++;
    }
    sprintf(tmp, "Обнаружено %d файлов. Верно?\n", cnt);
    printRusComment(tmp);
    c = getch();
    printf("%c\n", c);
    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
    	printRusComment("Не было получено разрешение начать\n");
    	goto Deleting;
	}
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
//	Last_prepare:;
	rewind(in);
    i = 0;
    while (i < 3) {
    	fscanf(in, "%c", &c);
    	if (c == '\n')
    		i++;
    }
    memset(str, 0, sizeof(str));
    sprintf(str, "mkdir \"Output %ld\"", t);
    if (system(str)) {
    	printRusComment("Ошибка при создании папки выходных файлов.\n");
    	goto Deleting;
	}
	Params:;
	sprintf(tmp, "Сейчас строка компиляции выглядит так: %s -i \"<filename>.%s\" %s \"Output %ld\\<filename>.%s\"\nИспользовать эту строку? "
	       "В случае отказа (не Y или Enter) вам будет предложено ввести свою строку параметров; Esc - выход\n", ffpath, format1, param, t, format2);
	printRusComment(tmp);
	c = getch();
	if (c == 27) {
		printf("Esc\n");
		goto FullDeleting;
	} else
		printf("%c\n", c);
	if (c == 'n' || c == 'N' || c == 'т' || c == 'Т') {
		printRusComment("Введите новую строку параметров: ");
		read(param, stdin);
		goto Params;
	}
	printRusComment("Не выключать компьютер после завершения? (Esc, N - выключение)\n");
	c = getch();
	if (c == 'n' || c == 27 || c == 'N' || c == 'т' || c == 'Т')
		off = 1;
//	Work:;
    i = 0;
    while (1) {
    	fscanf(in, "%c%*c%*c%*c", &c);
    	if (c != '|' && c != ' ') {              //Completed (?)
    		fscanf(in, "%s%s", str, prof);
    		printf("%c%s %s\n", c, str, prof);
    		break;
		}
    	if (read(prof, in)) {                    //prof - name.format
    		system("cls");
    		StrClear(prof, strlen(format1) + 1); //prof - name
    		FFtry:;
    		sprintf(tmp, "%d из %d перекодированы, начинаем %d:\n", i, cnt, i + 1);
    		printRusComment(tmp);
    		memset(str, 0, sizeof(str));
    		sprintf(str, "%s -i \"%s.%s\" %s \"Output %ld\\%s.%s\"", ffpath, prof, format1, param, t, prof, format2);
    		printf("%s\n", str);
    		if (system(str) && !u) {
    			printRusComment("Ошибка при работе ffmpeg'а. Нажмите R для повтора, Esc для выхода, любую клавишу для пропуска и U для пропуска всех ошибок данного типа\n");
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
							printRusComment("Esc\nУдалить перекодированные на данный момент файлы и папку?\n");
							c = getch();
							if (c == 'y' || c == 'Y' || c == 'н' || c == 'Н' || c == 13)
								goto FullDeleting;
							else
								goto Deleting;
						} elif (c == 'u' || c == 'U' || c == 'г' || c == 'Г')
							u = 1;
						c = 1;
					}
				if (c == 0)
					u = 1, i++;
	    	} else
    			i++;
    	}
    }
    sprintf(tmp, "%d из %d были перекодированы без ошибок%s\n", i, cnt, (u) ? " (или с их авто-игнорированием)" : "");
	printRusComment(tmp);
    if (0) {
    	FullDeleting:;
    	sprintf(str, "rmdir \"Output %ld\" /Q /S", t);
		system(str);
    }
	Deleting:;
	fclose(in);
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
