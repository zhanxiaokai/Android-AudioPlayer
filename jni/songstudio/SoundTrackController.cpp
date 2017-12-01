#include "com_phuket_tour_audioplayer_opensles_SoundTrackController.h"

#include "libmedia/sound_service.h"

#define LOG_TAG "Sound_Track_Controller"

static const char* JAVA_LANG_IOEXCEPTION = "java/lang/IOException";
static const char* JAVA_LANG_OUTOFMEMORYERROR = "java/lang/OutOfMemoryError";
static void ThrowException(JNIEnv* env, const char* className, const char* message) {
	// Get the exception class
	jclass clazz = env->FindClass(className);

	// If exception class is found
	if (0 != clazz) {
		// Throw exception
		env->ThrowNew(clazz, message);

		// Release local class reference
		env->DeleteLocalRef(clazz);
	}
}

static bool CheckError(JNIEnv* env, SLresult result) {
	bool isError = false;

	// If an error occurred
	if (SL_RESULT_SUCCESS != result) {
		// Throw IOException
		const char* msg = ResultToString(result);
		LOGI("msg is %s", msg);
		ThrowException(env, JAVA_LANG_IOEXCEPTION, msg);

		isError = true;
	}

	return isError;
}

SoundService* soundService = NULL;
JNIEXPORT jboolean JNICALL Java_com_phuket_tour_audioplayer_opensles_SoundTrackController_setAudioDataSource(JNIEnv * env, jobject obj, jstring accompany, jfloat percent) {
	const char* accompanyPath = env->GetStringUTFChars(accompany, NULL);
	soundService = SoundService::GetInstance();
	//设置播放完成的回调
	JavaVM *g_jvm = NULL;
	env->GetJavaVM(&g_jvm);
	jobject g_obj = env->NewGlobalRef(obj);
	soundService->setOnCompletionCallback(g_jvm, g_obj);
	//设置播放文件路径以及解码buffer的1秒钟的百分比
	soundService->initSongDecoder(accompanyPath, percent);
	SLresult result = soundService->initSoundTrack();
	env->ReleaseStringUTFChars(accompany, accompanyPath);
	bool isError = CheckError(env, result);
	return isError;
}

JNIEXPORT jint JNICALL Java_com_phuket_tour_audioplayer_opensles_SoundTrackController_getAccompanySampleRate(JNIEnv * env, jobject obj) {
	if (NULL != soundService) {
		return soundService->getAccompanySampleRate();
	}
	return -1;
}

JNIEXPORT void JNICALL Java_com_phuket_tour_audioplayer_opensles_SoundTrackController_play
(JNIEnv * env, jobject obj) {
	if(NULL != soundService) {
		soundService->play();
	}
}

JNIEXPORT jint JNICALL Java_com_phuket_tour_audioplayer_opensles_SoundTrackController_getCurrentTimeMills(JNIEnv * env, jobject obj) {
	if (NULL != soundService) {
		return soundService->getCurrentTimeMills();
	}
	return 0;
}

JNIEXPORT void JNICALL Java_com_phuket_tour_audioplayer_opensles_SoundTrackController_stop
(JNIEnv * env, jobject obj) {
	if (NULL != soundService) {
		soundService->stop();
		soundService = NULL;
	}
}
