#if ! JUCE_ANDROID
#  error
#endif

#include <juce_core/native/juce_android_JNIHelpers.h>


namespace lemons
{

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD, CALLBACK) \
  METHOD (addFlags, "addFlags", "(I)V")                                       \
  METHOD (clearFlags, "clearFlags", "(I)V")

DECLARE_JNI_CLASS (Window, "android/view/Window")
#undef JNI_CLASS_MEMBERS


void setAutoLockEnabled (bool shouldBeEnabled)
{
	auto activity = getMainActivity();
	auto env      = getEnv();
	auto window   = env->CallObjectMethod (activity.get(), AndroidActivity.getWindow);

	// https://developer.android.com/reference/android/view/WindowManager.LayoutParams#FLAG_KEEP_SCREEN_ON
	const auto keepScreenOnFlag { 0x00000080 };

	if (shouldBeOn)
		env->CallVoidMethod (window.get(), Window.clearFlags, keepScreenOnFlag);
	else
		env->CallVoidMethod (window.get(), Window.addFlags, keepScreenOnFlag);
}

}  // namespace lemons