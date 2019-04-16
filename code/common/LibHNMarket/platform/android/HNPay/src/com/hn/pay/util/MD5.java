package com.hn.pay.util;

import java.security.MessageDigest;

public class MD5 {

	private MD5() {}
	
	public final static String getMessageDigest(byte[] buffer) {
		char hexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A','B','C','D','E','F' };
		try {
			// 获得MD5摘要算法的 MessageDigest对象
			MessageDigest mdTemp = MessageDigest.getInstance("MD5");
			// 使用指定的字节更新摘要
			mdTemp.update(buffer);
			// 获得密文
			byte[] md = mdTemp.digest();
			 // 把密文转换成十六进制的字符串形式
			int j = md.length;
			char str[] = new char[j * 2];
			int k = 0;
			for (int i = 0; i < j; i++) {
				byte byte0 = md[i];
				str[k++] = hexDigits[byte0 >>> 4 & 0xf];
				str[k++] = hexDigits[byte0 & 0xf];
			}
			return new String(str);
		} catch (Exception e) {
			return null;
		}
	}
}
