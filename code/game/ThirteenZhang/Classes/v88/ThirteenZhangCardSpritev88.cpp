#include "ThirteenZhangCardSpriteV88.h"

namespace ThirteenZhang {
CardSpriteV88* CardSpriteV88::create(BYTE cardValue) {
    // out of range
    if (!(
                (cardValue >= 0x00 && cardValue <= 0x0D) ||
                (cardValue >= 0x11 && cardValue <= 0x1D) ||
                (cardValue >= 0x21 && cardValue <= 0x2D) ||
                (cardValue >= 0x31 && cardValue <= 0x3D) ||
                (cardValue >= 0x4E && cardValue <= 0x4F))) {
        return nullptr;
    }

    CardSpriteV88* card = new CardSpriteV88();
    if (card && card->initWithCardValue(cardValue)) {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

CardSpriteV88::CardSpriteV88() {

}

CardSpriteV88::~CardSpriteV88() {

}

bool CardSpriteV88::initWithCardValue(BYTE cardValue) {
    if (!Sprite::initWithSpriteFrameName(getCardTextureFileName(cardValue))) {
        return false;
    }
    this->setIgnoreAnchorPointForPosition(false);

    _cardValue = cardValue;

    return true;
}

void CardSpriteV88::setValue(BYTE cardValue) {
    do {
        if (cardValue < 0x00) break;

        if (cardValue > 0x3D) {
            if (cardValue != 0x4E && cardValue != 0x4F) break;
        }

        if (_cardValue != cardValue) {
            _cardValue = cardValue;
            this->setSpriteFrame(getCardTextureFileName(_cardValue));
        }

    } while (0);
}

BYTE CardSpriteV88::getValue() const {
    return _cardValue;
}

std::string CardSpriteV88::getCardTextureFileName(BYTE cardValue) {
    char filename[64] = {0};
    if(PRESSED_TEXTURE) {
		sprintf(filename, "v66/common/card_AMax/%02x.png", cardValue);
    } else {
        sprintf(filename, "%02x.png", cardValue);
    }
    return filename;
}

void CardSpriteV88::setGray(bool gray) {
    if (gray) {
        Sprite* sprite_chess = Sprite::createWithTexture(this->getTexture());
        sprite_chess->setPosition(sprite_chess->getContentSize().width / 2, sprite_chess->getContentSize().height / 2);

        RenderTexture *render = RenderTexture::create(sprite_chess->getContentSize().width, sprite_chess->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        sprite_chess->visit();
        render->end();
        Director::getInstance()->getRenderer()->render();

        Image *finalImage = render->newImage();

        unsigned char *pData = finalImage->getData();

        int iIndex = 0;

        for (int i = 0; i < finalImage->getHeight(); i++) {
            for (int j = 0; j < finalImage->getWidth(); j++) {
                // gray
                int iBPos = iIndex;

                unsigned int iB = pData[iIndex];
                iIndex++;

                unsigned int iG = pData[iIndex];
                iIndex++;

                unsigned int iR = pData[iIndex];
                iIndex++;
                iIndex++;

                unsigned int iGray = 0.3 * iR + 0.6 * iG + 0.1 * iB;

                pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
            }
        }

        Texture2D *texture = new Texture2D;
        texture->initWithImage(finalImage);
        this->setTexture(texture);

        delete finalImage;
        texture->release();
    } else {
        this->setSpriteFrame(getCardTextureFileName(_cardValue));
    }
}
}
