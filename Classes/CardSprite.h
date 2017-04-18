//  
//  CardSprite.h  
//  2048Game  
//  
//  Created by mac on 14-7-16.  
//  
//  

#ifndef ___2048Game__CardSprite__  
#define ___2048Game__CardSprite__  

#include "cocos2d.h"  

class CardSprite :public cocos2d::Sprite {

public:
	// 初始化游戏卡片的方法  
	static CardSprite *createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(CardSprite);

	// 设置数字  
	void setNumber(int num);

	// 获取数字  
	int getNumber();
private:
	// 显示在界面的数字  
	int number;
	void enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);

	// 定义显示数字的控件  
	cocos2d::LabelTTF *labTTFCardNumber;

	// 显示的背景  
	cocos2d::LayerColor *layerColorBG;


};

#endif /* defined(___2048Game__CardSprite__) */  
