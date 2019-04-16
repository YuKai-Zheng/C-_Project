#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNMarketExport.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformConfig.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include <string>

static const char* SERVICE_UI_JSON = "platform/serviceUi/ServiceNode.csb";                     //backGround

ServiceLayer::ServiceLayer()
{

}

ServiceLayer::~ServiceLayer()
{

}

void ServiceLayer::showService(Node* parent, Vec2 vec, int zorder, int tag)
{
	_serviceUI.serviceLoader->setPosition(Vec2(vec.x, vec.y - 80));
	auto winSize = Director::getInstance()->getWinSize();
	parent->addChild(this, zorder, tag);
}

void ServiceLayer::closeService()
{
	_serviceUI.serviceLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),  CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));

}

bool ServiceLayer::init()
{
	if (!HNLayer::init()) return false;
	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0,0,0,100));
	addChild(colorLayer,1);
	
	//获取信息
	std::string serviceJson = FileUtils::getInstance()->getStringFromFile("platform/service.json");
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(serviceJson.c_str());
	if (!doc.HasParseError())
	{
		if (doc.IsObject())
		{
			_phoneNumber = doc["phoneNumber"].GetString();
			_msgNumber = doc["msgNumber"].GetString();
			_email = doc["email"].GetString();
		}
	}

	_serviceUI.serviceLoader = CSLoader::createNode(SERVICE_UI_JSON);
	
	_serviceUI.serviceLoader->setScale(0);
	addChild(_serviceUI.serviceLoader, 2, 3);
	_serviceUI.serviceLoader->runAction(ScaleTo::create(0.2f, 1.0f));
	_serviceUI.serviceUILayout = (Layout*) _serviceUI.serviceLoader->getChildByName("Panel_1");
	//电话
	_serviceUI.Button_Tel = (Button*) _serviceUI.serviceUILayout->getChildByName("Button_phone");
	_serviceUI.Button_Tel->addTouchEventListener(CC_CALLBACK_2(ServiceLayer::buttonEventCallBack, this, SERVICE_UI::TEL_TAG));
	//信息
	_serviceUI.Button_Mesg = (Button*) _serviceUI.serviceUILayout->getChildByName("Button_mesg");
	_serviceUI.Button_Mesg->addTouchEventListener(CC_CALLBACK_2(ServiceLayer::buttonEventCallBack, this,SERVICE_UI::MESG_TAG));
	//邮箱
	_serviceUI.Button_FeedBack = (Button*) _serviceUI.serviceUILayout->getChildByName("Button_feedback");
	_serviceUI.Button_FeedBack->addTouchEventListener(CC_CALLBACK_2(ServiceLayer::buttonEventCallBack, this, SERVICE_UI::FEEDBACK_TAG));

	auto bgRect = _serviceUI.serviceUILayout->getBoundingBox();

	//创建一个点击事件
	auto MyListener = EventListenerTouchOneByOne::create();
	MyListener->setSwallowTouches(true);//阻止触摸向下传递
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		// 获取的当前触摸的目标
		auto target = dynamic_cast<ServiceLayer*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			if (bgRect.containsPoint(locationInNode)) return true;
			closeService();
			return true;
		}
		else
		{
			return false;
		}
	};
	//把点击监听添加到监听序列中,因为只创建了一个监听,一个监听只能绑定到一个对象上,所以其他对象要使用监听则克隆
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

	return true;
}

void ServiceLayer::setBGPositon(Vec2 vec)
{
	_serviceUI.serviceUILayout->setPosition(Vec2(vec.x ,vec.y - 100));
}

void ServiceLayer::buttonEventCallBack(Ref* pSender, Widget::TouchEventType type, SERVICE_UI::TAG tag)
{
	if (Widget::TouchEventType::ENDED != type) return;
	switch (tag)
	{
	case SERVICE_UI::TEL_TAG:
		{
			char buffer[128];
			sprintf(buffer, "{\"number\":\"%s\"}", _phoneNumber.c_str());
			Operator::requestChannel("sysmodule", "DealPhone", buffer);
		} break;
	case SERVICE_UI::MESG_TAG:
		{
			//char buffer[128];
			//sprintf(buffer, "{\"number\":\"%s\"}", _msgNumber.c_str());
			//Operator::requestChannel("sysmodule", "DealMsg", buffer);		
			GamePromptLayer::create()->showPrompt(GBKToUtf8("聚乐熊官方客服微信:\n 微信号码:aaa78078\n"));
		} break;
	case SERVICE_UI::FEEDBACK_TAG:
		{
			/*std::string jsData("");
			{
				rapidjson::Document doc;
				doc.SetObject();
				rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
				rapidjson::Value object(rapidjson::kObjectType);
				object.AddMember("to", _email.c_str(), allocator);
				object.AddMember("text", "", allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
				object.Accept(write);
				jsData = buffer.GetString();
			}
			Operator::requestChannel("sysmodule", "SendEmail", jsData);*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			GamePromptLayer::create()->showPrompt(GBKToUtf8("聚乐熊官方客服QQ:\n聚乐熊-乐乐:3306697775(充值)\n聚乐熊-珠珠:2969854667(咨询)\n"));
#else
			GamePromptLayer::create()->showPrompt(GBKToUtf8("聚乐熊官方客服QQ:\n聚乐熊-乐乐:3306697775\n聚乐熊-珠珠:2969854667\n"));
#endif
		} break;
	default:
		break;
	}
}