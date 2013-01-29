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
        Utils.copyDatFile(ctx, mainBundle+"/files/build/", "index.js");
        Utils.copyDatFile(ctx, mainBundle+"/files/build/", "bg.png");
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

	private static native void nativeRender();

	private static native void nativeCreated(String mainBundle, int width, int height);
	
	private static native void nativeChanged(int width, int height);
}
