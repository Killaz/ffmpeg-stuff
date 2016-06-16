// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printf("%s; Завершение программы.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("[endless] - programm won't close after one picture\n" \
	       "[reverse] - change left with right (needed for Oculus, for example)\n" \
           "[exit] - close thr programm\n" \
           "[help] - print this help\n" \
      );

#define lol() printf("Lol\n")

char format[9] = "";

void StrCat(char *where, char *from) {
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

// Сравнивает строку where со строкой from,начиная с from-того символа
bool StrCompare(const char *where, const char *what, int from) {
	int len = strlen(what), len1 = strlen(where);
	for (int i = 0; i < len; i++) {
		if (from + i >= len1 || where[from + i] != what[i])
			return 0;
	}
	return 1;
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

// Отрезатет от конца строки from hmuch символов
bool StrClear(char *from, int hmuch) {
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

// Возвращает 1, если есть формат файла, и , если его нет.
// В первом случае заменяет строку format на данный формат.
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

// Читает строку до переноса строки
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
	char c = 0, tmp = 0, file1[120], *file2 = NULL, str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1";
	char first = 0; // 1, когда из параметровполучено имя файла
	bool u = 0;     // skip-parameter             
	int i;
	bool rev = 0;
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
//	Start:
	system("cls");
// Разбор строки параметров
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i], "reverse"))
			rev = 1, printf("reverse: on\n");
		elif (StrCmp(argv[i], "ffpath"))
			if (i < argc - 1)
				StrCat(ffpath, argv[++i]);
			else
				printf("ffpath-parameter is used without path. Ignored.\n");
		elif (Analize(argv[i]) || argv[i][0] == '"') {
			if (first == 0)
				StrCat(file1, argv[i]), first = 1;
			elif (first == 1)
				StrCat(file2, argv[i]), first = 2;
			else
				printf("Unknown parameter: %s\n", argv[i]);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	if (first == 1) {
		first = 0;
		if (u)
			goto FFtry;
		else
			goto Pathing;
	}

	printf("Путь, если необходимо, нужно вводить с кавычками. Можно \"перетащить\" файл в консоль для получения пути\n");
	Opening:;
	printf("Введите путь до файла (стереокартинки/видео для разделения на два изображения/видео): ");
	read(file1, stdin);

	if (StrCmp(file1, "endless")) {
		u ^= 1, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (StrCmp(file1, "exit")) {
		return 0;
	} elif (StrCmp(file1, "help")) {
		printHelp();
		goto Opening;
	} elif (!Analize(file1)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	}

	if (u)
		goto FFtry;
	printf("Файл: %s.%s\n(N - заново написать путь, Esc - выход, F - указать расположение "
	       "ffmpeg'а (если не C:\\ffmpeg\\bin), B - Задать строку параметров (если не -q:v 1), K - F+B:\n", file1, format);
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

	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format[strlen(format) + 1] = 0,format[strlen(format)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
	}

	memset(str, 0, sizeof(str));
	if (StrCompare(file1, "-all", strlen(file1) - 4)) {
		if ((file2 = (char *) malloc((strlen(file1) - 4) * sizeof(char))) == NULL)
			file2 = file1;
		else {
			file2[0] = 0;
			StrCatN(file2, file1, 0, strlen(file1) - 4);
		}
	} else
		file2 = file1;
	
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:0:0\" %s %s-%s.%s", 
	        ffpath, file1, format, param, file2, rev ? "left" : "right", format);
	printf("%s\n", str);
	if (system(str)) {
		quit("Ошибка при работе ffmpeg'а - создание первого файла");
	}
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:iw/2:0\" %s %s-%s.%s",
	        ffpath, file1, format, param, file2, rev ? "right" : "left", format);
	printf("\nSTART: %s\n", str);
	if (system(str)) {
		quit("Ошибка при работе ffmpeg'а - создание первого файла");
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
