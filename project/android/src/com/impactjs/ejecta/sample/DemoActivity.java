package com.impactjs.ejecta.sample;

import com.impactjs.ejecta.EjectaGLSurfaceView;
import android.app.Activity;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class DemoActivity extends Activity{
	private GLSurfaceView mGLView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		int width = getWindowManager().getDefaultDisplay().getWidth();
		int height = getWindowManager().getDefaultDisplay().getHeight();
	    mGLView = new EjectaGLSurfaceView(this, width, height);
        setContentView(mGLView);
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		((EjectaGLSurfaceView)mGLView).onDestroy();
		super.onDestroy();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
	
	static {
    	//System.loadLibrary("corefoundation");
    	System.loadLibrary("JavaScriptCore");
        System.loadLibrary("ejecta");
    }
	   
}