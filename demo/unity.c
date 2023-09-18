/*
 * unity.c
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */

#ifndef DEMO_UNITY_C_
#define DEMO_UNITY_C_

#endif /* DEMO_UNITY_C_ */

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "gme/gme.h"

#include "unity.h"

static Music_Emu *emu = NULL;
static float volume = 1.0f;
static short rawSampleData[4096];
static FILE *debugOut = NULL;

static gme_equalizer_t gme_equalizer_state = {0};

const char *NsfOpenFile(const char *path, int sampleRate)
{
	return gme_open_file(path, &emu, sampleRate);
}

int NsfActivateDebugOut(const char *path)
{
	debugOut = fopen("/tmp/gmeunity.raw", "wb");
	return (debugOut != NULL);
}

const char *NsfOpenData(uint8_t *data, int dataLen, int sampleRate)
{
	return gme_open_data(data, dataLen, &emu, sampleRate);
}

const char *NsfPlayTrack(int index)
{
	if (emu)
	{
		gme_info_t *info;
		gme_err_t err = gme_track_info(emu, &info, index);
		if (err){
			printf("Err %d\n",err);
		}

		assert(info);
		fprintf(stderr,"Song: %s\n",info->song);
		fprintf(stderr,"System: %s\n",info->system);
		fprintf(stderr,"Lengths: %d %d %d\n",info->length, info->intro_length, info->loop_length);

		return gme_start_track(emu, index);
	}
	else
		return NULL;
}

const char *NsfGetSamples(int numSamples, float *sampleData, int channels)
{
	const char *err = "FileNotOpened";
	if (emu)
	{
		err = gme_play(emu, numSamples, rawSampleData);
		int outIndex = 0;

		if (err == NULL)
		{
			for (int i = 0; i < numSamples; i++)
			{
				for (int j = 0; j < channels; j++)
				{
					sampleData[outIndex++] = rawSampleData[i] * volume / ((float)SHRT_MAX);
				}
			}
		}

		if (debugOut)
		{
			fwrite(sampleData, sizeof(float), outIndex, debugOut);
		}
	}

	return err;
}

void NsfSetVolume(float val)
{
	volume = val;
}

void NsfClose()
{
	if (emu)
	{
		if (debugOut)
		{
			fclose(debugOut);
			debugOut = NULL;
		}
		gme_delete(emu);
		emu = NULL;
	}
}

void NsfMuteVoices(int muting_mask)
{
	if (emu)
		gme_mute_voices(emu, muting_mask);
}

void NsfSetTempo(double tempo)
{
	if (emu)
		gme_set_tempo(emu, tempo);
}

void NsfSetEqualizer(double treble, double bass)
{
	gme_equalizer_state.bass = bass;
	gme_equalizer_state.treble = treble;
	if (emu)
		gme_set_equalizer(emu, &gme_equalizer_state);
}

double NsfGetEqualizerTreble()
{
	if (emu)
		gme_equalizer(emu, &gme_equalizer_state);
	return gme_equalizer_state.treble;
}

double NsfGetEqualizerBass()
{
	if (emu)
		gme_equalizer(emu, &gme_equalizer_state);
	return gme_equalizer_state.bass;
}

/**
 * @brief Gets current position in played samples
 *
 * @return int position as played samples
 */
int NsfGetCurrentSamplePosition()
{
	if (emu)
		return gme_tell_samples(emu);

	return -1;
}

void NsfSeekSamplePosition(int samples)
{
	if (emu)
		gme_seek_samples(emu, samples);
}

/**
 * @brief Gets current position in played milliseconds
 *
 * @return int played milliseconds since track start
 */
int NsfGetCurrentTimePosition()
{
	if (emu)
		return gme_tell(emu);

	return -1;
}

void NsfSeekTimePosition(int milliseconds)
{
	if (emu)
		gme_seek(emu, milliseconds);
}
