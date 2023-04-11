/*
 * turnMethods.h
 *
 *  Created on: Dec 15, 2022
 *      Author: User
 */

#ifndef INC_TURNMETHODS_H_
#define INC_TURNMETHODS_H_

#include <stdio.h>
#include<stdbool.h>
#include "main.h"
#include "stm32f1xx_it.h"


static void beAttacked(struct character *A,int value){
	A->HP -= 35 * value / (A->DEF * 0.9);
	blinkRed();
	if (A->HP < 0){
		A->HP = 0;
	}
	//*A表示A指针指向对象的真实值
	//&B表示B的地址
	//务必不要对指针的地址和真实值进行操作
}

static int attackSkill(struct character *A,struct skill *S){
	A->MP -= S->cost;
	return A->ATK * 1.1 * S->value / 35;
}

static void healSkill(struct character *A,struct skill *S){
	A->MP -= S->cost;
	A->HP += S->value;
	if (A->HP > A->HP_max){
		A->HP = A->HP_max;
	}
}

static void upATK(struct character *A, struct skill *S){
	A->MP -= S->cost;
	A->ATK += S->value;
}

static void upDEF(struct character *A, struct skill *S){
	A->MP -= S->cost;
	A->DEF += S->value;
}

static void downATK(struct character *A, struct character *B,struct skill *S){
	A->MP -= S->cost;
	B->ATK -= S->value;
}

static void downDEF(struct character *A, struct character *B,struct skill *S){
	A->MP -= S->cost;
	B->DEF -= S->value;
}

static bool judgeMP(struct character *A,struct skill *S){
	if (A->MP < S->cost)
		return false;
	else return true;
}

static bool judgeCount(struct item *I){
	if (I->count <= 0)
		return false;
	else return true;
}

static bool judgeAlive(struct character *A){
	if (A->HP > 0) {
		return true;
	}else {
		return false;
	}
}

static void useItem(struct character *A, struct item *I){
	I->count--;
	switch (I->type) {
		case 0:
			//恢复HP道具
			A->HP += I->value;
			blinkGreen();
			if (A->HP > A->HP_max){
				A->HP = A->HP_max;
			}
			break;
		case 1:
			//恢复MP道具
			A->MP += I->value;
			if (A->MP > A->MP_max){
				A->MP = A->MP_max;
			}
			break;
		case 2:
			//增加攻击力道具
			A->ATK *= I->value;
			break;
		case 3:
			//增加防御力道具
			A->DEF *= I->value;
		default:
			break;
	}
}

static void normailzeCharacterValue(struct character *A){
	//终局后将角色攻击力防御力恢复到初始态
	A->ATK = A->ATK_origin;
	A->DEF = A->DEF_origin;
}

static void normailzeATKValue(struct character *A){
	//终局后将角色攻击力防御力恢复到初始态
	A->ATK = A->ATK_origin;
}

static void normailzeDEFValue(struct character *A){
	//终局后将角色攻击力防御力恢复到初始态
	A->DEF = A->DEF_origin;
}


static struct character newCharacter(unsigned char *p, int nameLength,int HP, int MP, int ATK, int DEF){
	struct character A;

	for (int var = 0; var < nameLength; var++) {
		A.name[var]=*p++;
	}

	A.ATK = ATK;
	A.ATK_origin = ATK;
	A.DEF = DEF;
	A.DEF_origin = DEF;
	A.HP = HP;
	A.HP_max = HP;
	A.MP = MP;
	A.MP_max = MP;
	return A;
}
static struct skill newSkill(unsigned char *p, int nameLength, int value,int cost,int type){
	struct skill S;
	for (int var = 0; var < nameLength; var++) {
		S.name[var] = *p++;
	}
	S.nameLength = nameLength;
	S.value = value;
	S.cost = cost;
	S.type = type;

	return S;
}

static struct item newItem(unsigned char *p, int nameLength, int value, int count, int type){
	struct item I;
	for (int var = 0; var < nameLength; var++) {
		I.name[var] = *p++;
	}
	I.nameLength = nameLength;
	I.value = value;
	I.count = count;
	I.type = type;

	return I;
}


#endif /* INC_TURNMETHODS_H_ */
