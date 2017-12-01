#include "packet_pool.h"

#define LOG_TAG "SongStudio PacketPool"

PacketPool::PacketPool() {
}

PacketPool::~PacketPool() {
}
//初始化静态成员
PacketPool* PacketPool::instance = new PacketPool();
PacketPool* PacketPool::GetInstance() {
	return instance;
}

/**************************start decoder original song packet queue process*******************************************/
void PacketPool::initDecoderOriginalSongPacketQueue() {
	const char* name = "decoder original song packet queue";
	decoderOriginalSongPacketQueue = new PacketQueue(name);
}

void PacketPool::abortDecoderOriginalSongPacketQueue() {
	if(NULL != decoderOriginalSongPacketQueue){
		decoderOriginalSongPacketQueue->abort();
	}
}

void PacketPool::destoryDecoderOriginalSongPacketQueue() {
	if(NULL != decoderOriginalSongPacketQueue){
		delete decoderOriginalSongPacketQueue;
		decoderOriginalSongPacketQueue = NULL;
	}
}

int PacketPool::getDecoderOriginalSongPacket(AudioPacket **audioPacket, bool block) {
	int result = -1;
	if(NULL != decoderOriginalSongPacketQueue){
		result = decoderOriginalSongPacketQueue->get(audioPacket, block);
	}
	return result;
}

int PacketPool::getDecoderOriginalSongPacketQueueSize() {
	return decoderOriginalSongPacketQueue->size();
}

void PacketPool::pushDecoderOriginalSongPacketToQueue(AudioPacket* audioPacket) {
	decoderOriginalSongPacketQueue->put(audioPacket);
}

void PacketPool::clearDecoderOriginalSongPacketToQueue() {
	decoderOriginalSongPacketQueue->flush();
}

/**************************end decoder original song packet queue process*******************************************/

/**************************start decoder accompany packet queue process*******************************************/
void PacketPool::initDecoderAccompanyPacketQueue() {
	const char* name = "decoder accompany packet queue";
	decoderAccompanyPacketQueue = new PacketQueue(name);
}

void PacketPool::abortDecoderAccompanyPacketQueue() {
	if(NULL != decoderAccompanyPacketQueue){
		decoderAccompanyPacketQueue->abort();
	}
}

void PacketPool::destoryDecoderAccompanyPacketQueue() {
	if(NULL != decoderAccompanyPacketQueue){
		delete decoderAccompanyPacketQueue;
		decoderAccompanyPacketQueue = NULL;
	}
}

int PacketPool::getDecoderAccompanyPacket(AudioPacket **audioPacket, bool block) {
	int result = -1;
	if(NULL != decoderAccompanyPacketQueue){
		result = decoderAccompanyPacketQueue->get(audioPacket, block);
	}
	return result;
}

int PacketPool::geDecoderAccompanyPacketQueueSize() {
	return decoderAccompanyPacketQueue->size();
}

void PacketPool::clearDecoderAccompanyPacketToQueue() {
	decoderAccompanyPacketQueue->flush();
}

void PacketPool::pushDecoderAccompanyPacketToQueue(AudioPacket* audioPacket) {
	decoderAccompanyPacketQueue->put(audioPacket);
}
/**************************end decoder accompany packet queue process*******************************************/

/**************************start audio packet queue process**********************************************/
void PacketPool::initAudioPacketQueue() {
//	LOGI("PacketPool::initAudioPacket queue");
	const char* name = "audioPacket queue";
	audioPacketQueue = new PacketQueue(name);
}
void PacketPool::abortAudioPacketQueue() {
//	LOGI("PacketPool::abortAudioPacketQueue queue");
	if(NULL != audioPacketQueue){
//		LOGI("PacketPool::abortAudioPacketQueue queue");
		audioPacketQueue->abort();
	}
}
void PacketPool::destoryAudioPacketQueue() {
//	LOGI("PacketPool::destoryAudioPacket queue");
	if(NULL != audioPacketQueue){
		delete audioPacketQueue;
		audioPacketQueue = NULL;
	}
}

int PacketPool::getAudioPacket(AudioPacket **audioPacket, bool block) {
//	LOGI("PacketPool::getAudioPacket");
	int result = -1;
	if(NULL != audioPacketQueue){
		result = audioPacketQueue->get(audioPacket, block);
	}
	return result;
}

int PacketPool::getAudioPacketQueueSize() {
	return audioPacketQueue->size();
}

void PacketPool::pushAudioPacketToQueue(AudioPacket* audioPacket) {
//	LOGI("PacketPool::pushAudioPacketToQueue");
	audioPacketQueue->put(audioPacket);
}

void PacketPool::clearAudioPacketToQueue() {
	audioPacketQueue->flush();
}
/**************************end audio packet queue process**********************************************/

/**************************start accompany packet queue process****************************************/
void PacketPool::initAccompanyPacketQueue() {
//	LOGI("PacketPool::initAccompanyPacket queue");
	const char* name = "accompanyPacket queue";
	accompanyPacketQueue = new PacketQueue(name);
}
void PacketPool::abortAccompanyPacketQueue() {
//	LOGI("PacketPool::abortAccompanyPacket queue");
	if(NULL != accompanyPacketQueue){
//		LOGI("PacketPool::abortAccompanyPacket queue");
		accompanyPacketQueue->abort();
	}
}
void PacketPool::destoryAccompanyPacketQueue() {
//	LOGI("PacketPool::destoryAccompanyPacket queue");
	if(NULL != accompanyPacketQueue){
		delete accompanyPacketQueue;
		accompanyPacketQueue = NULL;
	}
}

int PacketPool::getAccompanyPacket(AudioPacket **accompanyPacket, bool block) {
//	LOGI("PacketPool::getAccompanyPacket");
	int result = -1;
	if(NULL != accompanyPacketQueue){
		result = accompanyPacketQueue->get(accompanyPacket, block);
	}
	return result;
}

int PacketPool::getAccompanyPacketQueueSize() {
	return accompanyPacketQueue->size();
}

void PacketPool::pushAccompanyPacketToQueue(AudioPacket* accompanyPacket) {
//	LOGI("PacketPool::pushAccompanyPacketToQueue");
	accompanyPacketQueue->put(accompanyPacket);
}

void PacketPool::clearAccompanyPacketQueue() {
	if (accompanyPacketQueue != NULL) {
		accompanyPacketQueue->flush();
	}
}
/**************************end accompany packet queue process****************************************/

/**************************start live packet queue process*******************************************/
void PacketPool::initLivePacketQueue() {
//	LOGI("PacketPool::initLivePacket queue");
	const char* name = "livePacket queue";
	livePacketQueue = new PacketQueue(name);
}
void PacketPool::abortLivePacketQueue() {
//	LOGI("PacketPool::abortLivePacket queue");
	if(NULL != livePacketQueue){
		livePacketQueue->abort();
	}
}
void PacketPool::destoryLivePacketQueue() {
//	LOGI("PacketPool::destoryLivePacket queue");
	if(NULL != livePacketQueue){
		delete livePacketQueue;
		livePacketQueue = NULL;
	}
}

int PacketPool::getLivePacket(AudioPacket **livePacket, bool block) {
//	LOGI("PacketPool::getLivePacket");
	int result = -1;
	if(NULL != livePacketQueue){
		result = livePacketQueue->get(livePacket, block);
	}
	return result;
}

int PacketPool::getLivePacketQueueSize() {
	int result = -1;
	if(NULL != livePacketQueue){
		result = livePacketQueue->size();
	}
	return result;
}

void PacketPool::pushLivePacketToQueue(AudioPacket* livePacket) {
//	LOGI("PacketPool::pushLivePacketToQueue");
	livePacketQueue->put(livePacket);
}
/**************************end live packet queue process*******************************************/

/**************************start LiveSubscriber packet queue process*******************************************/
void PacketPool::initLiveSubscriberPacketQueue() {
//	LOGI("PacketPool::initLiveSubscriberPacket queue");
	const char* name = "liveSubscriberPacket queue";
	liveSubscriberPacketQueue = new PacketQueue(name);
}
void PacketPool::abortLiveSubscriberPacketQueue() {
//	LOGI("PacketPool::abortLiveSubscriberPacket queue");
	if(NULL != liveSubscriberPacketQueue){
//		LOGI("PacketPool::abortLiveSubscriberPacket queue");
		liveSubscriberPacketQueue->abort();
	}
}
void PacketPool::destoryLiveSubscriberPacketQueue() {
//	LOGI("PacketPool::destoryLiveSubscriberPacket queue");
	if(NULL != liveSubscriberPacketQueue){
		delete liveSubscriberPacketQueue;
		liveSubscriberPacketQueue = NULL;
	}
}

int PacketPool::getLiveSubscriberPacket(AudioPacket **livePacket, bool block) {
//	LOGI("PacketPool::getLivePacket");
	int result = -1;
	if(NULL != liveSubscriberPacketQueue){
		result = liveSubscriberPacketQueue->get(livePacket, block);
	}
	return result;
}

int PacketPool::getLiveSubscriberPacketQueueSize() {
	return liveSubscriberPacketQueue->size();
}

void PacketPool::pushLiveSubscriberPacketToQueue(AudioPacket* livePacket) {
//	LOGI("PacketPool::pushLivePacketToQueue");
	liveSubscriberPacketQueue->put(livePacket);
}
/**************************end LiveSubscriber packet queue process*******************************************/

/**************************start Tuning packet queue process*******************************************/
void PacketPool::initTuningPacketQueue() {
//	LOGI("PacketPool::initTuningPacket queue");
	const char* name = "tuningPacket queue";
	tuningPacketQueue = new PacketQueue(name);
}
void PacketPool::abortTuningPacketQueue() {
//	LOGI("PacketPool::abortTuningPacket queue");
	if(NULL != tuningPacketQueue){
		tuningPacketQueue->abort();
	}
}
void PacketPool::destoryTuningPacketQueue() {
//	LOGI("PacketPool::destoryTuningPacket queue");
	if(NULL != tuningPacketQueue){
		delete tuningPacketQueue;
		tuningPacketQueue = NULL;
	}
}

int PacketPool::getTuningPacket(AudioPacket **tuningPacket, bool block) {
//	LOGI("PacketPool::getLivePacket");
	int result = -1;
	if(NULL != tuningPacketQueue){
		result = tuningPacketQueue->get(tuningPacket, block);
	}
	return result;
}

int PacketPool::getTuningPacketQueueSize() {
	return tuningPacketQueue->size();
}

void PacketPool::pushTuningPacketToQueue(AudioPacket* livePacket) {
//	LOGI("PacketPool::pushLivePacketToQueue");
	tuningPacketQueue->put(livePacket);
}
/**************************end Tuning packet queue process*******************************************/

