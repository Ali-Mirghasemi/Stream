#include <stdio.h>
#include <stdlib.h>

#include "StreamBuffer.h"

static const char CRLF[2] = "\r\n";

static const char TEMPLATE_1[] = "Line1\n-Line2-\n--Line3--\nLine4\n";

static const char TEMPLATE_2[] = "Part 1\n\tA\r\nPart 2\n\tABCD\r\nPart 3\n\t--X--\r\nPart 4\r\n";

void showLines(StreamBuffer* stream);
void showParts(StreamBuffer* stream, uint8_t* pat, int patLen);

int main()
{
    uint8_t buff[100];
    StreamBuffer stream;

    Stream_init(&stream, buff, sizeof(buff));

    // push bytes
    printf("TXT: %s", TEMPLATE_1);
    Stream_writeBytes(&stream, (uint8_t*) TEMPLATE_1, sizeof(TEMPLATE_1) - 1);
    showLines(&stream);

    printf("-----------------------------------\n");
    printf("TXT: %s", TEMPLATE_2);
    Stream_writeBytes(&stream, (uint8_t*) TEMPLATE_2, sizeof(TEMPLATE_2) - 1);
    showParts(&stream, (uint8_t*) CRLF, sizeof(CRLF));

}

void showLines(StreamBuffer* stream) {
	int len;
	char temp[30];
    printf("Parse StreamBuffer, ReadLine, Avl: %d\n", Stream_available(stream));
	while ((len = Stream_readBytesUntil(stream, '\n', (uint8_t*) temp, sizeof(temp)))) {
		temp[len] = 0;
		printf("%2d) %s\n", len, temp);
	}
}

void showParts(StreamBuffer* stream, uint8_t* pat, int patLen) {
	int len;
	char temp[30];
    printf("Parse StreamBuffer, ReadPart, Avl: %d\n", Stream_available(stream));
	while ((len = Stream_readBytesUntilPattern(stream, pat, patLen, (uint8_t*) temp, sizeof(temp)))) {
		temp[len] = 0;
		printf("%2d) %s\n", len, temp);
	}
}
