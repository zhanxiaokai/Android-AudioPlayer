package com.phuket.tour.audioplayer.audiotrack;

import java.io.File;

import android.media.AudioFormat;
import android.media.AudioTrack;
import android.util.Log;

public class NativeMp3Player {

	public static final String TAG = "NativeMp3Player";
	private static final int BITS_PER_BYTE = 8;
	private static final int BITS_PER_CHANNEL = 16;
	private static final int CHANNEL_PER_FRAME = 2;
	private Mp3Decoder decoder;

	private AudioTrack audioTrack;
	/** 用于在暂停时候，播放空数据 **/

	/** default mp3's bitRate is 64kbps **/
	private int bitRate = 64000;
	/** mp3's capacityPerSec default is 8000B **/
	private int mp3CapacityPerSec = 8000;
	private long totalCapacity = -1;
	/** default mp3's decode buffer is 2 times than play buffer **/
	private static int DECODE_BUFFER_SIZE = 16 * 1024;
	private int sampleRateInHz = -1;
	private static int channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
	private static int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
	private int audioStreamType = -1;
	private int duration = 0;
	private Thread playerThread;

	/**
	 * 播放状态
	 */
	private volatile boolean isStop = false;
	private volatile boolean isPlaying = false;

	/** 用于标识seek到的时间 **/
	private volatile int seekBaseMillsTime = 0;
	private volatile long audioTrackBaseHeadPosition = 0;
	private volatile long pausedTimeMills = 0;

	public void setAudioStreamType(int audioStreamType) {
		this.audioStreamType = audioStreamType;
	}

	/** 初始化mp3 解码器以及初始化一些metaData **/
	public boolean setDataSource(String path) {
		decoder = new MusicDecoder();
		boolean result = initMetaData(path);
		if (result) {
			decoder.init(path, 0.2f);
		}
		return result;
	}

	private boolean initMetaData(String path) {
		int[] metaArray = new int[] { 0, 0 };
		decoder.getMusicMetaByPath(path, metaArray);
		this.sampleRateInHz = metaArray[0];
		this.bitRate = metaArray[1];
		if (sampleRateInHz <= 0 || bitRate <= 0) {
			return false;
		}
		totalCapacity = (new File(path)).length();
		mp3CapacityPerSec = bitRate / BITS_PER_BYTE;
		if (mp3CapacityPerSec == 0) {
			return false;
		}
		duration = (int) (totalCapacity / mp3CapacityPerSec);
		int byteCountPerSec = sampleRateInHz * CHANNEL_PER_FRAME * BITS_PER_CHANNEL / BITS_PER_BYTE;
		DECODE_BUFFER_SIZE = (int) ((byteCountPerSec / 2) * 0.2);

		initPlayState();

		seekBaseMillsTime = 0;
		audioTrackBaseHeadPosition = 0;

		return true;
	}

	/** 开始播放线程 **/
	public void prepare() {
		initPlayState();
		initAudioTrack();
		startPlayerThread();
	}

	private void initPlayState() {
		isPlaying = false;
		isStop = false;
	}

	private void startPlayerThread() {
		playerThread = new Thread(new PlayerThread(), "NativeMp3PlayerThread");
		playerThread.start();
	}

	private void initAudioTrack() {
		int bufferSize = AudioTrack.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat);
		audioTrack = new AudioTrack(this.audioStreamType, sampleRateInHz, channelConfig, audioFormat, bufferSize,
				AudioTrack.MODE_STREAM);
	}

	public void start() {
		synchronized (NativeMp3Player.class) {
			try {
				if (null != audioTrack) {
					audioTrack.play();
				}
			} catch (Throwable t) {
			}
			isPlaying = true;
		}
	}

	public void stop() {
		if (!isStop && null != audioTrack) {
			if (null != audioTrack && audioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				try {
					audioTrack.stop();
				} catch (Throwable t) {
					t.printStackTrace();
				}
			}
			isPlaying = true;
			isStop = true;
			try {
				Log.i(TAG, "join decodeMp3Thread....");
				if (null != playerThread) {
					playerThread.join();
					playerThread = null;
				}
				Log.i(TAG, " decodeMp3Thread ended....");
			} catch (Throwable t) {
				t.printStackTrace();
			}
			closeAudioTrack();
			destroy();
		}
	}

	public void destroy() {
		playerThread = null;
		audioTrack = null;
	}

	private void closeAudioTrack() {
		try {
			if (null != audioTrack && audioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				audioTrack.release();
			}
		} catch (Throwable t) {
			t.printStackTrace();
		}
	}

	class PlayerThread implements Runnable {
		private short[] samples;

		@Override
		public void run() {
			int sample_count = 0;
			boolean isPlayTemp = isPlaying = false;
			try {
				// 注意这里是2s的数据，因为decodeBufferSize是1秒钟wav的字节数，这里作用到short的数目，则是2秒的数据
				samples = new short[DECODE_BUFFER_SIZE];
				short[] fakeSamples = new short[DECODE_BUFFER_SIZE];
				int[] extraSlientSampleSize = new int[1];
				// int sampleDurationMs = (int)((float)DECODE_BUFFER_SIZE /
				// sampleRateInHz / CHANNEL_PER_FRAME * 1000);
				while (!isStop) {
					// long start = System.currentTimeMillis();
					extraSlientSampleSize[0] = 0;
					sample_count = decoder.readSamples(samples, extraSlientSampleSize);

					// Log.i(TAG, "end Native Mp3 playing......
					// DECODE_BUFFER_SIZE="+DECODE_BUFFER_SIZE+"
					// sample_count="+sample_count);

					if (sample_count == -2) {
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						// Log.i(TAG, "WARN : no play data");
						continue;
					}
					if (sample_count < 0) {
						break;
					}
					while (true) {
						synchronized (NativeMp3Player.class) {
							isPlayTemp = isPlaying;
						}
						if (isPlayTemp)
							break;
						else
							Thread.yield();
					}

					if (null != audioTrack && audioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
						audioTrack.setStereoVolume(1, 1);
						if (extraSlientSampleSize[0] > 0) {
							int totalSize = extraSlientSampleSize[0] + sample_count;
							short[] playSamples = new short[totalSize];
							System.arraycopy(samples, 0, playSamples, extraSlientSampleSize[0], sample_count);
							audioTrack.write(playSamples, 0, totalSize);
						} else {
							audioTrack.write(samples, 0, sample_count);
						}
					}
				}

				decoder.destory();
			} catch (Error e) {
				e.printStackTrace();
			}
			samples = null;

			// Log.i(TAG, "end Native Mp3 player thread...");
		}
	}

	public int getCurrentTimeMills() {
		int currentTimeMills = 0;
		try {
			if (null != audioTrack && audioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {

				long curPos = 0;
				curPos = audioTrack.getPlaybackHeadPosition() - audioTrackBaseHeadPosition;
				currentTimeMills = (int) ((float) curPos * 1000 / sampleRateInHz);
				currentTimeMills += seekBaseMillsTime;

			}
		} catch (Throwable e) {
		}
		return currentTimeMills;
	}

	public int getDuration() {
		return duration * 1000;
	}

	public int getAccompanySampleRate() {
		return sampleRateInHz;
	}
}
