/*
 * unity.h
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */

#ifndef DEMO_UNITY_H_
#define DEMO_UNITY_H_

#include <stdint.h>

#ifdef __WIN32
#define EXPORTFUNC __declspec(dllexport)
#else
#define EXPORTFUNC
#endif

EXPORTFUNC const char* NsfOpenFile(const char* path, int sampleRate);

EXPORTFUNC int NsfActivateDebugOut(const char* path);

EXPORTFUNC const char* NsfOpenData(uint8_t* data, int dataLen, int sampleRate);

EXPORTFUNC const char* NsfPlayTrack(int index);

EXPORTFUNC const char* NsfGetSamples(int numSamples, float* sampleData, int channels);

EXPORTFUNC void NsfSetVolume(float volume);

EXPORTFUNC void NsfClose();

EXPORTFUNC void NsfMuteVoices(int muting_mask);

// Set tempo, where 0.5 = half speed, 1.0 = normal, 2.0 = double speed
EXPORTFUNC void NsfSetTempo(double tempo);

// double treble; /* -50.0 = muffled, 0 = flat, +5.0 = extra-crisp */
// double bass;   /* 1 = full bass, 90 = average, 16000 = almost no bass */
EXPORTFUNC void NsfSetEqualizer(double treble, double bass);

EXPORTFUNC double NsfGetEqualizerTreble();

EXPORTFUNC double NsfGetEqualizerBass();


#endif /* DEMO_UNITY_H_ */
