#ifndef WeChatAndroid_h
#define WeChatAndroid_h

#include <string>
#include "network/HttpClient.h"

extern "C"
{

	class WeChatAndroid{

	public:
		static WeChatAndroid* getInstance();

		bool isAppInstalled();

		void goToWeiXin();

		void goToWeiXinLoginUI();

		void getAccessToken(std::string code);

		void sendRefreshTokenRequest();

		void sendUserInfoRequest(std::string ACCESS_TOKEN, std::string openID);

		std::string emojiFliter(std::string str);

		void shareText(std::string text);

		void shareWebpage(int type, std::string title, std::string describ, std::string webUrl);

		void shareScreenShot(int type);

		void changeScreenDirection(int iDirection);

		void changeScreenDirection(int iDirection,int width,int height);
	};

}
#endif /* WeChatAndroid_h */
