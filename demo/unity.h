/*
 * unity.h
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */

#ifndef DEMO_UNITY_H_
#define DEMO_UNITY_H_

#include <stdint.h>

const char* NsfOpenFile(const char* path, int sampleRate);

int NsfActivateDebugOut(const char* path);

const char* NsfOpenData(uint8_t* data, int dataLen, int sampleRate);

const char* NsfPlayTrack(int index);

const char* NsfGetSamples(int numSamples, float* sampleData, int channels);

void NsfSetVolume(float volume);

void NsfClose();

#endif /* DEMO_UNITY_H_ */
