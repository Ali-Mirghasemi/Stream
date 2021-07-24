#include <stdio.h>
#include <stdlib.h>

#include "../../Src/Stream.h"

static const char CRLF[2] = "\r\n";

static const char TEMPLATE_1[] = "Line1\n-Line2-\n--Line3--\nLine4\n";

static const char TEMPLATE_2[] = "Part 1\n\tA\r\nPart 2\n\tABCD\r\nPart 3\n\t--X--\r\nPart 4\r\n";

void showLines(Stream* stream);
void showParts(Stream* stream, uint8_t* pat, int patLen);

int main()
{
    uint8_t buff[100];
    Stream stream;

    Stream_init(&stream, buff, sizeof(buff));

    // push bytes
    printf("TXT: %s", TEMPLATE_1);
    Stream_writeBytes(&stream, (uint8_t*) TEMPLATE_1, sizeof(TEMPLATE_1) - 1);
    showLines(&stream);

    printf("-----------------------------------\n");
    printf("TXT: %s", TEMPLATE_2);
    Stream_writeBytes(&stream, (uint8_t*) TEMPLATE_2, sizeof(TEMPLATE_2) - 1);
    showParts(&stream, CRLF, sizeof(CRLF));

}

void showLines(Stream* stream) {
	int len;
	char temp[30];
    printf("Parse Stream, ReadLine, Avl: %d\n", Stream_available(stream));
	while ((len = Stream_readBytesUntil(stream, '\n', (uint8_t*) temp, sizeof(temp)))) {
		temp[len] = 0;
		printf("%2d) %s\n", len, temp);
	}
}

void showParts(Stream* stream, uint8_t* pat, int patLen) {
	int len;
	char temp[30];
    printf("Parse Stream, ReadPart, Avl: %d\n", Stream_available(stream));
	while ((len = Stream_readBytesUntilPattern(stream, pat, patLen, (uint8_t*) temp, sizeof(temp)))) {
		temp[len] = 0;
		printf("%2d) %s\n", len, temp);
	}
}
