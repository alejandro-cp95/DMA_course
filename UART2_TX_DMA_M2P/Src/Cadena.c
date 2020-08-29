/*
 * Cadena.c
 *
 *  Created on: 04/08/2020
 *      Author: -_Alex_-
 */

#include <stdio.h>
#include <string.h>

const char cadena[]=
{
	"A same peripheral request can be assigned to two different channels only if the application \
ensures that these channels are not requested to be served at the same time. In other \
words, if two different channels receive a same asserted peripheral request at the same \
time, an unpredictable DMA hardware behavior occurs.\r\n\n"
};

size_t length = strlen(cadena);
