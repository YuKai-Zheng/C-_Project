package com.jlx.MixProject;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.hn.market.export.Constant;
import com.hn.market.export.GameChannel;
import com.hn.pay.alipay.AlipayManager;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.ProductInfo;

/**
 * 支付管理类
 * 
 * @author yesunsong
 *
 */
public class PayManager {
	private static PayManager manger = null;
	private Activity mActivity;
	private Handler mHandler;
	
	private ProductInfo product;
	private int status = 0;

	private PayManager() {
		product = new ProductInfo();
	}

	public static PayManager getInstance() {
		if (manger == null) {
			manger = new PayManager();
		}
		return manger;
	}

	public void setActivity(Activity activity) {
		Log.e("", "test");
		this.mActivity = activity;
	}

	public void setHandle(Handler handler) {
		this.mHandler = handler;
	}

	// 支付请求
	public void payRequest(Message msg) {
		String data = (String) msg.obj;
		product.deserialize(data);

		if (Constant.SDK_TYPE_ZHIFUBAO == product.payType) {
			AlipayManager alipay = new AlipayManager(mActivity);
			alipay.pay(String.format("%d金币", product.number), String.format("%.2f元购%d金币", product.price, product.number),
					String.format("%.2f", product.price), product.orderID, product.IP, new PayResultCallBack() {

						@Override
						public void onPayResult(PAY_SDK sdk, int status) {

							PayManager.getInstance().status = status;

							switch (sdk) {
							case ALIPAY:

								break;
							case WECHAT:

								break;
							case UNIONPAY:

								break;
							default:
								break;
							}

							if (1 == status) {
								Toast.makeText(mActivity, "支付成功", Toast.LENGTH_SHORT).show();
							} else {
								Toast.makeText(mActivity, "支付失败", Toast.LENGTH_SHORT).show();
							}
							updatePayResult();
						}

					});
		}
	}

	// 支付返回
	public void payResponse(Message msg) {
		final Intent intent = (Intent) msg.obj;
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				GameChannel.getInstance().notifyPayCB(intent);
			}

		});
	}

	public void updatePayResult() {
		Intent intent = new Intent();
		intent.putExtra("status", status);
		intent.putExtra("identifier", product.identifier);

		Message msg = new Message();
		msg.what = Constant.COMMOAND_PAY_RESULT;
		msg.obj = intent;
		mHandler.sendMessage(msg);
	}

}
