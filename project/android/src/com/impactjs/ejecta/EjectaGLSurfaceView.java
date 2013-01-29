package com.impactjs.ejecta;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class EjectaGLSurfaceView extends GLSurfaceView {
	
	public EjectaGLSurfaceView(Context context) {
		// TODO Auto-generated constructor stub
		super(context);
	}
	
	EjectaRenderer mRenderer;
	public EjectaGLSurfaceView(Context context, int width, int height) {
		super(context);
		// TODO Auto-generated constructor stub
		mRenderer = new EjectaRenderer(context, width, height);
        setRenderer(mRenderer);
	}
	private static native void nativeSetPaths();
}
