package com.phuket.tour.audioplayer.opensles;

public class SoundTrackController {
	public interface OnCompletionListener{
		public void onCompletion();
	};
	private OnCompletionListener onCompletionListener;
	public void setOnCompletionListener(OnCompletionListener onCompletionListener){
		this.onCompletionListener = onCompletionListener;
	}
	/**
	 * 设置播放文件地址，有可能是伴唱原唱都要进行设置
	 */
	public native boolean setAudioDataSource(String accompanyPath, float percent);
	/**
	 * 获得伴奏的采样频率
	 */
	public native int getAccompanySampleRate();
	/**
	 * 播放伴奏
	 */
	public native void play();
	/**
	 * 获得播放伴奏的当前时间
	 */
	public native int getCurrentTimeMills();
	/**
	 * 停止伴奏
	 */
	public native void stop();
	public void onCompletion(){
		onCompletionListener.onCompletion();
	}
	
}
