package com.impactjs.ejecta;

import android.content.Context;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.view.KeyEvent;
import android.view.MotionEvent;

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
	
	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		mRenderer.nativeResume();
		super.onResume();
	}
	
	@Override
	public void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		mRenderer.nativePause();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		mRenderer.nativeTouch(event.getAction(), (int)event.getX(), (int)event.getY());
		return super.onTouchEvent(event);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		mRenderer.nativeOnKeyDown(keyCode);
		return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		mRenderer.nativeOnKeyUp(keyCode);
		return super.onKeyUp(keyCode, event);
	}
	
	@Override
	protected void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		
		super.onConfigurationChanged(newConfig);
	}
	
	public void onDestroy() {
		// TODO Auto-generated method stub
		mRenderer.nativeFinalize();
	}

    public void setEjectaEventListener(final EjectaRenderer.EjectaEventListener eventListener) {
        mRenderer.setOnCanvasCreatedListener(new EjectaRenderer.EjectaEventListener() {
            @Override
            public void onCanvasCreated() {
                eventListener.onCanvasCreated();
            }
        });
    }

    public void loadJavaScriptFile(String filename) {
        mRenderer.nativeLoadJavaScriptFile(filename);
    }

	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
	}

	private static native void nativeSetPaths();
}
