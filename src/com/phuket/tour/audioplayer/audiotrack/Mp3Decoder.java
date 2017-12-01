package com.phuket.tour.audioplayer.audiotrack;

public interface Mp3Decoder {

	public void init(String accompanyPath, float packetBufferTimePercent);

	public void destory();

	public int readSamples(short[] samples, int[] slientSizeArr);

	public int getMusicMetaByPath(String musicPath, int[] metaArray);
}
