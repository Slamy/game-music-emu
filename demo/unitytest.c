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

	fprintf(stderr, "bass=%f, treble=%f\n",NsfGetEqualizerBass(), NsfGetEqualizerTreble());

	err = NsfPlayTrack(track);
	assert(!err);

	fprintf(stderr, "bass=%f, treble=%f\n",NsfGetEqualizerBass(), NsfGetEqualizerTreble());

	// double treble; /* -50.0 = muffled, 0 = flat, +5.0 = extra-crisp */
	// double bass;   /* 1 = full bass, 90 = average, 16000 = almost no bass */

	double treble = NsfGetEqualizerTreble();
	double bass = NsfGetEqualizerBass();

	for(;;)
	{
		float samples[1024];

		err = NsfGetSamples(1024, samples,1);
		if (err)
			puts(err);

		assert(!err);
		fwrite(samples,1,1024*4,stdout);

		//treble-=0.2;
		//bass+=10;
		NsfSetEqualizer(treble, bass);

	}

	return 0;
}
