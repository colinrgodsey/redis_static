#include "redis.h"

//These are stub functions that are not used in the static library.
//These are the ones you're gonna want to hijack.

int dsRdbSaveBackground(char *filename) {
	return redis_dsRdbSaveBackground(filename);
}

int main(int argc, char **argv) {
	return redis_main(argc, argv);
}
