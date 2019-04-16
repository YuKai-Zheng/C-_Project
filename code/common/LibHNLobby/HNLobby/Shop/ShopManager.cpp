#include "HNLobby/Shop/ShopManager.h"
#include "HNLobby/PlatformConfig.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <algorithm>
#include <string>
#include <cstdlib>
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "spine/Json.h"

using namespace network;

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

namespace HN {
ProductManger* ProductManger::getInstance() {
    static ProductManger* pruductManager = nullptr;
    if (nullptr == pruductManager) {
        pruductManager = new ProductManger();
        return pruductManager;
    }
    return pruductManager;
}

ProductManger::ProductManger() {
    _products.clear();
}

ProductManger::~ProductManger() {

}

void ProductManger::addProducts(const std::string& nameId) {
    std::string id = nameId;
    const bool supportIAP = PlatformConfig::getInstance()->getSupportIAP();
    if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && supportIAP) {
//        id.append("_IAP");
    }

    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(PlatformConfig::getInstance()->getGoodsListUrl(id).c_str());
    request->setRequestType(HttpRequest::Type::GET);

    request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
        std::vector<char> *buffers = response->getResponseData();
        std::string resp0(buffers->begin(), buffers->end());
        log("addProducts result:%s", resp0.c_str());
        if (response->isSucceed()) {
            std::vector<char> *buffer = response->getResponseData();
            std::string resp(buffer->begin(), buffer->end());
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseDefaultFlags>(resp.c_str());
            if (!doc.IsObject()) {
                return;
            }

            int rs = 0;
            if (JSON_RESOVE(doc, "rs")) rs = doc["rs"].GetInt();

            if (rs) {
                rapidjson::Value& _array = doc["list"];
                if (_array.IsArray()) {
                    _products.clear();
                    for (int i = 0; i < _array.Size(); ++i) {
                        auto product = new PRODUCT_INFO();
                        product->identifier = _array[i]["Remark"].GetString();
                        product->price = atof(_array[i]["PayRMB"].GetString());
                        product->number = atoi(_array[i]["PayChip"].GetString());
                        product->IP = getAlipayNotifyURL();
                        _products.push_back(product);
                    }
                }
            }
        }
    });

    HttpClient::getInstance()->setTimeoutForConnect(20);
    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

UINT ProductManger::getSize() const {
    return (UINT)_products.size();
}

const PRODUCT_INFO* ProductManger::getProducts(INT index) {
    return _products.at(index);
}

const PRODUCT_INFO* ProductManger::getProducts(std::string& identifier) {
    std::vector<PRODUCT_INFO*>::iterator iter = _products.end();
    iter = std::find_if(_products.begin(), _products.end(), [&identifier](PRODUCT_INFO* product) {
        return identifier.compare(product->identifier);
    });
    if (iter != _products.end()) {
        return *iter;
    }
    return nullptr;
}

bool ProductManger::startPay(PRODUCT_INFO *product, CALLBACK_PRAGMA* callback) {
    Operator::sharedOperator()->pay(product->identifier, product->serialize(), callback);
    return true;
}

bool ProductManger::parsePayResult(const std::string& result) {
    bool ret = false;
    do {
        if (result.empty()) break;

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(result.c_str());
        if (doc.HasParseError()) break;

        INT status = 0;
        if (doc.IsObject() && doc.HasMember("status")) {
            status = doc["status"].GetInt();
        }
        ret = (status == 1);
    } while (0);

    return ret;
}

bool ProductManger::parsePayResult(const std::string& result, std::string& ext) {
    bool ret = false;
    do {
        if (result.empty()) break;

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(result.c_str());
        if (doc.HasParseError()) break;

        INT status = 0;
        if (doc.IsObject() && doc.HasMember("status")) {
            status = doc["status"].GetInt();
        }

        if (doc.IsObject() && doc.HasMember("ext")) {
            ext = doc["ext"].GetString();
        }

        ret = (status == 1);
    } while (0);

    return ret;
}
}
