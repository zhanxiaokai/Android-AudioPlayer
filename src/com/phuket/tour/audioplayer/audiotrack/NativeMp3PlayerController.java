package com.phuket.tour.audioplayer.audiotrack;

import java.util.Timer;
import java.util.TimerTask;

import android.media.AudioManager;
import android.os.Handler;

public class NativeMp3PlayerController {

	public static final int UPDATE_PLAY_VOICE_PROGRESS = 730;
	
	public NativeMp3Player mediaPlayer;

	private boolean isPlaying = false;

	private Timer mTimer;
	private TimerTask myTimerTask = null;
	private Handler mHander;

	public int getCurrentTimeMills() {
		return mediaPlayer.getCurrentTimeMills();
	}
	
	public NativeMp3PlayerController() {
		try {
			if (mediaPlayer == null) {
				mediaPlayer = new NativeMp3Player();
				mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			}
		} catch (Exception e) {
		}
	}

	public int getPlayerCurrentTime() {
		try {
			return mediaPlayer.getCurrentTimeMills();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	public int getPlayerDuration() {
		try {
			if (null != mediaPlayer) {
				return mediaPlayer.getDuration();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	// *****************************************************

	public boolean setAudioDataSource(String path) {
		boolean result = mediaPlayer.setDataSource(path);
		if (result) {
			mediaPlayer.prepare();
		}
		return result;
	}

	public void start() {
		timerStop();
		isPlaying = true;
		// timelabelProgress.sendEmptyMessage(0);
		timerStart();
		try {
			if (mediaPlayer != null) {
				mediaPlayer.start();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
	}

	public void stop() {
		timerStop();
		try {
			if (mediaPlayer != null) {
				mediaPlayer.stop();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
		isPlaying = false;
	}

	private void timerStop() {
		if (mTimer != null) {
			mTimer.cancel();
			mTimer = null;
		}
		if (myTimerTask != null) {
			myTimerTask.cancel();
			myTimerTask = null;
		}
	}

	private void timerStart() {
		if (mTimer == null) {
			mTimer = new Timer();
			myTimerTask = new MusicTimerTask();
			mTimer.schedule(myTimerTask, 0, 50);
		}
	}

	class MusicTimerTask extends TimerTask {
		@Override
		public void run() {
			int time = getPlayerCurrentTime();
			if (time != 0) {
				mHander.sendMessage(mHander.obtainMessage(
						UPDATE_PLAY_VOICE_PROGRESS, time, isPlaying ? 1 : 0));
			}
		}
	}

	public NativeMp3Player getMediaPlayer() {
		return mediaPlayer;
	}

	public void setHandler(Handler mTimeHandler) {
		this.mHander = mTimeHandler;
	}

	public int getAccompanySampleRate() {
		try {
			if (mediaPlayer != null) {
				return mediaPlayer.getAccompanySampleRate();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
		return -1;
	}

}