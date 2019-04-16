package com.jlx.MixProject;

import android.net.Uri;
import android.app.Activity;
import android.content.Intent;

public class QQActivity
{
    private static QQActivity instance;
    public static QQActivity getInstance()
    {
        if (instance == null)
        {
            instance = new QQActivity();
        }
        
        return instance;
    }

    public void SetActivity(Activity activity)
    {
        this.activity = activity;
    }

	// 调用QQ聊天
	public void chatWithQQ(String phoneNumber)
	{
		activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(URL + phoneNumber)));
    }
    
	private String QQ = "1686913913";
	private String URL = "mqqwpa://im/chat?chat_type=wpa&uin=";
    private Activity activity;
}