package com.impactjs.ejecta;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;
import android.content.SharedPreferences;

public class EjectaRenderer implements Renderer {

	private Context mContext;
	public static String dataBundle;
	private int screen_width;
	private int screen_height;
	private EjectaEventListener ejectaEventListener = null;
	// Declaring AssetManager prevents garbage collector from freeing once the native callback is called
	public static AssetManager assetManager;

	public EjectaRenderer(Context ctx, int width, int height) {
		dataBundle = "/data/data/" + ctx.getPackageName();
		assetManager = ctx.getResources().getAssets();
		mContext = ctx;
		screen_width = width;
		screen_height = height;
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		nativeRender(); 
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		nativeChanged(width, height);
		screen_width = width;
		screen_height = height;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		nativeCreated(assetManager, dataBundle, screen_width, screen_height);
		onCanvasCreated();
	}

	private native void nativeRender();

	private native void nativeCreated(AssetManager mgr, String mainBundle, int width, int height);

	private native void nativeChanged(int width, int height);

	public native void nativeFinalize();

	public native void nativePause();
	public native void nativeResume();

	public native void nativeLoadJavaScriptFile(String filename);

	public native void nativeTouch(int action, int x, int y);
	public native void nativeOnSensorChanged(float accle_x, float accle_y, float accle_z);
	public native void nativeOnKeyDown(int key_code);
	public native void nativeOnKeyUp(int key_code);

	// Emit event. Ejecta surface was created and init() has been called in JNI
	private void onCanvasCreated() {
		if (ejectaEventListener != null) {
			// Trigger event
			ejectaEventListener.onCanvasCreated();
		}
	};

	// Set listener public interface
	public void setOnCanvasCreatedListener(EjectaEventListener listener) {
		ejectaEventListener = listener;
	}

	public interface EjectaEventListener {
		public abstract void onCanvasCreated();
	}

	public void setSharedPreferences (String key , String value) {
		SharedPreferences settings = mContext.getSharedPreferences(mContext.getPackageName(), 0);
		SharedPreferences.Editor editor = settings.edit();
			
		editor.putString(key, value);
		editor.commit();
	}

	public String getSharedPreferences (String key) {
		SharedPreferences settings = mContext.getSharedPreferences(mContext.getPackageName(), 0);
		
		return settings.getString(key, "");
	}

}