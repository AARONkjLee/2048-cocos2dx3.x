#include "HelloWorldScene.h"  
#include "CardSprite.h"  

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object  
	auto scene = Scene::create();

	// 'layer' is an autorelease object  
	auto layer = HelloWorld::create();

	// add layer as a child  to scene  
	scene->addChild(layer);

	// return the scene  
	return scene;
}

// on "init" you need to initialize your instance  
bool HelloWorld::init()
{
	//////////////////////////////  
	// 1. super init first  
	if (!Layer::init())
	{
		return false;
	}

	score = 0;


	// �����Ļ���Ӵ�С  
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// ������Ϸ�ı���  
	auto layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(180, 170, 160, 255));
	this->addChild(layerColorBG);
	// ���Ϸ�������Ϸ�ķ���  
	auto labelTTFCardNumberName = LabelTTF::create("����:", "HirakakuProN-W6", 80);
	labelTTFCardNumberName->setPosition(Point(visibleSize.width / 5, visibleSize.height - 100));
	addChild(labelTTFCardNumberName);


	labelTTFCardNumber = LabelTTF::create("0", "HirakakuProN-W6", 80);
	labelTTFCardNumber->setPosition(visibleSize.width / 2 + 100, visibleSize.height - 100);
	addChild(labelTTFCardNumber
	);


	// ��������ʶ����¼�������  
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	// ����¼�����  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


	// �������ɿ�Ƭ�ķ���  
	createCardSprite(visibleSize);

	// �������������  
	autoCreateCardNumber();
	autoCreateCardNumber();
	return true;
}

// ��Ϸ�Ƿ��ܼ���������ȥ  
void HelloWorld::doCheckGameOver() {
	bool isGameOver = true;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (cardArr[x][y]->getNumber() == 0
				|| (x>0 && (cardArr[x][y]->getNumber() == cardArr[x - 1][y]->getNumber()))
				|| (x<3 && (cardArr[x][y]->getNumber() == cardArr[x + 1][y]->getNumber()))
				|| (y<0 && (cardArr[x][y]->getNumber() == cardArr[x][y - 1]->getNumber()))
				|| (x<3 && (cardArr[x][y]->getNumber() == cardArr[x][y + 1]->getNumber()))) {
				isGameOver = false;
			}
		}
	}
	if (isGameOver) {
		// ������Ϸ  
		Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
	}

}

// �Զ����ɿ�Ƭ  
void HelloWorld::autoCreateCardNumber() {
	int i = CCRANDOM_0_1() * 4;
	int j = CCRANDOM_0_1() * 4;

	// �ж��Ƿ��Ѿ����ڵ�λ��  
	if (cardArr[i][j]->getNumber() > 0) {
		// �Ѵ��ڣ��ݹ鴴��  
		autoCreateCardNumber();
	}
	else {
		// ����2��4�ı�����1:9�ĸ���  
		cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
	}
}

// ������Ƭ��sizeΪ��Ļ��С  
void HelloWorld::createCardSprite(cocos2d::Size size) {
	// �����Ԫ��Ŀ�Ⱥ͸߶ȣ�28Ϊ���Ҿ���  
	int lon = (size.width - 28) / 4;

	// 4*4�ĵ�Ԫ��  
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			// ����0�������ͬΪlon,lon+j+20Ϊ��ƬX��λ�ã���lon+0+20Ϊ��һ����Ƭ��λ�ã�20��ÿ�ſ�Ƭ�ļ�϶��lon+i+20+size.height/6�������˼����Ļ��С������������ݣ���������ֻ��4��λ��  
			CardSprite *card = CardSprite::createCardSprite(0, lon, lon, lon * j + 10, lon * i + 10 + size.height / 6);
			addChild(card);

			// ��ӿ�Ƭ����ά������  
			cardArr[j][i] = card;
		}
	}
}

// ��������ʶ����¼�  
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// ������  
	Point touchP0 = touch->getLocation();

	firstX = touchP0.x;
	firstY = touchP0.y;


	return true;
}

// ������������  
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// ��ȡ������λ��  
	Point touchP0 = touch->getLocation();
	// ��ȡX���Y����ƶ�����  
	endX = firstX - touchP0.x;
	endY = firstY - touchP0.y;

	// �ж�X���Y����ƶ����룬���X��ľ���ֵ����Y��ľ���ֵ�������ҷ���������  
	if (abs(endX) > abs(endY)) {
		// ����  
		if (endX + 5 > 0) {
			// ���  
			if (doLeft()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}

		}
		else {
			// �ұ�  
			if (doRight()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}

	}
	else {
		// ����  
		if (endY + 5 > 0) {
			// �±�  
			if (doDown()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
		else {
			// �ϱ�  
			if (doUp()) {
				autoCreateCardNumber();
				doCheckGameOver();
			};
		}

	}
}


// ����  
bool HelloWorld::doLeft() {
	log("doLeft");

	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int x1 = x + 1; x1 < 4; x1++) {
				if (cardArr[x1][y]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);


						// ���÷���  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;
}

// ����  
bool HelloWorld::doRight() {
	log("doRight");

	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {

			// ѭ���ж���߿�Ƭ�����Ǻϲ��������  
			for (int x1 = x - 1; x1 >= 0; x1--) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);

						// ���÷���  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());

						isdo = true;
					}
					break;
				}

			}
		}

	}

	return isdo;

}

// ����  
bool HelloWorld::doUp() {
	log("doUp");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y++;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);

						// ���÷���  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());

						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;

}

// ����  
bool HelloWorld::doDown() {
	log("doDown");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);


						// ���÷���  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());

						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)  
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif  

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
	exit(0);
#endif  
}