/*
 * unitytest.c
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */


#include "unity.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t* dump_file(const char*file_path, size_t *size)
{
	FILE *in = fopen(file_path, "rb");
	uint8_t *buffer = NULL;
	if (!in)
		return NULL;
	fseek(in, 0, SEEK_END);
	(*size) = (size_t)ftell(in);
	fseek(in, 0, SEEK_SET);
	buffer = (uint8_t*)malloc(*size);
	fread(buffer, 1, *size, in);
	return buffer;
}

int main(int argc, char *argv[])
{
	const char *filename = "test.nsf"; /* Default file to open */
	if ( argc >= 2 )
		filename = argv[1];

	int sample_rate = 44100; /* number of samples per second */
	/* index of track to play (0 = first) */
	int track = argc >= 3 ? atoi(argv[2]) : 0;

	const char* err;
#if 0
	err = NsfOpenFile(filename, sample_rate);
	assert(!err);
#else
	size_t file_size = 0;
	uint8_t *file_data = dump_file(filename, &file_size);
	err=NsfOpenData(file_data, file_size, sample_rate);
	assert(!err);
#endif

	err = NsfPlayTrack(track);
	assert(!err);

	for(;;)
	{
		float samples[1024];

		err = NsfGetSamples(1024, samples,1);
		assert(!err);
		fwrite(samples,1,1024*4,stdout);
	}

	return 0;
}
