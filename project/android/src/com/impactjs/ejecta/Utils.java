package com.impactjs.ejecta;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;

public class Utils {
	public static void copyDataFiles(Context context, String dest,String source) {
		copyDataFile( context,  dest, source );
	}
	
	public static void copyDataFile(Context context, String dest,String source) {
		String[] files;
		InputStream in = null;
		OutputStream out = null;
		try {
			files = context.getResources().getAssets().list(source);
		} catch (IOException e1) {
			return;
		}
		File mWorkingPath = new File(dest);
		if (mWorkingPath.isFile() && mWorkingPath.exists()) {
			return;
		} else {
			if (!mWorkingPath.mkdirs()) {
			}
		}
		for (int i = 0; i < files.length; i++) {
			String fileName = files[i];
			try {
				if (0 != source.length())
					in = context.getAssets().open(source + "/" + fileName);
				else
					in = context.getAssets().open(fileName);
			} catch (IOException e) {
				e.printStackTrace();
				if (0 == source.length()) {
					copyDataFile(context, dest + fileName + "/",fileName);
				} else {
					copyDataFile(context, dest + fileName+ "/",source + "/" + fileName);
				}
				continue;
			}
			try {
				
				File outFile = new File(mWorkingPath, fileName);
				if (outFile.exists())
					outFile.delete();
				
				out = new FileOutputStream(outFile);

				byte[] buf = new byte[1024];
				int len;
				while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				try {
					if (in != null)
						in.close();
					if (out != null) {
						out.flush();
						out.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

}
