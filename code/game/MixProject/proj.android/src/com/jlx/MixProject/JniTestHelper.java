package com.jlx.MixProject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.UUID;

import android.os.Environment;
import android.os.Handler;
import android.util.Log;

public class JniTestHelper {

	private static Handler mHandler;

	public static void init(Handler handler) {
		mHandler = handler;
	}

	public static String setUUID2CPlusplus(final String content0) {
		String rootPath = Environment.getExternalStorageDirectory().getPath();
		File file = new File(rootPath + File.separator + "data");
		Log.i("test", "write:" + file.canWrite());
		file.mkdir();
		
		//是否推广包
		boolean bIsAdPackage = false;
		
		try {
			File tmp = new File(rootPath + File.separator + File.separator + ".scecret");
			if (tmp.exists()) {
				String content = "";
				String tmpStr = null;
				BufferedReader reader = new BufferedReader(new FileReader(tmp));
				while ((tmpStr = reader.readLine()) != null) {
					content += tmpStr;
				}
				//setUUID(content);
				return content;
			} else {
				UUID uuid = UUID.randomUUID();
				System.out.println(uuid);

				FileOutputStream outputStream = new FileOutputStream(tmp);
				
				if(bIsAdPackage)
				{
					String tmpStr = "" + "c-" + uuid.toString();
					outputStream.write(tmpStr.getBytes());
					outputStream.close();
					//setUUID(tmpStr);
					return tmpStr;
				}
				else
				{
					outputStream.write(uuid.toString().getBytes());
					outputStream.close();
					//setUUID(uuid.toString());
					return uuid.toString();
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			return "";
		}
	}

	public static native void setPackageName(String packageName);

	public static native void setUUID(String uuid);
	
	public static native void getAccessToken(String code);
}
