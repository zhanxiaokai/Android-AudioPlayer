#include "accompany_decoder_controller.h"

#define LOG_TAG "AccompanyDecoderController"
AccompanyDecoderController::AccompanyDecoderController() {
	accompanyDecoder = NULL;
	playPosition = 0.0f;
}

AccompanyDecoderController::~AccompanyDecoderController() {
}

int AccompanyDecoderController::getMusicMeta(const char* accompanyPath,
		int * accompanyMetaData) {
	//获取伴奏的meta
	AccompanyDecoder* accompanyDecoder = new AccompanyDecoder();
	accompanyDecoder->getMusicMeta(accompanyPath, accompanyMetaData);
	delete accompanyDecoder;
	//初始化伴奏的采样率
	accompanySampleRate = accompanyMetaData[0];
//	LOGI("accompanySampleRate is %d", accompanySampleRate);
	return 0;
}

float AccompanyDecoderController::getPlayPosition() {
	return playPosition;
}

void* AccompanyDecoderController::startDecoderThread(void* ptr) {
	LOGI("enter AccompanyDecoderController::startDecoderThread");
	AccompanyDecoderController* decoderController =
			(AccompanyDecoderController *) ptr;
	int getLockCode = pthread_mutex_lock(&decoderController->mLock);
	while (decoderController->isRunning) {
		decoderController->decodeSongPacket();
		if (decoderController->packetPool->geDecoderAccompanyPacketQueueSize() > QUEUE_SIZE_MAX_THRESHOLD) {
			pthread_cond_wait(&decoderController->mCondition,
					&decoderController->mLock);
		}
	}
	pthread_mutex_unlock(&decoderController->mLock);
}

void AccompanyDecoderController::initAccompanyDecoder(
		const char* accompanyPath) {
	//初始化两个decoder
	accompanyDecoder = new AccompanyDecoder();
	accompanyDecoder->init(accompanyPath, accompanyPacketBufferSize);
}

void AccompanyDecoderController::init(const char* accompanyPath,
		float packetBufferTimePercent) {
	//初始化两个全局变量
	volume = 1.0f;
	accompanyMax = 1.0f;

	//计算计算出伴奏和原唱的bufferSize
	int accompanyByteCountPerSec = accompanySampleRate * CHANNEL_PER_FRAME
			* BITS_PER_CHANNEL / BITS_PER_BYTE;
	accompanyPacketBufferSize = (int) ((accompanyByteCountPerSec / 2)
			* packetBufferTimePercent);
	//初始化两个decoder
	initAccompanyDecoder(accompanyPath);
	//初始化队列以及开启线程
	packetPool = PacketPool::GetInstance();
	packetPool->initDecoderAccompanyPacketQueue();
	initDecoderThread();
}

void AccompanyDecoderController::initDecoderThread() {
	isRunning = true;
	pthread_mutex_init(&mLock, NULL);
	pthread_cond_init(&mCondition, NULL);
	pthread_create(&songDecoderThread, NULL, startDecoderThread, this);
}

//需要子类完成自己的操作
void AccompanyDecoderController::decodeSongPacket() {
	AudioPacket* accompanyPacket = accompanyDecoder->decodePacket();
	accompanyPacket->action = AudioPacket::AUDIO_PACKET_ACTION_PLAY;
	packetPool->pushDecoderAccompanyPacketToQueue(accompanyPacket);
}

void AccompanyDecoderController::destroyDecoderThread() {
	isRunning = false;
	void* status;
	int getLockCode = pthread_mutex_lock(&mLock);
	pthread_cond_signal(&mCondition);
	pthread_mutex_unlock(&mLock);
	pthread_join(songDecoderThread, &status);
	pthread_mutex_destroy(&mLock);
	pthread_cond_destroy(&mCondition);
}

int AccompanyDecoderController::readSamples(short* samples, int size,
		int* slientSizeArr) {
	int result = -1;
	AudioPacket* accompanyPacket = NULL;
	packetPool->getDecoderAccompanyPacket(&accompanyPacket, true);
	if (NULL != accompanyPacket) {
		int samplePacketSize = accompanyPacket->size;
		if (samplePacketSize != -1 && samplePacketSize <= size) {
			//copy the raw data to samples
			memcpy(samples, accompanyPacket->buffer, samplePacketSize * 2);
			adjustSamplesVolume(samples, samplePacketSize,
					volume / accompanyMax);
			delete accompanyPacket;
			result = samplePacketSize;
		}
	} else {
		result = -2;
	}
	if (packetPool->geDecoderAccompanyPacketQueueSize() < QUEUE_SIZE_MIN_THRESHOLD) {
		int getLockCode = pthread_mutex_lock(&mLock);
		if (result != -1) {
			pthread_cond_signal(&mCondition);
		}
		pthread_mutex_unlock (&mLock);
	}
	return result;
}

void AccompanyDecoderController::destroy() {
	destroyDecoderThread();
	packetPool->abortDecoderAccompanyPacketQueue();
	packetPool->destoryDecoderAccompanyPacketQueue();
	if (NULL != accompanyDecoder) {
		accompanyDecoder->destroy();
		delete accompanyDecoder;
		accompanyDecoder = NULL;
	}
}
