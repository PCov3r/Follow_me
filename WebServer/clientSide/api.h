#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curl/curl.h>

char* activateOverride(void);
char* sendCmd(int, int, int);
size_t write_callback_func(void, size_t, size_t, void);
