#include <stdio.h>
#include <string.h>
//for strlen()
#include <conio.h>
// for getch()
#include <stdlib.h>
// for system()
#include <windows.h>
// for Beep()
#include "ffmpeg_prog.h"

#define VERSION "1.0.0"

#define printHelp() \
	printf("\nffmpeg_camera_capture version: " VERSION "\n" \
	       "[-ffpath <string>] - set path to ffmpeg's .exe file [default: C:\\ffmpeg\\bin\\ffmpeg.exe]\n" \
	       "[-name30 <string>] - set path + name pattern for 30fps videos [default: %%userprofile%%\\Videos\\3D-720]\n" \
	       "[-name60 <string>] - set path + name pattern for 60fps videos [default: %%userprofile%%\\Videos\\3D-360]\n" \
	       "[-number30 <integer>] - set starting number for 30fps videos [default: 0]\n" \
	       "[-number60 <integer>] - set starting number for 60fps videos [default: 0]\n" \
	       "[-audio <boolean>] - set audio recording on or off [default: off]\n" \
	       "[-audioDevice <string>] - set name of audio device to use (don't forget quotes)\n" \
	      );

int numberCorrect(char *name, int n, char *format_out) {
	bool flag = 0;
	FILE *tmpF;
	char tmpS[300];
	while (!flag) {
		n++;
		sprintf(tmpS, "%s-%03d-left.%s", name, n, format_out);
		flag = 1;
		if ((tmpF = fopen(tmpS, "rb")) != NULL)
			fclose(tmpF), flag = 0;
		else {
			sprintf(tmpS, "%s-%03d-right.%s", name, n, format_out);
			if ((tmpF = fopen(tmpS, "rb")) != NULL)
				fclose(tmpF), flag = 0;
		}
	}
	return n;
}

int main (int argc, char *argv[]) {
	char c, ffpath[300] = "C:\\ffmpeg\\bin\\ffmpeg.exe", name30[200] = "%%userprofile%%\\Videos\\3D-720", name60[200] = "%%userprofile%%\\Videos\\3D-360",
	     str[2000], audioS[200] = "\"@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\\wave_{3E91D00C-7F55-47F0-B9C6-986C43CFE953}\"",
	     format_out[15] = "mkv";
	int n30 = 0, n60 = 0;
	bool audio = 0;
	for (int i = 1; i < argc; i++) {
		char command[80] = "";
		StrCat(command, argv[i]);
		for (size_t j = 0; j < strlen(command); j++)
			command[j] = tolower(command[j]);
		if (StrCmp(command, 2, "-name30", "/name30"))
			StrCopy(name30, argv[++i]);
		else if (StrCmp(command, 2, "-name60", "/name60"))
			StrCopy(name60, argv[++i]);
		else if (StrCmp(command, 2, "-number30", "/number30"))
			sscanf(argv[++i], "%d", &n30);
		else if (StrCmp(command, 2, "-number60", "/number60"))
			sscanf(argv[++i], "%d", &n60);
		else if (StrCmp(command, 2, "-audio", "/audio")) {
			i++;
			if (StrCmp(argv[i], 3, "1", "true", "on"))
				audio = 1;
			else if (StrCmp(argv[i], "0" ) || StrCmp(argv[i], "false") || StrCmp(argv[i], "off"))
				audio = 0;
			else
				printf("Unknown combination: \"audio %s\"; audio recording will be on\n", argv[i]);
		} else if (StrCmp(command, 2, "-audiodevice", "/audiodevice")) {
			StrCopy(audioS, argv[++i]);
		} else if (StrCmp(command, 2, "-help", "/help")) {
			printHelp();
			printf("Help is printed; exiting\n");
			system("pause");
			return 0;
		} else if (StrCmp(command, 2, "-ffpath", "/ffpath"))
			StrCopy(ffpath, argv[++i]);
		else {
			printf("Can't recognize parameter: %s / %s\n", argv[i], command);
			system("pause");
		}
	}
	bool menu = 1;
	while (1) {
		if (menu) {
			printf("\nffpath = %s\naudio (%s) = %s\nname30 (number = %d) = %s\nname60 (number = %d) = %s\nPress '3' for start recording 1280x720@30fps stereo video, "
			       "'6' for 640x320@60fps, 'p' for editing parameters, 'Esc' for exit\n\n>>",
			       ffpath, audio ? "on" : "off", audioS, n30, name30, n60, name60);
			menu = 0;
		}
		c = getch();
		if (c == 27) {
			printf("Exiting;\n");
			system("pause");
			return 0;
		}
		printf("%c\n", c);
		if (c == '3') {
			n30 = numberCorrect(name30, n30, format_out);
			sprintf(str, "%s -f dshow -s 1280x720 -vcodec mjpeg -framerate 30 -rtbufsize 400M -i video=\"Stereo Vision 1\""
			             "%s%s -f dshow -s 1280x720 -vcodec mjpeg -framerate 30 -rtbufsize 400M -i video=\"Stereo Vision 2\" -c:v copy "
			             "-c:a libvorbis -q:a 1 -af volume=5 -map 0:0 -map 0:1? -copyts -y \"%s-%03d-left.%s\" -map 1:0 -c "
			             "copy -copyts -y \"%s-%03d-right.%s\"",
			             ffpath, audio ? ":audio=" : "", audio ? audioS : "", name30, n30, format_out, name30, n30, format_out);
			printf("%s\n", str);
			Beep(3000, 600);
			Beep(5000, 400);
			Beep(7000, 300);
			if (system(str))
				Beep(9000, 1300), printf("Error of ffmpeg working\n");
		} else if (c == '6') {
			n60 = numberCorrect(name60, n60, format_out);
			sprintf(str, "%s -f dshow -s 640x360 -vcodec mjpeg -framerate 60 -rtbufsize 400M -i video=\"Stereo Vision 1\""
			             "%s%s -f dshow -s 640x360 -vcodec mjpeg -framerate 60 -rtbufsize 400M -i video=\"Stereo Vision 2\" -c:v copy "
			             "-c:a libvorbis -q:a 1 -af volume=5 -map 0:0 -map 0:1? -copyts -y \"%s-%03d-left.%s\" -map 1:0 -c "
			             "copy -copyts -y \"%s-%03d-right.%s\"",
			             ffpath, audio ? ":audio=" : "", audio ? audioS : "", name60, n60, name60, format_out, n60, format_out);
			printf("%s\n", str);
			Beep(7000, 600);
			Beep(5000, 400);
			Beep(3000, 300);
			if (system(str))
				Beep(9000, 1300), printf("Error of ffmpeg working\n");
		} else if (c == 'p'|| c == 'P') {
			while (1) {
				printf("\n\"path\" / \"p\" for entering path to ffmpeg.exe\n\"name30\" for entering path+name for 1280x720@30fps out files\n"
				       "\"name60\" for entering path+name pattern for 640x320@60fps out files\n\"toggleAudio\" for enable/disale audio recording\n"
				       "* \"pattern\" means that files will be like \"pattern-resolution-number-eye\"\n"
				       "\"number30\" for entering starting number for 720p@30 files\n\"number60\" for entering sarting number for 320p@60\n"
				       "* \"starting number\" works only when there's no file with this number, otherwise first unused number will be used\n"
				       "\"audio\" for entering audio device's name (look for it, typing \"ffmpeg -list_deices true -f dshow -i dummy\")\n"
				       "\"exit\" / \"stop\" for exiting from program\n(empty string) / \"start\" / \"go\"\n");
				c = readWord(str, stdin);
				for (size_t i = 0; i < strlen(str); i++)
					str[i] = tolower(str[i]);
				if (StrCmp(str, 3, "", "go", "start"))
					break;
				if (StrCmp(str, 2, "exit", "stop")) {
					printf("Exiting\n");
					system("pause");
					return 0;
				}
				else if (StrCmp(str, 2, "path", "p")) {
					if (c == 10)
						printf("Input path to ffmpeg (including \"ffmpeg.exe\"): ");
					read(ffpath, stdin);
				} else if (StrCmp(str, 2, "name30", "s30")) {
					if (c == 10)
						printf("Input path + name pattern for 720p@30 files: ");
					read(name30, stdin);
				} else if (StrCmp(str, 2, "name60", "s60")) {
					if (c == 10)
						printf("Input path + name pattern for 360p@60 files: ");
					read(name60, stdin);
				} else if (StrCmp(str, 2, "number30", "n30")) {
					if (c == 10)
						printf("Input starting number for 720p@30 files: ");
					scanf("%d", &n30);
				} else if (StrCmp(str, 2, "number60", "n60")) {
					if (c == 10)
						printf("Input starting number for 360p@60 files: ");
					scanf("%d", &n60);
				} else if (StrCmp(str, 3, "formatout", "fo", "fout")) {
					if (c == 10)
						printf("Input format for output files (without a dot): ");
					read(format_out, stdin);
				} else if (StrCmp(str, 2, "toggleaudio", "ta")) {
					audio = !audio;
				} else
					printf("Unknown parameter: %s", str);
			}
		} else {
			printf("Unknown parameter: %c", c);
		}
		menu = 1;
	}
	return 0;
}
