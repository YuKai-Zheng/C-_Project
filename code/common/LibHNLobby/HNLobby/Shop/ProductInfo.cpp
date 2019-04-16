#include "HNLobby/Shop/ProductInfo.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "cocos2d.h"
USING_NS_CC;

namespace HN {
std::string PRODUCT_INFO::serialize() {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);



    //object.AddMember(rapidjson::Value("identifier", allocator), rapidjson::Value(identifier.c_str(), allocator), allocator);
    //object.AddMember(rapidjson::Value("number", allocator), rapidjson::Value(number), allocator);
    //object.AddMember(rapidjson::Value("price", allocator), rapidjson::Value(price), allocator);
    //object.AddMember(rapidjson::Value("payType", allocator), rapidjson::Value(payType), allocator);
    //object.AddMember(rapidjson::Value("orderID", allocator), rapidjson::Value(orderID.c_str(), allocator), allocator);
    //object.AddMember(rapidjson::Value("IP", allocator), rapidjson::Value(IP.c_str(), allocator), allocator);

    //rapidjson::StringBuffer buffer;
    //rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    //object.Accept(write);
    //return buffer.GetString();

    std::string tmp;
    tmp += "{\"identifier\":\"" + identifier + "\",";
    tmp += "\"number\":" + StringUtils::format("%ld", number) + ",";
    tmp += "\"price\":" + StringUtils::format("%f", price) + ",";
    tmp += "\"payType\":" + StringUtils::format("%d", payType) + ",";
    tmp += "\"orderID\":\"" + StringUtils::format("%s", orderID.c_str()) + "\",";
    tmp += "\"IP\":\"" + StringUtils::format("%s", IP.c_str()) + "\"}";
    return tmp;
}

void PRODUCT_INFO::deserialize(std::string data) {
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    if (!doc.HasParseError()) {
        if (doc.IsObject() && doc.HasMember("status")) {
            identifier = doc["identifier"].GetString();
            number = doc["number"].GetInt64();
            price = doc["price"].GetDouble();
            payType = doc["payType"].GetInt();
            orderID = doc["orderID"].GetString();
            IP = doc["IP"].GetString();
        }
    }
}
}
