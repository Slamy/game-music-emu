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

#endif /* DEMO_UNITY_H_ */
