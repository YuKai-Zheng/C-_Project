package com.jlx.MixProject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import com.alipay.android.phone.mrpc.core.v;
import com.token.verifysdk.VerifyCoder;

import android.R.bool;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

/**
 * 验证码服务
 * 
 * @author sunsongye
 *
 */
public class VertifyCode {
	private Activity m_activity;
	private String m_serverUrl;
	private static VertifyCode instance;

	public static VertifyCode getInstance() {
		if (instance == null) {
			instance = new VertifyCode();
		}
		return instance;
	}

	@Deprecated
	public void getJSURL() {
		new Thread(new Runnable() {

			@Override
			public void run() {
				String JSUrl = "http://" + m_serverUrl + "/Public/AuditConfig.ashx?";

				Map<String, String> params = new HashMap<String, String>();
				params.put("action", "GetCodeURL");

				try {
					HttpResponse response = sendHttpClientPOSTRequest(JSUrl, params, "UTF-8");
					// sendPOSTRequest(JSUrl,params,"UTF-8");

					// 取得状态行中的状态码
					if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
						String result = EntityUtils.toString(response.getEntity());
						System.out.println("result:" + result);
						// 创建一个JSON对象
						JSONObject jsonObject = new JSONObject(result.toString());
						// 获取某个对象的JSON数组
						JSONArray jsonArray = jsonObject.getJSONArray("content");

						// 新建一个JSON对象，该对象是某个数组里的其中一个对象
						JSONObject jsonObject2 = (JSONObject) jsonArray.opt(0);
						String url = jsonObject2.getString("url");
						gotoVerifyActivity(url);
					}
					//
				} catch (Exception e1) {
					e1.printStackTrace();
				}
			}
		}).start();

	}

	public void vertify(String jsurl){
		gotoVerifyActivity(jsurl);		
	}
	
//	private void vertifyTicket(final String ticket) {
//		new Thread(new Runnable() {
//
//			@Override
//			public void run() {
//				String JSUrl = "http://" + m_serverUrl + "/Public/AuditConfig.ashx?";
//
//				Map<String, String> params = new HashMap<String, String>();
//				params.put("action", "CheckTicket");
//				params.put("key", ticket);
//
//				try {
//					HttpResponse response = sendHttpClientPOSTRequest(JSUrl, params, "UTF-8");
//					if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
//						String result = EntityUtils.toString(response.getEntity());
//						System.out.println("result:" + result);
//						if (result.equals("")) {
//							Log.e("test", "vertify ticket error!!!!");
//							return;
//						}
//						JSONObject jsonObject = new JSONObject(result.toString());
//						String content = jsonObject.getString("content");
//						System.out.println("content:" + content);
//					}
//				} catch (Exception e1) {
//					e1.printStackTrace();
//				}
//			}
//		}).start();
//	}
	
	public static native void setVertifyResult(bool  isPassed);
	public static native void vertifyTicket(String  ticket);

	private void gotoVerifyActivity(String jsurl) {
		VerifyCoder verify = VerifyCoder.getVerifyCoder();
		verify.setShowtitle(true);
		verify.startVerifyActivityForResult(m_activity, jsurl, 1);
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == 1) {
			if (resultCode == Activity.RESULT_OK) {
				Log.e("onActivityResult", "verifysucc");
				String ticket = data.getStringExtra("ticket");
				String randstr = data.getStringExtra("randstr");
				Toast.makeText(m_activity, "验证成功,票据为" + ticket, 2000).show();

				vertifyTicket(ticket);
			} else {
				Toast.makeText(m_activity, "未验证成功", 2000).show();
			}
		}
	}

	public void setActivity(Activity activity) {
		m_activity = activity;
	}

	public void setServerUrl(String url) {
		m_serverUrl = url;
	}

	/**
	 * 通过HttpClient框架发送POST请求 HttpClient该框架已经集成在android开发包中
	 * 个人认为此框架封装了很多的工具类，性能比不上自己手写的下面两个方法 但是该方法可以提高程序员的开发速度，降低开发难度
	 * 
	 * @param path
	 *            请求路径
	 * @param params
	 *            请求参数
	 * @param encoding
	 *            编码
	 * @return 请求是否成功
	 * @throws Exception
	 */
	private HttpResponse sendHttpClientPOSTRequest(String path, Map<String, String> params, String encoding)
			throws Exception {
		List<NameValuePair> pairs = new ArrayList<NameValuePair>();// 存放请求参数
		if (params != null && !params.isEmpty()) {
			for (Map.Entry<String, String> entry : params.entrySet()) {
				// BasicNameValuePair实现了NameValuePair接口
				pairs.add(new BasicNameValuePair(entry.getKey(), entry.getValue()));
			}
		}
		UrlEncodedFormEntity entity = new UrlEncodedFormEntity(pairs, encoding); // pairs:请求参数
																					// encoding:编码方式
		HttpPost httpPost = new HttpPost(path); // path:请求路径
		httpPost.setEntity(entity);

		DefaultHttpClient client = new DefaultHttpClient(); // 相当于浏览器
		HttpResponse response = client.execute(httpPost); // 相当于执行POST请求

		return response;
	}
}
