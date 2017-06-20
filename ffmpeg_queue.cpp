// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
// for getch()
#include <string.h>
// for strlen()
#include <stdlib.h>
// for system()
#include <time.h>
// for time()
#include <ctype.h>
// for tolower()

#include "ffmpeg_prog.h"

#define quit(a) \
	printf("%s; Завершение программы.\n", a); \
	if (shutdown) { \
		system("shutdown /s /t 120"); \
		printf("Компьютер будет выключен через 120 секунд.\n\n"); \
	} \
	system("pause"); \
	return 0;
#define VERSION "1.0.1"

#define printHelp() \
	printf("ffmpeg_queue version: " VERSION "\n" \
	       "[-ffpath <string>] or [-ffpath <string with spaces>\"] - set path to ffmpeg's .exe file\n" \
	       "[-inSign <string>] - set \"-i\" to something different (empty for example, for use not with ffmpeg)\n" \
	       "[-outSign <string>] - set empty string before path to output file to something else\n" \
	       "[-formats <format_in>,<format_out>] or [-formats \"format_in, format_out\"] or [-formats <format_in-out>]\n" \
	       "     - set coding from format_in to format_out\n" \
	       "[-inParams <string>] or [-inParams \"<string with spaces>\"] - give input files parameters to ffmpeg\n" \
	       "[-outParams <string>] or [-outParams \"<string with spaces>\"] - give output files parameters to ffmpeg\n" \
	       "[-inDir <string>] or [-inDir \"<string with spaces>\"] - set input folder (path to program's exe file by default)\n" \
	       "[-outDir <string>] or [-outDir \"<string with spaces>\"] - set output folder (creates automaticly by default)\n" \
	       "[-skip] - skip errors automaticly (automaticly turned on with [-shutdown])\n" \
	       "[-noUser] - menu won't be displayed, no questions will be asked\n"  \
	       "[-shutdown] - turns off computer after finishing\n" \
	       "[-noOverrideOutDir] - you can put -outDir first, path won't be merged to inDir (that's not what you want usually)\n" \
	       "[-noRepairPaths] - short pathes won't be merged with launch dir's path nor with inDir (will be hiddenly merged with exe's path)\n" \
	      );

#define lol() printf("Lol\n")

char format_in[15] = "avi", format_out[15] = "mkv";

// Склеить строки из args[] в where, начиная с from-того и заканчивая len-тым элементом args[],
// если они находятся между стартовым и конечным символами, либо первый символ - не стартовый. from меняется.
bool mergeStrings(char **args, char *where, int &from, int len, char startSymb, char finishSymb) { // is not used anymore; may not work
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
	bool skip = 0, shutdown = 0, user = 1, overrideOutDir = 1, repairPaths = 1, edited = 0;
	int i, cnt;//, lang = 1; // lang = 0 - eng, 1 - rus
	char c = 0, fileName[140], file[350], str[2000], ffpath[300] = "C:\\ffmpeg\\bin\\ffmpeg.exe",
	     inParams[400] = "", outParams[400] = "-map 0 -c:a libopus -vbr:a on -b:a 192k -c:v libx265 -qmin:v 22 -crf:v 25 -qmax:v 32",
	     inSign[15] = "-i", outSign[15] = "", inDir[300] = "", outDir[300] = "", cd[300] = "",
	     inDir_norm[300], outDir_norm[300], ffpath_norm[300], cd_norm[300];
	unsigned long long t, t1;
	FILE *in = NULL;
	Init:;
	t = (unsigned long long) time(0);
	sprintf(file, "dir_%I64u.txth", t);
//	Start:
	system("cls");

//	Getting %cd% (will fail if we have no rights to create files in exe's directory)
	if (!edited) {
		sprintf(str, "echo %%cd%% >> cd_%I64u.txth", t);
		system(str);
		sprintf(str, "cd_%I64u.txth", t);
		in = fopen(str, "rt");
		read(inDir, in);
		fclose(in);
		in = NULL;
		sprintf(str, "erase cd_%I64u.txth", t);
		system(str);
		inDir[strlen(inDir) - 1] = '\\';
		StrCopy(cd, inDir);
		sprintf(outDir, "%sOutput %I64u\\", inDir, t);
		StrCopy(inDir_norm, inDir);
		StrCopy(outDir_norm, outDir);
		StrCopy(cd_norm, cd);
		NormalRus(inDir_norm);
		NormalRus(outDir_norm);
		NormalRus(cd_norm);
	}

	// Разбор строки параметров
	// Parameter string processing
	if (!edited) {
		for (i = 1; i < argc; i++) {
			char command[80] = "";
			StrCat(command, argv[i]);
			for (size_t j = 0; j < strlen(command); j++)
				command[j] = tolower(command[j]);
			if (StrCmp(command, 2, "-inparams", "/inparams"))
				StrCat(inParams, argv[++i]);
			else if (StrCmp(command, 2, "-outparams", "/outparams"))
				StrCopy(outParams, argv[++i]);
			else if (StrCmp(command, 2, "-insign", "/insign"))
				StrCopy(inSign, argv[++i]);
			else if (StrCmp(command, 2, "-outsign", "/outsign"))
				StrCopy(outSign, argv[++i]);
			else if (StrCmp(command, 2, "-indir", "/indir")) {
				StrCopy(inDir, argv[++i]);
				if (inDir[strlen(inDir) - 1] != '\\') {
					inDir[strlen(inDir) + 1] = 0;
					inDir[strlen(inDir)] = '\\';
				}
			} else if (StrCmp(command, 2, "-outdir", "/outdir")) {
				StrCopy(outDir, argv[++i]);
				if (outDir[strlen(outDir) - 1] != '\\') {
					outDir[strlen(outDir) + 1] = 0;
					outDir[strlen(outDir)] = '\\';
				}
			} else if (StrCmp(command, 2, "-formats", "/formats")) {
				StrClear(format_in);
				StrClear(format_out);
				i++;
				int comma = -1;
				for (size_t j = 0; j < strlen(argv[i]); j++)
					if (argv[i][j] == ',')
						comma = j;
				if (comma == -1)
					StrCat(format_in, argv[i]), StrCat(format_out, argv[i]);
				else {
					StrCatN(format_in, argv[i], comma);
					for (size_t j = comma + 1; j < strlen(argv[i]); j++)
						if (argv[i][j] != ' ') {
							comma = j;
							break;
						}
					StrCatN(format_out, argv[i], comma, strlen(argv[i]));
				}
			} else if (StrCmp(command, 2, "-nouser", "/nouser"))
			user = 0, skip = 1;
			else if (StrCmp(command, 2, "-help", "/help")) {
				printHelp();
				quit("Help printed; quiting");
			} else if (StrCmp(command, 4, "-shutdown", "/shutdown", "-off", "/off"))
				shutdown = 1;
			else if (StrCmp(command, 2, "-ffpath", "/ffpath"))
				StrCopy(ffpath, argv[++i]);
			else if (StrCmp(command, 2, "-skip", "/skip"))
				skip = 1;
			else if (StrCmp(command, 2, "-nooverrideoutdir", "/nooverrideoutdir"))
				overrideOutDir = 0;
			else if (StrCmp(command, 2, "-norepairpaths", "/norepairpaths"))
				repairPaths = 0;
			else {
				printf("Can't recognize parameter: %s / %s\n", argv[i], command);
				system("pause");
			}
		}
		if (repairPaths && !StrCompare(inDir, ":\\", 1) && inDir[0] != '%') {
			sprintf(str, "%s%s", cd, inDir);
			sprintf(inDir_norm, "%s%s", cd_norm, inDir);
			StrCopy(inDir, str);
		}
		if (repairPaths && !StrCompare(outDir, ":\\", 1) && outDir[0] != '%') {
			if (overrideOutDir) {
				sprintf(str, "%s%s", inDir, outDir);
				StrCopy(outDir, str);
			} else
				sprintf(str, "%s%s", cd, outDir);
				sprintf(outDir_norm, "%s%s", cd_norm, outDir);
				StrCopy(outDir, str);
		}
		StrCopy(ffpath_norm, ffpath);
	}
/*	printf("Программа находится в папке с музыкой?\n");
	c = getch();
	if (c == 27) {
		printf("Esc\n");
		quit("Нажатие Esc");
	} else if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
		printf("%c\n", c);
		quit("Не было получено подтверждение");
	}
	printf("%c\n", c);*/
/*	Format:;
	if (user) {
		printf("Будут перегоняться файлы из *.%s в *.%s?\n", format_in, format_out);
		c = getch();
		if (c != 27) printf("%c\n", c);
		if (c != 'y' && c != 'Y' && c != 13 && c != 27) {
			printf("Введите расширение (например, \"flac mp3\", \"wma m4a\" без кавычек): ");
			scanf("%s %s", format_in, format_out);
			getchar();                                                  // magic, eats Enter
			goto Format;
		} else if (c == 27) {
			quit("Нажатие Esc");
		}
	}*/
	Opening:;
	if (in != NULL) {
		fclose(in), in = NULL;
		sprintf(str, "erase %s", file);
		system(str);
	}
	sprintf(str, "dir \"%s\" /B /A-D >> \"%s\"", inDir_norm, file);
	system(str);
	if ((in = fopen(file, "rt")) == NULL) { // never has been tested, lol
//		fclose(stdin);
		printf("Отказано в доступе к папке для создания временного файла. Нажмите 'Y', если хотите ввести путь "
		       "к папке с доступом, или любую другую клавишу в другом случае.\n");
		if (!user)
			return 1;
		c = getch();
		if (c != 27) printf("%c\n", c);
		if (c == 'y' || c == 'Y' || c == 'н' || c == 'Н' || c == 13) {
			memset(file, 0, sizeof(file));
			i = c = 0;
			printf("Введите путь до папки: ");
			do {
				scanf("%c", &file[i]);
			} while (file[i++] != '\n');
			if (file[i - 1] == '\n') {
				file[i - 1] = 0;
				sprintf(file, "%s\\dir_%I64u.txth", file, t); //won't work with rus paths, but who cares
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
    	if (StrCompareEndings(str, format_in))
    		cnt++;
    }
/*    if (user) {
	    printf("Обнаружено %d файлов. Верно?\n", cnt);
	    c = getch();
	    printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
	    	printf("Не было получено разрешение начать\n");
	    	goto Deleting;
		}
	}*/
/*	Pathing:;
	if (user) {
		printf("ffmpeg.exe можно найти по адресу \"%s\"?\n", ffpath);
		c = getch();
	    if (c != 27) printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != 'н' && c != 'Н' && c != 13) {
	    	printf("Напишите путь до ffmpeg.exe: ");
	    	read(ffpath, stdin);
	    	goto Pathing;
		}
	}*/
//	Last_prepare:;

	ParamsMenu:;
	if (user) {
		printf("\nНайдено %d файлов;\nСейчас строка вызова выглядит так (выключение: %s):\n%s %s %s \"%s<filename>.%s\" %s %s \"%s<filename>.%s\"\n"
		       "Введите:\n\"path\" / \"p\", чтобы изменить путь до ffmpeg.exe,\n"
		       "\"inParams\" / \"ip\" для изменения параметров входных файлов,\n"
		       "\"inSign\" / \"is\" для изменения параметра перед входным файлом,\n"
		       "\"inDir\" для изменения директории входных файлов,\n"
		       "    \"outDir_noRepair\" - ввод короткого пути без его раскрытия относительно %%cd%% запуска,\n"
		       "\"formatIn\" / \"fi\" / \"fin\" для изменения формата входных файлов,\n"
		       "\"outParams\" / \"op\" для изменения параметров выходных файлов,\n"
		       "\"outSign\" / \"os\" для изменения параметра перед выходным файлом,\n"
		       "\"outDir\" / \"od\" для изменения папки вывода,\n"
		       "    \"outDir_noRepair\" - ввод короткого пути без его раскрытия относительно папки входных файлов,\n"
		       "\"formatOut\" / \"fo\" / \"fout\" для изменения формата выходных файлов,\n"
		       "\"shutdown\" / \"off\" для изменения параметра выключения после перекодировки,\n"
		       "(пустая строка) / \"start\" / \"go\" - начать перекодировку с текущими параметрами,\n\"exit\" / \"stop\" - выход\n>> ",
		       cnt, shutdown ? "будет произведено" : "не будет произведено", ffpath, inParams, inSign, inDir, format_in,
		       outParams, outSign, outDir, format_out);
		c = readWord(str, stdin);
		for (size_t i = 0; i < strlen(str); i++)
			str[i] = tolower(str[i]);
		if (StrCmp(str, 3, "", "go", "start")) {
			if (edited) {
				user = 0;
				StrClear(str);
				fclose(in);
				sprintf(str, "erase %s", file);
				system(str);
				goto Init;
			} else 
				goto Work;
		}
		edited = 1;
		if (StrCmp(str, 2, "exit", "stop")) {
			printf("Exiting;\n");
			goto Deleting;
		} else if (StrCmp(str, 2, "path", "p")) {
			if (c == 10)
				printf("Введите путь до ffmpeg.exe (включая сам \"ffmpeg.exe\"): ");
			read(ffpath, stdin);
		} else if (StrCmp(str, 2, "inparams", "ip")) {
			if (c == 10)
				printf("Введите строку параметров для входных файлов: ");
			read(inParams, stdin);
		} else if (StrCmp(str, 2, "insign", "is")) {
			if (c == 10)
				printf("Введите параметр, предворяющий написание входного файла (\"-i\"): ");
			read(inSign, stdin);
		} else if (StrCmp(str, 4, "indir", "id", "id_nr", "indir_norepair")) {
			if (c == 10)
				printf("Введите путь до папки с входными файлами: ");
			read(inDir, stdin);
			if (inDir[strlen(inDir) - 1] != '\\') {
				inDir[strlen(inDir) + 1] = 0;
				inDir[strlen(inDir)] = '\\';
			}
			StrCopy(inDir_norm, inDir);
			NormalRus(inDir_norm);
			if (!StrCmp(str, 2, "id_nr", "indir_norepair") && !StrCompare(inDir, ":\\", 1) && inDir[0] != '%') {
				StrClear(str);
				sprintf(str, "%s%s", cd, inDir);
				StrCopy(inDir, str);
				sprintf(str, "%s%s", cd_norm, inDir_norm);
				StrCopy(inDir_norm, str);
			} else {
				StrCopy(inDir_norm, inDir);
				NormalRus(inDir_norm);
			}
			goto Opening;
		} else if (StrCmp(str, 3, "formatin", "fi", "fin")) {
			if (c == 10)
				printf("Введите формат входных файлов для перекодировки(без точки): ");
			read(format_in, stdin);
			goto Opening;
		} else if (StrCmp(str, 2, "outparams", "op")) {
			if (c == 10)
				printf("Введите строку параметров для выходных файлов: ");
			read(outParams, stdin);
		} else if (StrCmp(str, 2, "outsign", "os")) {
			if (c == 10)
				printf("Введите строку параметров для выходных файлов: ");
			read(outSign, stdin);
		} else if (StrCmp(str, 4, "outdir", "od", "od_nr", "outdir_norepair")) {
			if (c == 10)
				printf("Введите путь до папки вывода: ");
			read(outDir, stdin);
			if (outDir[strlen(outDir) - 1] != '\\') {
				outDir[strlen(outDir) + 1] = 0;
				outDir[strlen(outDir)] = '\\';
			}
			StrCopy(outDir_norm, outDir);
			NormalRus(outDir_norm);
			if (!StrCmp(str, 2, "od_nr", "outdir_norepair") && !StrCompare(inDir, ":\\", 1) && inDir[0] != '%') {
				sprintf(str, "%s%s", inDir, outDir);
				StrCopy(outDir, str);
				sprintf(str, "%s%s", inDir_norm, outDir_norm);
				StrCopy(outDir_norm, str);
			} else {
				StrCopy(outDir_norm, outDir);
				NormalRus(outDir_norm);
			}
			goto Opening;
		} else if (StrCmp(str, 3, "formatout", "fo", "fout")) {
			if (c == 10)
				printf("Введите формат выходных файлов (без точки): ");
			read(format_out, stdin);
			goto Opening;
		} else if (StrCmp(str, 2, "shutdown", "off"))
			shutdown = !shutdown;
		else
			printf("Can't recognize command: %s\n", str);
		printf("\n");
		goto ParamsMenu;
	}
	StrCopy(ffpath_norm, ffpath);
	NormalRus(ffpath_norm);
	if (edited)
		user = 1;
	Work:;
	rewind(in);
    sprintf(str, "mkdir \"%s\"", outDir_norm);
    if (system(str)) {
    	printf("Ошибка при создании папки выходных файлов.\n");
    	goto Deleting;
	}
    i = 0;
    while (read(fileName, in)) {
    	if (StrCompareEndings(fileName, format_in)) {         //fileName - name.format
    		system("cls");
    		StrClear(fileName, strlen(format_in) + 1);        //fileName - name
    		FFtry:;
    		printf("%d из %d перекодированы, начинаем %d:\n", i, cnt, i + 1);
    		sprintf(str, "title \"now %d of %d%s\"", i + 1, cnt, shutdown ? "; Will turn off" : "");
    		system(str);
    		sprintf(str, "%s %s %s \"%s%s.%s\" %s %s \"%s%s.%s\"", ffpath, inParams, inSign, inDir, fileName,
    		                                                       format_in, outParams, outSign, outDir, fileName, format_out);
    		printf("%s\n", str);
//    		NormalRus(str);
			char fileName_norm[140];
			StrCopy(fileName_norm, fileName);
			NormalRus(fileName_norm);
    		sprintf(str, "%s %s %s \"%s%s.%s\" %s %s \"%s%s.%s\"", ffpath_norm, inParams, inSign, inDir_norm, fileName_norm,
    		                                                       format_in, outParams, outSign, outDir_norm, fileName_norm, format_out);
    		if (system(str) && !skip) {
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
						else if (c == 27) {
							printf("Esc\nУдалить перекодированные на данный момент файлы и папку?\n");
							c = getch();
							if (c == 'y' || c == 'Y' || c == 13)
								goto FullDeleting;
							else
								goto Deleting;
						} else if (c == 'u' || c == 'U')
							skip = 1;
						else
							break;
						c = 1;
					}
				if (c == 0)
					skip = 1, i++;
	    	} else
    			i++;
			printf(">> %s %s %s \"%s%s.%s\" %s %s \"%s%s.%s\"\n", ffpath, inParams, inSign, inDir, fileName,
    		                                                       format_in, outParams, outSign, outDir, fileName, format_out);
    	}
    }
    printf("%d из %d\n", i, cnt);
    if (i == 0) {
    	FullDeleting:;
    	sprintf(str, "rmdir \"%s\" /Q /S", outDir_norm);
		system(str);
    }
	Deleting:;
	fclose(in);
	if (shutdown) {
		sprintf(str, "%sshutdown.no", inDir_norm);
		if ((in = fopen(str, "rt")) != NULL) {
			shutdown = 0;
			fclose(in);
			system("erase shutdown.no");
			printf("Shutdown canceled: shutdown.no found\n");
		}
	} else {
		sprintf(str, "%sshutdown.yes", inDir_norm);
		if ((in = fopen("shutdown.yes", "rt")) != NULL) {
			shutdown = 1;
			fclose(in);
			system("erase shutdown.yes");
			printf("Will shut down the PC: shutdown.yes found\n");
		}
	}
	sprintf(str, "title \"Finished: %I64d\"", (unsigned long long) time(0));
	system(str);
	sprintf(str, "erase %s", file);
	if (!system(str)) {
		quit("Временный файл был удален");
	} else {
		quit("Ошибка при удалении временного файла");
	}
	return 0;
}
