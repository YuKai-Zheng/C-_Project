/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.jlx.MixProject;

import java.lang.reflect.InvocationTargetException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.game.new8le.wxapi.WXConstant;
import com.game.new8le.wxapi.WXEntryActivity;
import com.hn.market.core.HNPhotoModule;
import com.hn.market.export.ChannelExport;
import com.hn.market.export.Constant;
import com.hn.market.export.GameChannel;
import com.tencent.bugly.crashreport.CrashReport;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;


public class AppActivity extends Cocos2dxActivity {

	private static AppActivity context;
	private Class payManagerClass;
	private Object payManagerObject;
	
	private static IWXAPI api;
	
	private static int nSetDirection = -1;
	
	private Handler uiMessageHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case Constant.COMMOAND_PAY:
				payRequest(msg);
				break;
			case Constant.COMMOAND_PAY_RESULT:
				payResponse(msg);
				break;
			case Constant.COMMAND_RESPONSECHANNEL:
				channelResonpse(msg);
				break;
			}
		}
	};

	public static Handler mHandler = new Handler() {

		@SuppressLint("NewApi")
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);

			System.out.println("public static Handler mHandler = new Handler(){");

			String ____context = (String) msg.obj;

			ClipboardManager clipboardManager = (ClipboardManager) context.getSystemService(context.CLIPBOARD_SERVICE);

			ClipData clipData = new ClipData(ClipData.newPlainText(null, ____context));
			clipboardManager.setPrimaryClip(clipData);

			if (clipboardManager.hasText()) {
				clipboardManager.getText();
			}
		}
	};

	// 鏀粯璇锋眰
	public void payRequest(Message msg) {
		PayManager.getInstance().payRequest(msg);
	}

	// 鏀粯杩斿洖
	public void payResponse(Message msg) {
		PayManager.getInstance().payResponse(msg);
	}

	public void updatePayResult() {
		PayManager.getInstance().updatePayResult();
	}

	public void channelResonpse(Message msg) {
		final Object obj = msg.obj;
		Cocos2dxHelper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				GameChannel.getInstance().notifyResponseChannel(obj);
			}

		});
	}

	public static void paste(final String ____context) {
		System.out.println("paste ____context ============== " + ____context);
		context.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				System.out.println(
						" Run context.runOnGLThread(new Runnable() {context.runOnGLThread(new Runnable() {context.runOnGLThread(new Runnable() {context.runOnGLThread(new Runnable() {");

				Message message = new Message();
				message.obj = ____context;
				mHandler.sendMessage(message);
			}
		});
	}
	
	//鐧诲綍寰俊
	public static void loginToWX(){
		if (WXEntryActivity.getInstance().IsInstallWeChat(false)) {
			new Thread(new Runnable(){
				public void run() {
				   	SendAuth.Req req = new SendAuth.Req();  
				   	req.scope = "snsapi_userinfo";  
				   	req.state = WXConstant.wx_code_state;
				   	api.sendReq(req);
				   	Log.e("we_chat", "func_loginToWX");
				}
			}).start();
		}else{
			new Thread(new Runnable(){
				public void run(){
					Looper.prepare();
					Toast.makeText(context, "璇峰厛瀹夎鏈�鏂板井淇″鎴风", Toast.LENGTH_SHORT).show();
					Looper.loop(); 
				}
			}).start();
		}
	}

	public static void chatWithQQ(String phoneNumber)
	{
		QQActivity.getInstance().chatWithQQ(phoneNumber);
	}
	
	public static void changeScreenDirection(int iDirection){
		nSetDirection = iDirection;
		if(iDirection == 0){
			context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}else if(iDirection == 1){
			context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		context = this;

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		GameChannel.getInstance().start(this, uiMessageHandler);

		HNPhotoModule photo = HNPhotoModule.getInstance();
		photo.start(this);
		ChannelExport.getInstance().registerModule(photo.GetModuleName(), photo);

		VertifyCode.getInstance().setActivity(this);
		
		CrashReport.initCrashReport(getApplicationContext(), "900036332", false);
		
		//寰俊鏀粯
		api = WXAPIFactory.createWXAPI(this, null);
		api.registerApp(WXConstant.wx_app_id);
		WXEntryActivity.getInstance().setApi(api);
		WXEntryActivity.getInstance().setActivity(this);
		Log.e("we_chat", "code_registerApp");
//		try {
//			payManagerClass = Class.forName("com.jlx.MixProject.PayManager");
//			payManagerObject = payManagerClass.newInstance();
//			java.lang.reflect.Method setActivityMethod;
//			setActivityMethod = payManagerClass.getMethod("setActivity", AppActivity.class);
//			setActivityMethod.invoke(payManagerObject, new Object[] { AppActivity.this });
//
//			java.lang.reflect.Method setHandleMethod = payManagerClass.getMethod("setHandle", Handler.class);
//			setHandleMethod.invoke(payManagerObject, new Object[] { uiMessageHandler });

			PayManager.getInstance().setActivity(this);
			PayManager.getInstance().setHandle(uiMessageHandler);

			QQActivity.getInstance().SetActivity(this);
			// payManager.setHandle(uiMessageHandler);
//		} catch (ClassNotFoundException e) {
//			e.printStackTrace();
//		} catch (NoSuchMethodException e) {
//			e.printStackTrace();
//		} catch (InstantiationException e) {
//			e.printStackTrace();
//		} catch (IllegalAccessException e) {
//			e.printStackTrace();
//		} catch (IllegalArgumentException e) {
//			e.printStackTrace();
//		} catch (InvocationTargetException e) {
//			e.printStackTrace();
//		}
	}

	@Override
	protected void onResume() {
		Log.e("we_chat", "onResume");
		super.onResume();
		GameChannel.getInstance().onResume(this);
		
		if(nSetDirection == 1)
		{
			changeScreenDirection(nSetDirection);
		}
	}

	@Override
	protected void onPause() {
		Log.e("we_chat", "onPause");
		GameChannel.getInstance().onPause(this);
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		Log.e("we_chat", "onDestroy");
		GameChannel.getInstance().onDestroy();
		super.onDestroy();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// super.onActivityResult(requestCode, resultCode, data);
		GameChannel.getInstance().onActivityResult(requestCode, resultCode, data);

		HNPhotoModule.getInstance().onActivityResult(requestCode, resultCode, data);

		VertifyCode.getInstance().onActivityResult(requestCode, resultCode, data);
	}

}
