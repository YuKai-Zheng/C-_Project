package com.game.new8le.wxapi;

import java.io.File;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.jlx.MixProject.AppActivity;
import com.jlx.MixProject.JniTestHelper;
import com.game.new8le.R;
import com.game.new8le.wxapi.WXConstant;
import com.hn.pay.alipay.Util;
import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.tencent.mm.opensdk.modelmsg.WXImageObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXWebpageObject;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Toast;


public class WXEntryActivity extends Cocos2dxActivity implements IWXAPIEventHandler{
	private static WXEntryActivity mInstance = null;
	private static AppActivity activity = null;
	private static IWXAPI api = null;
	
	public static WXEntryActivity getInstance(){
		if(null == mInstance)
		{
			mInstance = new WXEntryActivity();
		}
		return mInstance;
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		mInstance = this;
		
		api = WXAPIFactory.createWXAPI(this, null);
		api.registerApp(WXConstant.wx_app_id);
		api.handleIntent(getIntent(), this);
	}
	
    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        api.handleIntent(intent, this);
    }
    
    public void setApi(IWXAPI tempApi){
    	api = tempApi;
    }
    
    public void setActivity(AppActivity tempActivity){
    	activity = tempActivity;
    }
    
    public static boolean IsInstallWeChat(boolean bTip){
    	Log.e("we_chat", "func_IsInstallWeChat");
    	if(api != null && api.isWXAppInstalled())
    	{
    		Log.e("we_chat", "func_IsInstallWeChat_true");
    		return true;
    	}
    	
    	if(bTip)
    	{
    		Log.e("we_chat", "func_IsInstallWeChat_tip");
        	new Thread(new Runnable(){
    			public void run(){
    				Looper.prepare();
    				Toast.makeText(mInstance, "鐠囧嘲鍘涚�瑰顥婇張锟介弬鏉夸簳娣団�愁吂閹撮顏�?", Toast.LENGTH_SHORT).show();
    				Looper.loop(); 
    			}
    		}).start();
    	}
    	Log.e("we_chat", "func_IsInstallWeChat_false");
    	return false;
    }   
    
    public static void ShareText(String text){
    	if(!IsInstallWeChat(false)){
    		return;
    	}
    }
    
    public static void ShareWebPage(int type, String title, String describ, String webUrl){
    	if(!IsInstallWeChat(false)){
    		return;
    	}
    	Log.e("we_chat", "func_ShareWebPage");
    	
    	WXWebpageObject webpage = new WXWebpageObject();
    	webpage.webpageUrl = webUrl;
    	
    	WXMediaMessage msg = new WXMediaMessage(webpage);
    	msg.title = title;
    	msg.description = describ;
    	
    	int WX_THUMB_SIZE = 120;
    	Bitmap bmp = BitmapFactory.decodeResource(mInstance.getContext().getResources(), R.drawable.icon);
    	Bitmap newBmp = Bitmap.createScaledBitmap(bmp, WX_THUMB_SIZE, WX_THUMB_SIZE, true);	
    	msg.thumbData = Util.bmpToByteArray(newBmp, true);
    	
    	SendMessageToWX.Req req = new SendMessageToWX.Req();
    	req.transaction = "webpage";
    	req.message = msg;
    	if(type == 0){
    		req.scene = SendMessageToWX.Req.WXSceneTimeline;
    	}else if(type == 1){
    		req.scene = SendMessageToWX.Req.WXSceneSession;	
    	}
    	api.sendReq(req);
    	
    	newBmp.recycle();
    }
    
    public static void ShareScreenShot(int type, String filePath){
    	if(!IsInstallWeChat(false)){
    		return;
    	}
    	
    	Log.e("we_chat", "func_ShareScreenShot");
    	
    	if(filePath.isEmpty())
    	{
    		Log.e("we_chat", "func_ShareScreenShot_filepath_empty");
    		return;
    	}
    	
    	File file = new File(filePath);
    	if (!file.exists()) {
    		Log.e("we_chat", "func_ShareScreenShot_file_null");
    		return;
    	}
    	
    	Bitmap bmp = BitmapFactory.decodeFile(filePath);

    	WXImageObject imgObj = new WXImageObject(bmp);
    	WXMediaMessage msg = new WXMediaMessage();
    	msg.mediaObject = imgObj;
    	
    	Bitmap newBmp = Bitmap.createScaledBitmap(bmp, 120, 60, true);
    	msg.thumbData = Util.bmpToByteArray(newBmp, true);
    	
    	SendMessageToWX.Req req = new SendMessageToWX.Req();
    	req.transaction = "picture";
    	req.message = msg;
    	if(type == 0){
    		req.scene = SendMessageToWX.Req.WXSceneTimeline;
    	}else if(type == 1){
    		req.scene = SendMessageToWX.Req.WXSceneSession;	
    	}
    	api.sendReq(req);
    	
    	newBmp.recycle();
    } 
    
	@Override
	public void onReq(BaseReq arg0) {
		Log.e("we_chat", "func_onReq");
	}

	@Override
	public void onResp(BaseResp resp) {
		Log.e("we_chat", "func_onResp");
		
		switch (resp.errCode) {
        case BaseResp.ErrCode.ERR_OK:
        	 switch (resp.getType()) {
             case ConstantsAPI.COMMAND_SENDAUTH:
                 //閻ц缍嶉崶鐐剁殶,婢跺嫮鎮婇惂璇茬秿閹存劕濮涢惃鍕拷鏄忕�?
            	 Log.e("we_chat", "func_onResp_login_success");
            	 
         		//闂堢偞纭堕崶鐐插�?
         		if(!((SendAuth.Resp) resp).state.equals(WXConstant.wx_code_state))
         		{
         			Log.e("we_chat", "func_onResp_error");
         			return;
         		}
            	 
            	 String code = ((SendAuth.Resp) resp).code;
     	    	 JniTestHelper.getAccessToken(code);
                 break;
             case ConstantsAPI.COMMAND_SENDMESSAGE_TO_WX:
                 //閸掑棔闊╅崶鐐剁�?,婢跺嫮鎮婇崚鍡曢煩閹存劕濮涢崥搴ｆ畱闁槒绶�?
            	 Log.e("we_chat", "func_onResp_share_success");
                 break;
             default:
                 break;
        	 }
            break;
        case BaseResp.ErrCode.ERR_USER_CANCEL:
        	//Toast.makeText(this, "閸欐牗绉烽幒鍫熸�?", Toast.LENGTH_SHORT).show();
        	Log.e("we_chat", "func_onResp_cancel");
            break;
        case BaseResp.ErrCode.ERR_AUTH_DENIED:
        	//Toast.makeText(this, "閹哄牊娼堢悮顐ｅ珕缂侊拷", Toast.LENGTH_SHORT).show();
        	Log.e("we_chat", "func_onResp_denien");
            break;
        default:
        	//Toast.makeText(this, "閺堫亞鐓￠柨娆掝�?", Toast.LENGTH_SHORT).show();
        	Log.e("we_chat", "func_onResp_unknow");
            break;
        }
	}

}
