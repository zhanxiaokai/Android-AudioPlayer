package com.phuket.tour.audioplayer.audiotrack;

public class MusicDecoder implements Mp3Decoder {

	@Override
	public void init(String accompanyPath, float packetBufferTimePercent) {
		openFile(accompanyPath, packetBufferTimePercent);
	}

	@Override
	public void destory() {
		closeFile();
	}

	@Override
	public int readSamples(short[] samples, int[] slientSizeArr) {
		return readSamples(samples, samples.length, slientSizeArr);
	}
	
	@Override
	public int getMusicMetaByPath(String accompanyPath, int[] metaArray) {
		return getMusicMeta(accompanyPath, metaArray);
	}
	
	/** 第一步进行获取伴奏文件的meta信息，一个是采样率一个是比特率 **/
	private native int getMusicMeta(String musicPath, int[] metaArray);
	/** 最开始init的时候进行打开伴奏文件 **/
	private native int openFile(String accompanyPath, float packetBufferTimePercent);
	/** 打开文件之后进行读取samples处理 **/
	private native int readSamples(short[] samples, int size, int[] slientSizeArr);
	/** 最终当歌曲结束的时候 关闭文件  **/
	private native void closeFile();

}
