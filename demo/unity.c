/*
 * unity.c
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */

#ifndef DEMO_UNITY_C_
#define DEMO_UNITY_C_

#endif /* DEMO_UNITY_C_ */

#include "limits.h"
#include "stdlib.h"
#include "gme/gme.h"

static Music_Emu* emu;
static float volume = 1.0f;
static short rawSampleData[4096];

const char* NsfOpenFile(const char* path, int sampleRate) {
	return gme_open_file(path, &emu, sampleRate);
}

const char* NsfPlayTrack(int index) {
	return gme_start_track(emu, index);
}

const char* NsfGetSamples(int numSamples, float* sampleData) {
	const char* err=gme_play(emu, numSamples, rawSampleData);

	if (err == NULL)
	{
		for (int i =0;i< numSamples; i++)
		{
			sampleData[i]=rawSampleData[i]*volume/((float)SHRT_MAX);
		}
	}

	return err;
}

void NsfSetVolume(float val) {
	volume = val;
}

void NsfClose() {
	gme_delete(emu);
}
