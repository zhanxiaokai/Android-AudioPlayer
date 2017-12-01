#ifndef ACCOMPANY_DECODER_CONTROLLER_H
#define ACCOMPANY_DECODER_CONTROLLER_H

#include <unistd.h>
#include "accompany_decoder.h"
#include <pthread.h>

#define CHANNEL_PER_FRAME	2
#define BITS_PER_CHANNEL		16
#define BITS_PER_BYTE		8
/** decode data to queue and queue size **/
#define QUEUE_SIZE_MAX_THRESHOLD 25
#define QUEUE_SIZE_MIN_THRESHOLD 20

/**
 * 这是伴奏单独的控制类，原唱的控制类继承自这个类
 */
class AccompanyDecoderController {
protected:
	/** 本身这个类也是一个生产者，每次客户端读出一个packet去我们就会把伴奏的packet给放入队列中，把原唱的packet销毁掉 **/
	PacketPool* packetPool;

	/** 伴奏的解码器 **/
	AccompanyDecoder* accompanyDecoder;

	/** 伴奏和原唱的采样频率与解码伴奏和原唱的每个packet的大小 **/
	int accompanySampleRate;
	int accompanyPacketBufferSize;

	/** 由于可能要进行音量的变化，这个是控制音量的值 **/
	float volume;
	/** 7.0新增**/
	float accompanyMax;

	/** 解码线程，主要是调用原唱和伴唱的解码器 解码出对应的packet数据来 **/
	pthread_t songDecoderThread;
	static void* startDecoderThread(void* ptr);
	/** 开启解码线程 **/
	virtual void initDecoderThread();
	virtual void decodeSongPacket();
	/** 销毁解码线程 **/
	virtual void destroyDecoderThread();
public:

	/** 由于在解码线程中要用到以下几个值，所以访问控制符是public的 **/
	bool isRunning;
	pthread_mutex_t mLock;
	pthread_cond_t mCondition;

	AccompanyDecoderController();
	~AccompanyDecoderController();

	/** 根据伴奏文件与原唱文件得到采样率以及比特率赋值给全局变量，并且将伴奏文件的采样率与比特率放入metaData返回 **/
	virtual int getMusicMeta(const char* accompanyPath, const char* originalSongPath, int * accompanyMetaData){};

	/** 初始两个decoder,并且根据上一步算出的采样率，计算出伴奏和原唱的bufferSize **/
	virtual void init(const char* accompanyPath, const char* originalSongPath, float packetBufferTimePercent){};

	/** 根据伴奏文件与原唱文件得到采样率以及比特率赋值给全局变量，并且将伴奏文件的采样率与比特率放入metaData返回 **/
	virtual int getMusicMeta(const char* accompanyPath, int * accompanyMetaData);

	/** 初始两个decoder,并且根据上一步算出的采样率，计算出伴奏和原唱的bufferSize **/
	virtual void init(const char* accompanyPath, float packetBufferTimePercent);

	/** 读取samples,返回实际读取的sample的个数 并且将accompanyPacket放入到accompanyQueue中，如果有originalSongPacket销毁 **/
	virtual int readSamples(short* samples, int size, int* slientSizeArr);

	/** 设置伴奏或者原唱的音量 **/
	void setVolume(float volumeParam, float accompanyMaxParam) {
		volume = volumeParam;
		accompanyMax = accompanyMaxParam;
	}

	virtual float getPlayPosition();
	/** 销毁这个controller **/
	virtual void destroy();

protected:
	float playPosition;

	virtual void initAccompanyDecoder(const char* accompanyPath);
};
#endif //ACCOMPANY_DECODER_CONTROLLER_H
