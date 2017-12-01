#ifndef SONG_STDIO_PACKETQUEUE_H
#define SONG_STDIO_PACKETQUEUE_H

#include <pthread.h>
#include "./../libcommon/CommonTools.h"

typedef struct AudioPacket {

	static const int AUDIO_PACKET_ACTION_PLAY = 0;
	static const int AUDIO_PACKET_ACTION_PAUSE = 100;
	static const int AUDIO_PACKET_ACTION_SEEK = 101;

	short * buffer;
	int size;
	float position;
	int action;

	float extra_param1;
	float extra_param2;

	AudioPacket() {
		buffer = NULL;
		size = 0;
		position = -1;
		action = 0;
		extra_param1 = 0;
		extra_param2 = 0;
	}
	~AudioPacket() {
//		__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete AudioPacket");
		if (NULL != buffer) {
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "buffer's addr is %x", buffer);
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete buffer ...");
			delete[] buffer;
			buffer = NULL;
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete buffer success");
		}
//		__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete AudioPacket success");
	}
} AudioPacket;

typedef struct AudioPacketList {
	AudioPacket *pkt;
	struct AudioPacketList *next;
	AudioPacketList(){
		pkt = NULL;
		next = NULL;
	}
} AudioPacketList;
inline void buildPacketFromBuffer(AudioPacket * audioPacket, short* samples, int sampleSize) {
	short* packetBuffer = new short[sampleSize];
	if (NULL != packetBuffer) {
		memcpy(packetBuffer, samples, sampleSize * 2);
		audioPacket->buffer = packetBuffer;
		audioPacket->size = sampleSize;
	} else {
		audioPacket->size = -1;
	}
}
class PacketQueue {
public:
	PacketQueue();
	PacketQueue(const char* queueNameParam);
	~PacketQueue();

	void init();
	void flush();
	int put(AudioPacket* audioPacket);

	/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
	int get(AudioPacket **audioPacket, bool block);

	int size();

	void abort();

private:
	AudioPacketList* mFirst;
	AudioPacketList* mLast;
	int mNbPackets;
	bool mAbortRequest;
	pthread_mutex_t mLock;
	pthread_cond_t mCondition;
	const char* queueName;
};

#endif // SONG_STDIO_PACKETQUEUE_H
