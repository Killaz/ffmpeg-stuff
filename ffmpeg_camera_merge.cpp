#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "ffmpeg_prog.h"

#include <vector>

int main(int argc, char *argv[]) {
	char inPattern[80] = "3D-720", outEnding[80] = "sbsl", inDir[200] = "\%userprofile\%\\Videos\\", str[2000], dirFile[80] = "",
	     leftEnding[20] = "left", rightEnding[20] = "right", format_in[15] = "mkv", format_out[15] = "mkv",
	     ffpath[300] = "D:\\ffmpeg\\bin\\ffmpeg.exe", outParams[400] = "-c:v libx264 -qmin:v 22 -crf:v 25 -qmax:v 28 -c:a libvorbis -q:a 1 -pix_fmt yuv420p -copyts";
	bool sbsr = 1;
	std::vector<char*> leftList, rightList, outList;
	unsigned long long t = time(0);
	FILE *in = NULL;
	sprintf(dirFile, "dir_%I64u.txth", t);
	sprintf(str, "dir \"%s\" /B /A-D >> \"%s\"", inDir, dirFile);
	system(str);
	assert(in = fopen(dirFile, "rt"));
	while (1) {
		if (!(read(str, in)))
			break;
		if (!StrCompare(str, inPattern, 0))
			continue;
		char *start = NULL;
		if (start = strstr(str, leftEnding)) {
			char *tmp = /*(char *) malloc(sizeof(char) * (start-str + 2));*/ new char[start - str + 2];
			StrClear(tmp);
			StrCatN(tmp, str, start - str);
			Analize(str, format_in);
			leftList.push_back(tmp);
		} else if (start = strstr(str, rightEnding)) {
			char *tmp = new char[start - str + 2];
			StrClear(tmp);
			StrCatN(tmp, str, start - str);
			rightList.push_back(tmp);
		} else if (start = strstr(str, outEnding)) {
			char *tmp = new char[start - str + 2];
			StrClear(tmp);
			StrCatN(tmp, str, start - str);
			outList.push_back(tmp);
		}
	}
	for (size_t i = 0; i < leftList.size(); i++) {
		size_t j, same = -1;
		for (size_t j = 0; i < rightList.size(); j++)
			if (StrCmp(leftList[i], rightList[j])) {
				same = j;
				break;
			}
		if (same == -1) {
			printf("Can't find second file for %s\n", leftList[i]);
			continue;
		}
		bool flag = 0;
		for (size_t j = 0; j < outList.size(); j++)
			if (StrCmp(leftList[i], outList[j])) {
				flag = 1;
				break;
			} else printf("%s != %s\n", leftList[i], outList[j]);
		if (flag) {
			printf("out file %s%s.%s found, no overriding\n", leftList[i], outEnding, format_out);
			delete leftList[i];
			leftList.erase(leftList.begin() + i) - 1;
			delete rightList[same];
			rightList.erase(rightList.begin() + same);
			delete outList[j];
			outList.erase(outList.begin() + j);
			i--;
			continue;
		}
//		printf("%s matches %s\n", leftList[i], rightList[same]);
		sprintf(str, "%s -i %s%s%s.%s -i %s%s%s.%s -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w [out]\" -map 0:a? -map [out] %s %s%s%s.%s",
	              ffpath, inDir, leftList[i], sbsr ? rightEnding : leftEnding, format_in, inDir, leftList[i], sbsr ? leftEnding : rightEnding, format_in, outParams, inDir, leftList[i], outEnding, format_out);
		printf("%s\n", str);
		system(str);
	}
	fclose(in);
	sprintf(str, "erase %s", dirFile);
	system(str);
	return 0;
}
