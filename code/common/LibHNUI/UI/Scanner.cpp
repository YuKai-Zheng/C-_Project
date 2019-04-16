#include "Scanner.h"

namespace HN {


	static const char* SCANNER_ANI_NODE = "v88/animation/common/scanner/scanner.csb";

	Scanner::Scanner(){
	}

	Scanner::~Scanner() {
	}


	bool Scanner::init() {
		if (!HNNode::init()) {
			return false;
		}
//        scannerEffectNode = CSLoader::createNode(SCANNER_ANI_NODE);
//        addChild(scannerEffectNode);
//        antActionScanner = CSLoader::createTimeline(SCANNER_ANI_NODE);
	
		return true;
	}

	void Scanner::starPlayerEffect()
	{
//        if (scannerEffectNode) {
//            scannerEffectNode->runAction(antActionScanner);
//            antActionScanner->play("scanner", false);
//        }
	}

	

	
}
