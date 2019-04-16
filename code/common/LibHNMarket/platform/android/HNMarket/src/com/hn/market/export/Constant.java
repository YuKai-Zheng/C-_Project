package com.hn.market.export;

public class Constant {
	public static final int COMMOAND_PAY = 1000;
	public static final int COMMOAND_PAY_RESULT = 1001;
	public static final int COMMAND_RESPONSECHANNEL = 10000;
	
	/**微信支付*/
	public static final int SDK_TYPE_WEIXIN = 0;
	/**支付宝支付*/
	public static final int SDK_TYPE_ZHIFUBAO = 1;
	public static final int SDK_TYPE_YINLIAN = 2;
	public static final int SDK_TYPE_XINYONGKA = 3;
	public static final int SDK_TYPE_YEEPAY = 4;

	public class PAY_RESULT {
		int status;
	}
}
