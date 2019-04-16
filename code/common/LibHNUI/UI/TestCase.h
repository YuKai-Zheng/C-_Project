#ifndef __H_TESTCASE_H__
#define __H_TESTCASE_H__

#include "Base/HNLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
using namespace extension;
using namespace HN;

namespace HN {
    class TestCase : public HNLayer {
        public:
            TestCase();
            ~TestCase();
            CREATE_FUNC(TestCase);

        public:
            virtual bool init() override;

    };
}
#endif //__H_TESTCASE_H__
