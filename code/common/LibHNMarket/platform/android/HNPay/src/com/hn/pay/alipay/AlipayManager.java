package com.hn.pay.alipay;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Map;
import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.PayResultCallBack.PAY_SDK;
import com.hn.pay.util.SignUtils;

/**
 * 重要说明:
 * 
 * 这里只是为了方便直接向商户展示支付宝的整个支付流程；所以Demo中加签过程直接放在客户端完成；
 * 真实App里，privateKey等数据严禁放在客户端，加签过程务必要放在服务端完成； 防止商户私密数据泄露，造成不必要的资金损失，及面临各种安全风险；
 */
public class AlipayManager {
	/** 支付宝支付业务：入参app_id */
	public static final String APPID = "2017062707578245";

	/** 支付宝账户登录 授权业务：入参pid值(=商户PID) */
	public static final String PID = "2088721368556113";
	/** 支付宝账户登录 授权业务：入参target_id值 */
	public static final String TARGET_ID = "2088721368556113";

	/** 商户私钥，pkcs8格式 */
	public static final String RSA_PRIVATE = "MIIEuwIBADANBgkqhkiG9w0BAQEFAASCBKUwggShAgEAAoIBAQCoX5LhYFlY9JEBpO4is4jQiRbq1NX7ZDHIUoW0mkg/Go4kuQqM1k/cxzA1/n/l7yQm6iUElECZV5ZbBkVDayCFxjMeuzZZB7bgVMlxQYZCTXOF7NiOq1HM0W8Le2SKsEQKMhBMfLyvmUsmZ5694TCF1fee2LFHi9MDRiqxXkbQxmX6TI0emn7QATEowIYAE+M5TnMjV37JrxXwCM66p4F0KZlGubMU8//nU3ht8gcPQoFIRuGjtUBPkJbiIcdRrrpQukgCfzsbieIjzt59+gJanVRyiKbsa2AoPH8i4GFOX5UQysXPVZfpZAxYCvSXVLpcWO4gVeyivAkjXgIWdVstAgMBAAECgf8F61Y+liSN/AsdA69biPRGYItttg3PyLX3VO9eag4gKc7Ve6vjR6WHHXmUbF4St1WOCK1Ee/HJckrW6GGG/T0ndtBVRD+k4aCMQm7SjHsr2Y02KB105pD5omNFBBGEzq2Vrizkg34erkT2FK5//mq2Sc9qxjN1UHUaqFzX33id3k0dmozisqwNOXS+g1sRTW14i10XeRrYlUeXEcE0G6+ygmH+/kbS8NSZmpkIjbHGu7l4TKz5sM5FVM2kfI0ofdybPkxccpSD0enUkA4gPpPrv8qsXiQZiiUDgtVKM3xgNYSCGUu0mtUMn5JbzUHkp0v6hNcIeNexmcU1L9EI450CgYEA1BHgOZv7qoUnUIcrLRKJRqd2HS9CMG4qB9VyNqEfBfc/4K+eVmJpqXuWhS8iv5Vz2hWwg06SlRY+gJ08pQUYGYCG31Du3wYKM534EUW4Y3o0ri90OCAxWRyB0/DfWqMbhG6NOp7CH2Iv1ehnuzzHkgMpdmHdT1akSkUcd8JaZVsCgYEAy0B109DkTTFZqKJ2D4V8hpKkJ2qBfiuBevQ3c+SLnXdZOG/3KQGwnF9h/Y/XBTTAaIc85Lx+OorBwa5t1xJ5ngYZrNU8snvlRrEvZEwtmixw0yIZttVPhYdNPxSVoOzVUZC05hb19CTpqLpY0ECkMtA0oO58Js21NhhdSyOWwBcCgYEAmZJUYQOfLO39+WfZEWvB82NjumC1uuKcF04vxnXJRIW3QVuL+xyLTcuMxefLbxeobL6WM5BbwBD6tCKpfwsAV79mbCNsyB7UlmqH8OIfVBJ0QctlixttAQ4Oyk89P6KsU6vHJDNBen6dC3egXsv17bvSc8MNA95HXO1NX0BkO88CgYAR6QE5BAxKHRIogQBkR7/8X8mlS4ZTpuPHAOQuCzKeUpkDFyKxc1uru9/3pwgshkR6MYiVI2MAJ6ABQCyfqhobabAzgarSk4ZVn2FstiXCky3x/OxFJzDVtGGDMH4q6xO1F0pJiDZuKB6K5rjSyQ9vBtfMYpeAF9pjBOybpWwwSwKBgC27S+SeByO/DphJOfjcPgTv62RqIe1LKe2Mjj/OT8VMx6glcIj81t72oQA7Zg6L+V93cdP6P38/PtdPNcSv/Kvnx4rdfwM1IeT1vCLChgCH2Gf1h4UK5BGSJtOcn+qw2cR+nRtl1XydhCvVbE6/xePIHEVCIl7mR1Z92aK1zMH0";
	// 支付宝公钥
	// public static final String RSA_PUBLIC =
	// "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqF+S4WBZWPSRAaTuIrOI0IkW6tTV+2QxyFKFtJpIPxqOJLkKjNZP3McwNf5/5e8kJuolBJRAmVeWWwZFQ2sghcYzHrs2WQe24FTJcUGGQk1zhezYjqtRzNFvC3tkirBECjIQTHy8r5lLJmeeveEwhdX3ntixR4vTA0YqsV5G0MZl+kyNHpp+0AExKMCGABPjOU5zI1d+ya8V8AjOuqeBdCmZRrmzFPP/51N4bfIHD0KBSEbho7VAT5CW4iHHUa66ULpIAn87G4niI87effoCWp1Ucoim7GtgKDx/IuBhTl+VEMrFz1WX6WQMWAr0l1S6XFjuIFXsorwJI14CFnVbLQIDAQAB"

	// 商户PID
	public static final String PARTNER = "2088721368556113";
	// 商户收款账号
	public static final String SELLER = "941278664@qq.com";
	// public static final String RSA_PRIVATE =
	// "MIICXAIBAAKBgQCqTfQoptkoFPd2mYnOzJBZC8J4mp30F/YF613J6Z4Ad4pB1kBNUxztEXbQtVgzzEjOeTU33UQMIVxUZ3MjOzOMJWWVi1aeHgZLMQZKbppBN7WoK817B/y0B32gj+UPaNTNu/HNNHBVqnSgvpMLm5gqG27mdFC/ui38FTZbds1CWwIDAQABAoGBAIEDQtZU5ycNExZeY94sa6F5Xh4dC1FxoWeEkO03tAEoja0oUFgDgWM6Gqcq6aF7vR/lBGLIlRcXYqlyIO/sljP1S8FMU9oh7Ma73mEnG88V2fkrvr0ur6c92WhjzybvE4UWNjNNrZhoq8jNcVDtmrb9SW6X/YncH7+zjirjxUxJAkEA1+4DWS5kmLmf4VuSNdOmD4kCulL92qK1OsyND5ylR4faQIt11aR5lpHUD72uu4s95h+BJ3bLI1h6MfQmGIAkvwJBAMnodlsnqQdylgMNbGOhgTeBC9Hp/HyQqKIrLGZ3lN7OcWsUCYpH4t9YqYUvP1BefnLyl2MoRYvmOF279Y2S/WUCQElHdAly7h7KeBDBw9fSkPuasrtK6O2m5DwlKQYQUiL5Xr8i1QWBAOZxbPc+lyr8cTBEps8eyIehQi7rvXX5Hv8CQCLadvvbR8laVVznh0tXiMe9Bf5RZt1V11ZPH5X8lH01o8x5JABz0kz9sB2mf0ZvWWKcT9e1DvRMZHSByqyKsCUCQG7XVkhz/CDtgUeVSbXbp4xkDkN5EZQBV0Gk8jEPBVO0OYvvC7xRhH5oFOHMC+zM861nA0Y/Abon54HR4mgyzy4=";

	private static final int SDK_PAY_FLAG = 1;
	private static final int SDK_CHECK_FLAG = 2;
	private static final int SDK_AUTH_FLAG = 2;

	private Activity activity;

	private PayResultCallBack callback;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				PayResult payResult = new PayResult((Map<String, String>) msg.obj);
				/**
				 * 对于支付结果，请商户依赖服务端的异步通知结果。同步通知结果，仅作为支付结束的通知。
				 */
				String resultInfo = payResult.getResult();// 同步返回需要验证的信息
				String resultStatus = payResult.getResultStatus();

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					// 该笔订单是否真实支付成功，需要依赖服务端的异步通知。
					callback.onPayResult(PAY_SDK.ALIPAY, 1);
				} else {
					// 该笔订单真实的支付结果，需要依赖服务端的异步通知。
					Toast.makeText(activity, "支付失败", Toast.LENGTH_SHORT).show();

					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000”
					// 代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					// if (TextUtils.equals(resultStatus, "8000")) {
					// Toast.makeText(activity, "支付结果确认中",
					// Toast.LENGTH_SHORT).show();
					// } else {
					// // 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
					// Toast.makeText(activity, "支付失败",
					// Toast.LENGTH_SHORT).show();
					// callback.onPayResult(PAY_SDK.ALIPAY, 0);
					// }
				}
				break;
			}

			case SDK_AUTH_FLAG: {
				@SuppressWarnings("unchecked")
				AuthResult authResult = new AuthResult((Map<String, String>) msg.obj, true);
				String resultStatus = authResult.getResultStatus();

				// 判断resultStatus 为“9000”且result_code
				// 为“200”则代表授权成功，具体状态码代表含义可参考授权接口文档
				if (TextUtils.equals(resultStatus, "9000") && TextUtils.equals(authResult.getResultCode(), "200")) {
					// 获取alipay_open_id，调支付时作为参数extern_token 的value
					// 传入，则支付账户为该授权账户
					Toast.makeText(activity, "授权成功\n" + String.format("authCode:%s", authResult.getAuthCode()), Toast.LENGTH_SHORT).show();
				} else {
					// 其他状态值则为授权失败
					Toast.makeText(activity, "授权失败" + String.format("authCode:%s", authResult.getAuthCode()), Toast.LENGTH_SHORT).show();
				}
				break;
			}

			// case SDK_CHECK_FLAG: {
			// Toast.makeText(activity, "检查结果为：" + msg.obj,
			// Toast.LENGTH_SHORT).show();
			// break;
			// }
			// default:
			// break;
			}
		};
	};

	public AlipayManager(Activity activity) {
		this.activity = activity;
	}

	/**
	 * 支付宝支付业务
	 */
	public void pay(String subject, String body, String price, String orderID, String IP,PayResultCallBack callback) {
		this.callback = callback;
//		String orderInfo = getOrderInfo(product, productDescription, orderID, productPrice);
//		String sign = sign(orderInfo);
//		try {
//			// 仅需对sign 做URL编码
//			sign = URLEncoder.encode(sign, "UTF-8");
//		} catch (UnsupportedEncodingException e) {
//			e.printStackTrace();
//		}
//		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&" + getSignType();

		if (TextUtils.isEmpty(APPID) || TextUtils.isEmpty(RSA_PRIVATE)) {
			new AlertDialog.Builder(activity).setTitle("警告").setMessage("需要配置APPID | RSA_PRIVATE")
					.setPositiveButton("确定", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialoginterface, int i) {
							activity.finish();
						}
					}).show();
			return;
		}

		/**
		 * 这里只是为了方便直接向商户展示支付宝的整个支付流程；所以Demo中加签过程直接放在客户端完成；
		 * 真实App里，privateKey等数据严禁放在客户端，加签过程务必要放在服务端完成；
		 * 防止商户私密数据泄露，造成不必要的资金损失，及面临各种安全风险；
		 * 
		 * orderInfo的获取必须来自服务端；
		 */
		Map<String, String> params = OrderInfoUtil2_0.buildOrderParamMap(APPID,subject,body,price,orderID,IP);
		String orderParam = OrderInfoUtil2_0.buildOrderParam(params);
		String sign = OrderInfoUtil2_0.getSign(params, RSA_PRIVATE);
		final String orderInfo = orderParam + "&" + sign;

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// // 构造PayTask 对象
				// PayTask alipay = new PayTask(activity);
				// // 调用支付接口
				// String result = alipay.pay(payInfo, true);
				//
				// Message msg = new Message();
				// msg.what = SDK_PAY_FLAG;
				// msg.obj = result;
				// mHandler.sendMessage(msg);
				PayTask alipay = new PayTask(activity);
				Map<String, String> result = alipay.payV2(orderInfo,true);
				Log.i("msp", result.toString());
				
				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * 支付宝账户授权业务
	 * 
	 * @param v
	 */
	public void auth(View v) {
		if (TextUtils.isEmpty(PID) || TextUtils.isEmpty(APPID) || TextUtils.isEmpty(RSA_PRIVATE) || TextUtils.isEmpty(TARGET_ID)) {
			new AlertDialog.Builder(activity).setTitle("警告").setMessage("需要配置PARTNER |APP_ID| RSA_PRIVATE| TARGET_ID")
					.setPositiveButton("确定", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialoginterface, int i) {
						}
					}).show();
			return;
		}

		/**
		 * 这里只是为了方便直接向商户展示支付宝的整个支付流程；所以Demo中加签过程直接放在客户端完成；
		 * 真实App里，privateKey等数据严禁放在客户端，加签过程务必要放在服务端完成；
		 * 防止商户私密数据泄露，造成不必要的资金损失，及面临各种安全风险；
		 * 
		 * authInfo的获取必须来自服务端；
		 */
		Map<String, String> authInfoMap = OrderInfoUtil2_0.buildAuthInfoMap(PID, APPID, TARGET_ID);
		String info = OrderInfoUtil2_0.buildOrderParam(authInfoMap);
		String sign = OrderInfoUtil2_0.getSign(authInfoMap, RSA_PRIVATE);
		final String authInfo = info + "&" + sign;
		Runnable authRunnable = new Runnable() {

			@Override
			public void run() {
//				// 构造AuthTask 对象
//				AuthTask authTask = new AuthTask(activity);
//				// 调用授权接口，获取授权结果
//				Map<String, String> result = authTask.authV2(authInfo, true);
//
//				Message msg = new Message();
//				msg.what = SDK_AUTH_FLAG;
//				msg.obj = result;
//				mHandler.sendMessage(msg);
			}
		};

		// 必须异步调用
		Thread authThread = new Thread(authRunnable);
		authThread.start();
	}

	/**
	 * check whether the device has authentication alipay account.
	 * 查询终端设备是否存在支付宝认证账户
	 * 
	 */
	public void check() {
		Runnable checkRunnable = new Runnable() {

			@Override
			public void run() {
				PayTask payTask = new PayTask(activity);
				// boolean isExist = payTask.checkAccountIfExist();

				Message msg = new Message();
				msg.what = SDK_CHECK_FLAG;
				// msg.obj = isExist;
				mHandler.sendMessage(msg);
			}
		};

		Thread checkThread = new Thread(checkRunnable);
		checkThread.start();

	}

	// /**
	// * 原生的H5（手机网页版支付切natvie支付） 【对应页面网页支付按钮】
	// *
	// * @param v
	// */
	// public void h5Pay(View v) {
	// Intent intent = new Intent(this, H5PayDemoActivity.class);
	// Bundle extras = new Bundle();
	// /**
	// * url是测试的网站，在app内部打开页面是基于webview打开的，demo中的webview是H5PayDemoActivity，
	// * demo中拦截url进行支付的逻辑是在H5PayDemoActivity中shouldOverrideUrlLoading方法实现，
	// * 商户可以根据自己的需求来实现
	// */
	// String url = "http://m.taobao.com";
	// // url可以是一号店或者淘宝等第三方的购物wap站点，在该网站的支付过程中，支付宝sdk完成拦截支付
	// extras.putString("url", url);
	// intent.putExtras(extras);
	// startActivity(intent);
	// }

	/**
	 * get the sdk version. 获取SDK版本号
	 * 
	 */
	public void getSDKVersion() {
		PayTask payTask = new PayTask(activity);
		String version = payTask.getVersion();
		Toast.makeText(activity, version, Toast.LENGTH_SHORT).show();
	}

	/**
	 * create the order info. 创建订单信息
	 *
	 */
	public String getOrderInfo(String subject, String body, String orderID, String price) {

		// 参数编码， 固定值
		String orderInfo = "_input_charset=\"utf-8\"";

		// 商品详情
		orderInfo += "&body=" + "\"" + body + "\"";

		// 设置未付款交易的超时时间
		// 默认30分钟，一旦超时，该笔交易就会自动被关闭。
		// 取值范围：1m～15d。
		// m-分钟，h-小时，d-天，1c-当天（无论交易何时创建，都在0点关闭）。
		// 该参数数值不接受小数点，如1.5h，可转换为90m。
		orderInfo += "&it_b_pay=\"30m\"";

		// 服务器异步通知页面路径
		orderInfo += "&notify_url=" + "\"" + "http://www.game0809.com/Manage/Manage/Pay/Alipay/notify_url.aspx" + "\"";

		// 商户网站唯一订单号
		orderInfo += "&out_trade_no=" + "\"" + orderID + "\"";

		// 合作者身份ID
		orderInfo += "&partner=" + "\"" + PARTNER + "\"";

		// 支付类型， 固定值
		orderInfo += "&payment_type=\"1\"";

		// 卖家支付宝账号
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";

		// 接口名称， 固定值
		orderInfo += "&service=\"mobile.securitypay.pay\"";

		// 商品名称
		orderInfo += "&subject=" + "\"" + subject + "\"";

		// 商品金额
		orderInfo += "&total_fee=" + "\"" + price + "\"";

		// 支付宝处理完请求后，当前页面跳转到商户指定页面的路径，可空
		// orderInfo += "&return_url=\"m.alipay.com\"";

		// 调用银行卡支付，需配置此参数，参与签名， 固定值
		// orderInfo += "&paymethod=\"expressGateway\"";

		return orderInfo;
	}

	/**
	 * get the out_trade_no for an order. 获取外部订单号
	 * 
	 */
	public String getOutTradeNo() {
		SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss", Locale.getDefault());
		Date date = new Date();
		String key = format.format(date);

		Random r = new Random();
		key = key + r.nextInt();
		key = key.substring(0, 15);
		return key;
	}

	/**
	 * sign the order info. 对订单信息进行签名
	 *
	 * @param content
	 *            待签名订单信息
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. 获取签名方式
	 *
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

}
