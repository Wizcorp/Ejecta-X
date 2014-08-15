##Command Line Steps to Build and Install##
1. Install [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html) first.

2. Install [Android SDK](http://developer.android.com/sdk/index.html) then Eclipse and ADT OR, [Android Studio](http://developer.android.com/sdk/installing/studio.html)

3. Set ANDROID_HOME environment variable to the root of your Android SDK directory.
	```
	export ANDROID_HOME=/path/to/android-sdk-unzipped
	```

4. Navigate to the android sample project.
	```
	cd /path/to/ejectax/project/android
	```

5. Run ndk-build
	```
	/path/to/android-ndk-library/ndk-build
	```

6. Run the `assemble` gradle task
	```
	./gradlew assemble
	```

7. Install the apk to your device
	```
	adb install -r build/outputs/apk/android-debug.apk
	```

