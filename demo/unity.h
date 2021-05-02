/*
 * unity.h
 *
 *  Created on: 02.05.2021
 *      Author: andre
 */

#ifndef DEMO_UNITY_H_
#define DEMO_UNITY_H_

const char* NsfOpenFile(const char* path, int sampleRate);

const char* NsfPlayTrack(int index);

const char* NsfGetSamples(int numSamples, float* sampleData);

void NsfSetVolume(float volume);

void NsfClose();

#endif /* DEMO_UNITY_H_ */
