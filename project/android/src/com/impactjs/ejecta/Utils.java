package com.impactjs.ejecta;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;

public class Utils {
	/*				 我的更新          start                       */
	public static void copyDatFile(Context context, String dest,String source) {
		InputStream in = null;
		OutputStream out = null;
		File mWorkingPath = new File(dest);
		if (!mWorkingPath.exists()) {
			if (!mWorkingPath.mkdirs()) {
			}
		}
		try {
			in = context.getResources().getAssets().open("build/" + source);
			File outFile = new File(mWorkingPath, source);
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
	/*				 我的更新     end                            */}
