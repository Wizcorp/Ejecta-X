package com.impactjs.ejecta;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

public class EjectaRenderer implements Renderer {
	
    private Context mContext;
    public static String mainBundle;
    private int screen_width;
    private int screen_height;
    
	public EjectaRenderer(Context ctx, int width, int height) {
		mainBundle = "/data/data/" + ctx.getPackageName();
        mContext = ctx;
        System.out.println(mainBundle);
        Utils.copyDatFiles(ctx, mainBundle+"/files/build/", "build");
        screen_width = width;
        screen_height = height;
	}

	@Override
	public void onDrawFrame(GL10 gl) {  
		nativeRender(); 
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) { 
		
		// TODO Auto-generated method stub
		 
		 nativeChanged(width, height);
		 
		 screen_width = width;
		 screen_height = height;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		nativeCreated(mainBundle, screen_width, screen_height);
	}

	private native void nativeRender();

	private native void nativeCreated(String mainBundle, int width, int height);
	
	private native void nativeChanged(int width, int height);
	
	public native void nativeFinalize();
    
	public native void nativePause();
	public native void nativeResume();
    
	public native void nativeTouch(int action, int x, int y);
	public native void nativeOnSensorChanged(float accle_x, float accle_y, float accle_z);
	public native void nativeOnKeyDown(int key_code);
	public native void nativeOnKeyUp(int key_code);
}
