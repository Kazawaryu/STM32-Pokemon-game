/*
 * character_A.h
 *
 *  Created on: Dec 15, 2022
 *      Author: User
 */

#ifndef INC_CHARACTERS_H_
#define INC_CHARACTERS_H_

#include <stdio.h>
#include<stdbool.h>
#include "main.h"
#include "stm32f1xx_it.h"
# include"string.h"

struct character{
	unsigned char name[20];
	int HP_max;
	int MP_max;
	int ATK_origin;
	int DEF_origin;

	int ATK;
	int DEF;
	int HP;
	int MP;
};


struct skill{
	unsigned char name[20];
	int nameLength;

	int value;
	int cost;
	int type;
};

struct item{
	unsigned char name[20];
	int nameLength;

	int value;
	int count;
	int type;
};



#endif /* INC_CHARACTERS_H_ */
