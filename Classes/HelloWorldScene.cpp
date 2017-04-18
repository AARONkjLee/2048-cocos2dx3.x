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


	// 获得屏幕可视大小  
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 加入游戏的背景  
	auto layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(180, 170, 160, 255));
	this->addChild(layerColorBG);
	// 在上方加入游戏的分数  
	auto labelTTFCardNumberName = LabelTTF::create("分数:", "HirakakuProN-W6", 80);
	labelTTFCardNumberName->setPosition(Point(visibleSize.width / 5, visibleSize.height - 100));
	addChild(labelTTFCardNumberName);


	labelTTFCardNumber = LabelTTF::create("0", "HirakakuProN-W6", 80);
	labelTTFCardNumber->setPosition(visibleSize.width / 2 + 100, visibleSize.height - 100);
	addChild(labelTTFCardNumber
	);


	// 创建手势识别的事件监听器  
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	// 添加事件监听  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


	// 调用生成卡片的方法  
	createCardSprite(visibleSize);

	// 调用生成随机数  
	autoCreateCardNumber();
	autoCreateCardNumber();
	return true;
}

// 游戏是否还能继续运行下去  
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
		// 结束游戏  
		Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
	}

}

// 自动生成卡片  
void HelloWorld::autoCreateCardNumber() {
	int i = CCRANDOM_0_1() * 4;
	int j = CCRANDOM_0_1() * 4;

	// 判断是否已经存在的位置  
	if (cardArr[i][j]->getNumber() > 0) {
		// 已存在，递归创建  
		autoCreateCardNumber();
	}
	else {
		// 生成2和4的比例是1:9的概率  
		cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
	}
}

// 创建卡片，size为屏幕大小  
void HelloWorld::createCardSprite(cocos2d::Size size) {
	// 求出单元格的宽度和高度，28为左右距离  
	int lon = (size.width - 28) / 4;

	// 4*4的单元格  
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			// 数字0，宽高相同为lon,lon+j+20为卡片X轴位置，如lon+0+20为第一个卡片的位置，20是每张卡片的间隙，lon+i+20+size.height/6代表的意思是屏幕大小竖方向分了六份，我们这里只放4个位置  
			CardSprite *card = CardSprite::createCardSprite(0, lon, lon, lon * j + 10, lon * i + 10 + size.height / 6);
			addChild(card);

			// 添加卡片到二维数组中  
			cardArr[j][i] = card;
		}
	}
}

// 加入手势识别的事件  
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// 触摸点  
	Point touchP0 = touch->getLocation();

	firstX = touchP0.x;
	firstY = touchP0.y;


	return true;
}

// 触摸结束触发  
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// 获取触摸点位置  
	Point touchP0 = touch->getLocation();
	// 获取X轴和Y轴的移动距离  
	endX = firstX - touchP0.x;
	endY = firstY - touchP0.y;

	// 判断X轴和Y轴的移动距离，如果X轴的绝对值大于Y轴的绝对值就是左右否则是上下  
	if (abs(endX) > abs(endY)) {
		// 左右  
		if (endX + 5 > 0) {
			// 左边  
			if (doLeft()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}

		}
		else {
			// 右边  
			if (doRight()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}

	}
	else {
		// 上下  
		if (endY + 5 > 0) {
			// 下边  
			if (doDown()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
		else {
			// 上边  
			if (doUp()) {
				autoCreateCardNumber();
				doCheckGameOver();
			};
		}

	}
}


// 向左  
bool HelloWorld::doLeft() {
	log("doLeft");

	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int x1 = x + 1; x1 < 4; x1++) {
				if (cardArr[x1][y]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);


						// 设置分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;// 跳出  
				}
			}
		}
	}

	return isdo;
}

// 向右  
bool HelloWorld::doRight() {
	log("doRight");

	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {

			// 循环判断左边卡片往右是合并还是清空  
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

						// 设置分数  
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

// 向上  
bool HelloWorld::doUp() {
	log("doUp");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y++;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);

						// 设置分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());

						isdo = true;
					}
					break;// 跳出  
				}
			}
		}
	}

	return isdo;

}

// 向下  
bool HelloWorld::doDown() {
	log("doDown");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);


						// 设置分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(__String::createWithFormat("%i", score)->getCString());

						isdo = true;
					}
					break;// 跳出  
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