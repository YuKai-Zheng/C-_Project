package com.hn.framework;

import java.io.UnsupportedEncodingException;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.UUID;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.base.adapter.ITelephoneManager;
import com.base.adapter.TelephoneManagerFactor;

public class HNCommonUtils {
	private static final String TAG = HNCommonUtils.class.getSimpleName();

	public static final String ISP_TYPE_CHINAMOBILE = "ChinaMobile";
	public static final String ISP_TYPE_CHINAUNICOM = "ChinaUnicom";
	public static final String ISP_TYPE_CHINATELECOM = "ChinaTelecom";

	public static final int NET_TYPE_UNKNOWN = -1;
	public static final int NET_TYPE_MOBILE = 0;
	public static final int NET_TYPE_WIFI = 1;

	private static String mIMSI = null;

	public static String getIMSI(Context context) {
		if (mIMSI == null) {
			mIMSI = getIMSIImp(context);
		}
		return mIMSI;
	}

	private static String getIMSIImp(Context context) {
		String imsi = null;
		TelephonyManager tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		if (tm != null && tm.getSimState() == TelephonyManager.SIM_STATE_READY) {
			imsi = tm.getSubscriberId();
		}
		if (imsi == null || imsi.equals("")) {
			imsi = getQualcommDubleSimIMSI(context);
		}
		if (imsi == null || imsi.equals("")) {
			imsi = initMtkDoubleSimIMSI(context);
		}
		if (imsi == null || imsi.equals("")) {
			ITelephoneManager aoraTM = TelephoneManagerFactor.getInstance(
					context).create();
			if (aoraTM != null
					&& aoraTM.getSimState() == TelephonyManager.SIM_STATE_READY) {
				imsi = aoraTM.getSubscriberId();
			}
		}
		if (imsi == null || imsi.equals("")) {
			imsi = "000000000000000";
		}
		return imsi;
	}

	private static String initMtkDoubleSimIMSI(Context context) {
		String imsi = null;
		try {
			TelephonyManager tm = (TelephonyManager) context
					.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName("com.android.internal.telephony.Phone");
			for (int i = 1; i < 3 && imsi == null; i++) {
				Field fields = c.getField("GEMINI_SIM_" + i);
				fields.setAccessible(true);
				Integer simID = (Integer) fields.get(null);
				Method m = TelephonyManager.class.getDeclaredMethod(
						"getSubscriberIdGemini", int.class);
				imsi = (String) m.invoke(tm, simID);
			}
		} catch (Exception e) {

		}
		return imsi;
	}

	private static String getQualcommDubleSimIMSI(Context context) {
		String imsi = null;
		try {
			Class<?> telphoneCls = Class
					.forName("android.telephony.MSimTelephonyManager");
			Object obj = context.getSystemService("phone_msim");
			Method ms = telphoneCls.getMethod("getSubscriberId", int.class);
			int simId_1 = 0;
			int simId_2 = 1;
			imsi = (String) ms.invoke(obj, simId_1);
			if (imsi == null || imsi.equals("")) {
				imsi = (String) ms.invoke(obj, simId_2);
			}
		} catch (Exception e) {

		}
		return imsi;
	}

	public static String getISP(Context context) {
		String imsi = getIMSI(context);
		if (imsi.startsWith("46000") || imsi.startsWith("46002")
				|| imsi.startsWith("46007")) {
			return ISP_TYPE_CHINAMOBILE;
		} else if (imsi.startsWith("46001")) {
			return ISP_TYPE_CHINAUNICOM;
		} else if (imsi.startsWith("46003") || imsi.startsWith("46005")) {
			return ISP_TYPE_CHINATELECOM;
		}
		return "-1";
	}

	public static String getIMEI(Context context) {
		TelephonyManager tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		return tm.getDeviceId();
	}

	public static String getModel() {
		return android.os.Build.MODEL;
	}

	public static String getSys() {
		return "android";
	}

	public static String getSysVersion() {
		return android.os.Build.VERSION.RELEASE;
	}

	public static boolean checkSimReady(Context context) {
		TelephonyManager tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		boolean simReady = false;
		switch (tm.getSimState()) {
		case TelephonyManager.SIM_STATE_ABSENT:
			break;
		case TelephonyManager.SIM_STATE_UNKNOWN:
			break;
		case TelephonyManager.SIM_STATE_NETWORK_LOCKED:
			break;
		case TelephonyManager.SIM_STATE_PIN_REQUIRED:
			break;
		case TelephonyManager.SIM_STATE_PUK_REQUIRED:
			break;
		case TelephonyManager.SIM_STATE_READY:
			break;
		}
		if (tm != null && tm.getSimState() == TelephonyManager.SIM_STATE_READY) {
			simReady = true;
		}
		return simReady;
	}

	public static String getGameVersion(Context context) {
		String s = "1.0.0";
		PackageManager pm = context.getPackageManager();
		PackageInfo pi;
		try {
			pi = pm.getPackageInfo(context.getPackageName(), 0);
			s = pi.versionName;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return s;
	}

	public static String getGameName(Context context, String name) {
		return getMetaDataString(context, name);
	}

	public static int getMetaDataInt(Context context, String name) {
		int Value = -1;
		PackageManager pm = context.getPackageManager();
		ApplicationInfo appInfo;
		try {
			appInfo = pm.getApplicationInfo(context.getPackageName(),
					PackageManager.GET_META_DATA);
			Value = appInfo.metaData.getInt(name);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return Value;
	}

	public static String getMetaDataString(Context context, String name) {
		String s = "";
		PackageManager pm = context.getPackageManager();
		ApplicationInfo appInfo;
		try {
			appInfo = pm.getApplicationInfo(context.getPackageName(),
					PackageManager.GET_META_DATA);
			s = appInfo.metaData.getString(name);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return s;
	}

	public static boolean isConnectingToInternet(Context context) {
		ConnectivityManager connectivity = (ConnectivityManager) (context)
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getState() == NetworkInfo.State.CONNECTED) {
						return true;
					}
				}
			}
		}
		return false;
	}

	public static int getNetType(Context context) {
		ConnectivityManager connectMgr = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo info = connectMgr.getActiveNetworkInfo();
		if (info != null) {
			int type = info.getType();
			if (type == ConnectivityManager.TYPE_WIFI) {
				return NETWORK_CLASS_WIFI;
			} else if (type == ConnectivityManager.TYPE_MOBILE) {
				// return NET_TYPE_MOBILE;
				int networkType = NET_TYPE_UNKNOWN;
				TelephonyManager telephonyManager = (TelephonyManager) context
						.getSystemService(Context.TELEPHONY_SERVICE);
				networkType = telephonyManager.getNetworkType();
				return getNetworkClassByType(networkType);
			}
		}
		return -1;
	}

	private static final int NETWORK_CLASS_WIFI = -101;
	private static final int NETWORK_CLASS_UNAVAILABLE = -1;
	/** Unknown network class. */
	private static final int NETWORK_CLASS_UNKNOWN = 0;
	/** Class of broadly defined "2G" networks. */
	private static final int NETWORK_CLASS_2_G = 1;
	/** Class of broadly defined "3G" networks. */
	private static final int NETWORK_CLASS_3_G = 2;
	/** Class of broadly defined "4G" networks. */
	private static final int NETWORK_CLASS_4_G = 3;
	//
	private static final int NETWORK_TYPE_UNAVAILABLE = -1;
	private static final int NETWORK_TYPE_WIFI = -101;
	/** Network type is unknown */
	public static final int NETWORK_TYPE_UNKNOWN = 0;
	/** Current network is GPRS */
	public static final int NETWORK_TYPE_GPRS = 1;
	/** Current network is EDGE */
	public static final int NETWORK_TYPE_EDGE = 2;
	/** Current network is UMTS */
	public static final int NETWORK_TYPE_UMTS = 3;
	/** Current network is CDMA: Either IS95A or IS95B */
	public static final int NETWORK_TYPE_CDMA = 4;
	/** Current network is EVDO revision 0 */
	public static final int NETWORK_TYPE_EVDO_0 = 5;
	/** Current network is EVDO revision A */
	public static final int NETWORK_TYPE_EVDO_A = 6;
	/** Current network is 1xRTT */
	public static final int NETWORK_TYPE_1xRTT = 7;
	/** Current network is HSDPA */
	public static final int NETWORK_TYPE_HSDPA = 8;
	/** Current network is HSUPA */
	public static final int NETWORK_TYPE_HSUPA = 9;
	/** Current network is HSPA */
	public static final int NETWORK_TYPE_HSPA = 10;
	/** Current network is iDen */
	public static final int NETWORK_TYPE_IDEN = 11;
	/** Current network is EVDO revision B */
	public static final int NETWORK_TYPE_EVDO_B = 12;
	/** Current network is LTE */
	public static final int NETWORK_TYPE_LTE = 13;
	/** Current network is eHRPD */
	public static final int NETWORK_TYPE_EHRPD = 14;
	/** Current network is HSPA+ */
	public static final int NETWORK_TYPE_HSPAP = 15;

	private static int getNetworkClassByType(int networkType) {
		switch (networkType) {
		case NETWORK_TYPE_UNAVAILABLE:
			return NETWORK_CLASS_UNAVAILABLE;
		case NETWORK_TYPE_WIFI:
			return NETWORK_CLASS_WIFI;
		case NETWORK_TYPE_GPRS:
		case NETWORK_TYPE_EDGE:
		case NETWORK_TYPE_CDMA:
		case NETWORK_TYPE_1xRTT:
		case NETWORK_TYPE_IDEN:
			return NETWORK_CLASS_2_G;
		case NETWORK_TYPE_UMTS:
		case NETWORK_TYPE_EVDO_0:
		case NETWORK_TYPE_EVDO_A:
		case NETWORK_TYPE_HSDPA:
		case NETWORK_TYPE_HSUPA:
		case NETWORK_TYPE_HSPA:
		case NETWORK_TYPE_EVDO_B:
		case NETWORK_TYPE_EHRPD:
		case NETWORK_TYPE_HSPAP:
			return NETWORK_CLASS_3_G;
		case NETWORK_TYPE_LTE:
			return NETWORK_CLASS_4_G;
		default:
			return NETWORK_CLASS_UNKNOWN;
		}
	}

	public static String getNetIP(Context context) {
		if (getNetType(context) == NETWORK_CLASS_WIFI) {
			// 获取wifi服务
			WifiManager wifiManager = (WifiManager) context
					.getSystemService(Context.WIFI_SERVICE);
			// //判断wifi是否开启
			if (wifiManager.isWifiEnabled()) {
				WifiInfo wifiInfo = wifiManager.getConnectionInfo();
				int ipAddress = wifiInfo.getIpAddress();
				return (ipAddress & 0xFF) + "." + ((ipAddress >> 8) & 0xFF)
						+ "." + ((ipAddress >> 16) & 0xFF) + "."
						+ (ipAddress >> 24 & 0xFF);
			}
			return "null";
		} else if (getNetType(context) == NETWORK_CLASS_2_G||getNetType(context) == NETWORK_CLASS_3_G||getNetType(context) == NETWORK_CLASS_4_G) {
			try {
				for (Enumeration<NetworkInterface> en = NetworkInterface
						.getNetworkInterfaces(); en.hasMoreElements();) {
					NetworkInterface intf = en.nextElement();
					for (Enumeration<InetAddress> enumIpAddr = intf
							.getInetAddresses(); enumIpAddr.hasMoreElements();) {
						InetAddress inetAddress = enumIpAddr.nextElement();
						if (!inetAddress.isLoopbackAddress()) {
							return inetAddress.getHostAddress().toString();
						}
					}
				}
			} catch (SocketException ex) {
				Log.e("WifiPreference IpAddress", ex.toString());
			}
		}
		return "null";
	}

	public static boolean hasInstalledPackage(Context context,
			String packageName) {
		final PackageManager packageManager = context.getPackageManager();
		List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);
		List<String> pName = new ArrayList<String>();
		if (pinfo != null) {
			for (int i = 0; i < pinfo.size(); i++) {
				String pn = pinfo.get(i).packageName;
				pName.add(pn);
			}
		}
		return pName.contains(packageName);
	}

	public static byte[] encrypt(byte[] srcData, String key) {
		if (key == null || key.equals("")) {
			return srcData;
		}
		byte[] keyData = null;
		try {
			keyData = key.getBytes("utf-8");
		} catch (UnsupportedEncodingException uee) {
			return srcData;
		}

		byte[] desData = new byte[srcData.length];

		int j = 0;
		for (int i = 0; i < srcData.length; i++) {
			desData[i] = (byte) (srcData[i] ^ keyData[j]);
			j++;
			if (j == keyData.length) {
				j = 0;
			}
		}
		return desData;
	}

	public static byte[] decrypt(byte[] srcData, String key) {
		return encrypt(srcData, key);
	}

	public static String getUUID() {
		String uuid = UUID.randomUUID().toString();
		return uuid.substring(0, 8) + uuid.substring(9, 13)
				+ uuid.substring(14, 18) + uuid.substring(19, 23)
				+ uuid.substring(24);
	}

	public static String getSerialNumber(Context context) {
		final TelephonyManager tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);

		final String tmDevice, tmSerial, androidId;

		tmDevice = "" + tm.getDeviceId();

		tmSerial = "" + tm.getSimSerialNumber();

		androidId = ""
				+ android.provider.Settings.Secure.getString(
						context.getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);

		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());

		String uniqueId = deviceUuid.toString();

		Log.d(TAG, "uuid =" + uniqueId);

		return uniqueId;
	}

	public static void dealPhone(Context context, String phoneNumber) {
		String urlTel = String.format("tel://%s", phoneNumber);
		Intent intent = new Intent(Intent.ACTION_DIAL, Uri.parse(urlTel));
		context.startActivity(intent);
	}

	public static void dealMsg(Context context, String mesNumber) {
		String urlMsg = String.format("smsto:%s", mesNumber);
		Intent intent = new Intent(Intent.ACTION_SENDTO, Uri.parse(urlMsg));
		context.startActivity(intent);
	}

	public static void sendEmail(Context context, String to, String text) {
		Intent data = new Intent(Intent.ACTION_SENDTO);
		data.setData(Uri.parse(String.format("mailto:%s", to)));
		data.putExtra(Intent.EXTRA_SUBJECT, "请描述需要咨询的问题。");
		data.putExtra(Intent.EXTRA_TEXT, text);
		context.startActivity(data);
	}

	public static void installApp(Context context, String storePath) {
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.parse("file://" + storePath),
				"application/vnd.android.package-archive");
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		context.startActivity(intent);
	}
}
