#include <arpa/inet.h>
#include <endian.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "arg.h"

char * argv0;

enum {
	EXIT_WHEN_ERROR,
	CONTINUE_WHEN_ERROR,
};

const char * map[] = {
	"0000", "0001", "0010", "0011",
	"0100", "0101", "0110", "0111",

	"1000", "1001", "1010", "1011",
	"1100", "1101", "1110", "1111",
};

static
void
usage() {
	fprintf(stderr, "usage: %s [-fh]\n"
	"\n"
	"\t-f force continuation in case of an error\n"
	"\t-h this message\n", argv0);
	exit(1);
}

static
void
print_bitset_address(const void * address) {
	const unsigned char * bytes = address;
	for (int i = 0; i < 4; i++) {
		fwrite(map[bytes[i] >> 0x4], 1, 4, stdout);
		fwrite(map[bytes[i] &  0xf], 1, 4, stdout);
		if (i < 3)
			putchar('.');
	}
	putchar('\n');
}

int
main(int argc, char * argv[]) {
	char * line = NULL;
	size_t cap = 0;
	ssize_t len;
	uint32_t address;
	int force = EXIT_WHEN_ERROR;

	ARGBEGIN {
	case 'f':
		force = CONTINUE_WHEN_ERROR; 
		break;
	default:
		usage();
	} ARGEND;

	if (argc != 0)
		usage();

	while ((len = getline(&line, &cap, stdin)) > 0) {
		line[--len] = '\0';
		switch (inet_pton(AF_INET, line, &address)) {
		case 1:
			print_bitset_address(&address);
			break;
		case 0:
			fprintf(stderr, "Ivalid address: %s\n", line);
			if (force == EXIT_WHEN_ERROR)
				return 1;
			break;
		case -1:
		default:
			fprintf(stderr, "Error while processing %s\n", line);
			perror("");
			if (force == EXIT_WHEN_ERROR)
				return 2;
			break;
		}
	}
	free(line);
	return 0;
}
