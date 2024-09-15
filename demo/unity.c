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
#include <pthread.h>

#include "gme/gme.h"

#include "unity.h"
static Music_Emu *emu = NULL;
static float volume = 1.0f;
static short rawSampleData[4096];
static FILE *debugOut = NULL;
static pthread_mutex_t global_mutex;

static gme_equalizer_t gme_equalizer_state = {0};

const char *NsfOpenFile(const char *path, int sampleRate)
{
	pthread_mutex_lock(&global_mutex);
	const char *result = gme_open_file(path, &emu, sampleRate);
	pthread_mutex_unlock(&global_mutex);
	return result;
}

int NsfActivateDebugOut(const char *path)
{
	pthread_mutex_lock(&global_mutex);
	debugOut = fopen("/tmp/gmeunity.raw", "wb");
	pthread_mutex_unlock(&global_mutex);
	return (debugOut != NULL);
}

const char *NsfOpenData(uint8_t *data, int dataLen, int sampleRate)
{
	pthread_mutex_lock(&global_mutex);
	const char *result = gme_open_data(data, dataLen, &emu, sampleRate);
	pthread_mutex_unlock(&global_mutex);
	return result;
}

const char *NsfPlayTrack(int index)
{
	const char *result = NULL;

	pthread_mutex_lock(&global_mutex);
	if (emu)
	{

		gme_info_t *info;
		gme_err_t err = gme_track_info(emu, &info, index);
		if (err)
		{
			printf("Err %s\n", err);
		}

		assert(info);
		fprintf(stderr, "Song: %s\n", info->song);
		fprintf(stderr, "System: %s\n", info->system);
		fprintf(stderr, "Lengths: %d %d %d\n", info->length, info->intro_length, info->loop_length);

		result = gme_start_track(emu, index);
	}
	pthread_mutex_unlock(&global_mutex);

	return result;
}

const char *NsfGetSamples(int numSamples, float *sampleData, int channels)
{
	const char *err = "FileNotOpened";
	if (emu)
	{
		pthread_mutex_lock(&global_mutex);
		err = gme_play(emu, numSamples, rawSampleData);
		pthread_mutex_unlock(&global_mutex);
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
			pthread_mutex_lock(&global_mutex);
			fwrite(sampleData, sizeof(float), outIndex, debugOut);
			pthread_mutex_unlock(&global_mutex);
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
	pthread_mutex_lock(&global_mutex);
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
	pthread_mutex_unlock(&global_mutex);
}

void NsfMuteVoices(int muting_mask)
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_mute_voices(emu, muting_mask);
	pthread_mutex_unlock(&global_mutex);
}

void NsfSetTempo(double tempo)
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_set_tempo(emu, tempo);
	pthread_mutex_unlock(&global_mutex);
}

void NsfSetEqualizer(double treble, double bass)
{
	pthread_mutex_lock(&global_mutex);
	gme_equalizer_state.bass = bass;
	gme_equalizer_state.treble = treble;
	if (emu)
		gme_set_equalizer(emu, &gme_equalizer_state);
	pthread_mutex_unlock(&global_mutex);
}

double NsfGetEqualizerTreble()
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_equalizer(emu, &gme_equalizer_state);
	pthread_mutex_unlock(&global_mutex);
	return gme_equalizer_state.treble;
}

double NsfGetEqualizerBass()
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_equalizer(emu, &gme_equalizer_state);
	pthread_mutex_unlock(&global_mutex);
	return gme_equalizer_state.bass;
}

/**
 * @brief Gets current position in played samples
 *
 * @return int position as played samples
 */
int NsfGetCurrentSamplePosition()
{
	int result = -1;

	pthread_mutex_lock(&global_mutex);
	if (emu)
		result = gme_tell_samples(emu);
	pthread_mutex_unlock(&global_mutex);

	return result;
}

void NsfSeekSamplePosition(int samples)
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_seek_samples(emu, samples);
	pthread_mutex_unlock(&global_mutex);
}

/**
 * @brief Gets current position in played milliseconds
 *
 * @return int played milliseconds since track start
 */
int NsfGetCurrentTimePosition()
{
	int result = -1;
	pthread_mutex_lock(&global_mutex);
	if (emu)
		return gme_tell(emu);
	pthread_mutex_unlock(&global_mutex);

	return result;
}

void NsfSeekTimePosition(int milliseconds)
{
	pthread_mutex_lock(&global_mutex);
	if (emu)
		gme_seek(emu, milliseconds);
	pthread_mutex_unlock(&global_mutex);
}
