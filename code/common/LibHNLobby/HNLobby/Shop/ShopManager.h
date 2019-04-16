#ifndef ShopManager_h__
#define ShopManager_h__

#include "HNLobby/Shop/ProductInfo.h"
#include "../LibHNMarket/HNMarketExport.h"
#include <vector>
#include <string>

namespace HN {

	// 鏀粯绠＄悊
	class ProductManger
	{
	public:
        static ProductManger* getInstance();

	public:
		ProductManger();
		~ProductManger();

	public:
		void addProducts(const std::string& nameId);

		const PRODUCT_INFO* getProducts(INT index);
        
        const PRODUCT_INFO* getProducts(std::string& productIdentifier);

        UINT getSize() const;

		bool startPay(PRODUCT_INFO* product, CALLBACK_PRAGMA* callback);

		//{
		//	"status": 1
		//}
		bool parsePayResult(const std::string& result);
		bool parsePayResult(const std::string& result, std::string& ext);

	private:
		std::vector<PRODUCT_INFO*> _products;
	};
}

#endif // ShopManager_h__