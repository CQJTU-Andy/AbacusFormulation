//
// Created by andy on 2022/7/23.
//

#include "division.h"
#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif
#include "subtraction.h"
#include "multiplication.h"

void drawExpressionOfDivision(){
    setFontSizeTo32();
    double diNumber = atof(original_c_first_operand);
    double divNumber = atof(original_c_second_operand);
    stringGenerator<<diNumber<<"/"<<divNumber<<"="<<diNumber*divNumber;
    strcpy(strInfo,stringGenerator.str().c_str());
    drawExpression(strInfo);
    stringGenerator.str("");
    setFontSizeTo16();
}

void drawNumOnAbacusOfDivision(Num* sa){
    cleardevice(); //�����Ļ����
    drawExpressionOfDivision(); // ������ʽ����Ľ��
    drawAbacus(sa,AbacusParams()); //��������
}

void simulateDivision(size_t integerLen1, size_t integerLen2){
    Num* di = a_first_operand;
    Num* div = a_second_operand;
    int headIndexOfDi = PLACES_NUM - integerLen1 - 2;
    int headIndexOfDiv = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = PLACES_NUM - integerLen1 - 2 - 2;
    int currentQuo; //��ǰ���Գ�������
    int currentDi; //��ǰ���Ƚϵı�����
    int firstTwoDi; //������ǰ��λ
    int currentDiv; //��ǰ���Ƚϵĳ���
    int nextDiv; //������һ�����������
    int quoLocation = 1; //�̵�λ�ã�1����������λ�̣�0������������λ��

    //1.��λ
    int sameNumDivOfDi = toNumber(di, headIndexOfDi, PLACES_NUM - integerLen1 - 2 + integerLen2) / pow(10, integerLen1 - integerLen2); //�����ȡ��ͬλ��
    int integerPartOfDiv = toNumber(div, headIndexOfDiv, 12);
    size_t quoNum = sameNumDivOfDi < integerPartOfDiv ? integerLen1 - integerLen2 : integerLen1 - integerLen2 + 1;
    quoNum+=2; //������λС����λ��

    currentDi =oneToNumber(&di[headIndexOfDi]); //����
    firstTwoDi = toNumber(di, headIndexOfDi, headIndexOfDi + 1)/ pow(10, integerLen1 - integerLen2); //��2
    currentDiv = oneToNumber(&div[headIndexOfDiv]); //����
    nextDiv = oneToNumber(&div[headIndexOfDiv+1]); //����
    if(integerLen2==1){
        //2.���̣�1λ����
        if (currentDi < currentDiv){
            currentQuo = firstTwoDi/currentDiv;
            quoLocation = 0;
        }else{
            currentQuo = currentDi/currentDiv;
        }
    }else{
        //2.���̣���λ����
        if (currentDi < currentDiv){ //����<����
            quoLocation = 0;
            if(nextDiv<=4){
                currentQuo = firstTwoDi/currentDiv;
            }else{
                currentQuo = firstTwoDi/(currentDiv+1);
            }
        }else{
            if(nextDiv<=4){
                currentQuo = currentDi/currentDiv;
            }else{
                currentQuo = currentDi/(currentDiv+1);
            }
        }
    }
    //3.����
    setNumToAbacusMulVersion(currentQuo, di, headIndexOfDi - 1 - quoLocation);

    Num product[PLACES_NUM];
    int _product;
    Num* currentMul;
    for (int i = headIndexOfDiv; i < PLACES_NUM; ++i) {  // 1λ�˷��Ļ���λ��� 5*7821
        currentMul = &di[i];
        //��1λ�˷�֮��
        _product = currentQuo*oneToNumber(currentMul);
        lookUpMultiplicationTableDivisionVersion(_product, headIndexOfDiv, product);
        //�����������˻�
        while(allToNumber(product) > toNumber(di,headIndexOfDi,PLACES_NUM-1)){//4.���̣����̹��������� ��ԭΪ���㣬�ñȽϴ��棩
            currentQuo--;
        }
        //5.����
        for (int j = headIndexOfDiv; j < headIndexOfDiv+2; j++){  // ֻ���������Ͻ��м����㣨�����Ͻ�λ���������ʹ�н�λ������������еĵݹ����Ҳ���������⣩
            if(oneToNumber(&product[j]) != 0){
                simulateSubtractionPureVersion(di, product, j);
            }
        }
        //6.����
        while(toNumber(di, headIndexOfDi, headIndexOfDi + 1)/ pow(10, integerLen1 - integerLen2) > toNumber(div, headIndexOfDiv, 12)){
            currentQuo++;
            //��������һ������
            for (int j = headIndexOfDiv; j < headIndexOfDiv+2; j++){
                if(oneToNumber(&div[j]) != 0){
                    simulateSubtractionPureVersion(di, div, j);
                }
            }
        }
    }

}
