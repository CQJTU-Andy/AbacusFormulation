//
// Created by andy on 2022/7/18.
//

#include "multiplication.h"
#include "addition.h"


//#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif

Coordinate multiplicationRhymeDisplayLeftTop={885,150},multiplicationRhymeDisplayRightBottom{1350,183};

//��ʾ���ʽ����Ľ��
void drawExpressionOfMultiplication()
{
    setFontSizeTo32();
    double facNumber = atof(ochar_1operand);
    double mulNumber = atof(ochar_2operand);
    stringGenerator<<facNumber<<"*"<<mulNumber<<"="<<facNumber*mulNumber;
    strcpy(strInfo,stringGenerator.str().c_str());
    drawExpression(strInfo);
    stringGenerator.str("");
    setFontSizeTo16();
}

//����һ��������ʽ
void drawNumOnAbacusOfMultiplication(Num *sa)
{
    cleardevice(); //�����Ļ����
    drawExpressionOfMultiplication(); // ������ʽ����Ľ��
    drawAbacus(sa,AbacusParams()); //��������
//    drawMnemonicRhymeOfMultiplication(); //����
}

void drawNumOnAbacusOfMultiplicationPureVersion(Num *sa)
{
    AbacusParams  param;
    clearAbacus(param);
    drawAbacus(sa,AbacusParams()); //��������
}

/**
 * ģ���žſھ����س˻�
 * @param fac
 * @param mul
 * @param indexOfFirstDigit ���ĵ�һ���������ڵĵ�λ
 * @param product
 */
void lookUpMultiplicationTable(int _product, int indexOfFirstDigit, Num* product){
    if(_product<10){
        setNumToAbacusIndexVersion(_product, product, indexOfFirstDigit + 1); //0���÷��ã�ֱ�ӷ��õڶ�λ
    }else{
        setNumToAbacusIndexVersion(_product / 10, product, indexOfFirstDigit); //���õ�һλ
        setNumToAbacusIndexVersion(_product % 10, product, indexOfFirstDigit + 1); //���õڶ�λ
    }
}

void lookUpMultiplicationTableDivisionVersion(int _product, int indexOfFirstDigit, Num* product){
    if(_product<10){
        setNumToAbacusIndexVersion(_product, product, indexOfFirstDigit); //0���÷��ã�ֱ�ӷ��õڶ�λ
    }else{
        setNumToAbacusIndexVersion(_product / 10, product, indexOfFirstDigit); //���õ�һλ
        setNumToAbacusIndexVersion(_product % 10, product, indexOfFirstDigit + 1); //���õڶ�λ
    }
}

/**
 *
 * @param result
 * @param integerLen1 ������λ��
 * @param integerLen2 ����λ����λ���ٵ���һ����
 */
void simulateMultiplication(Num* result, int integerLen1, int integerLen2){
    //��λ
//    int productDigitNum = oneToNumber(&num_1operand[PLACES_NUM - integerLen1]) *
//                          oneToNumber(&num_2operand[PLACES_NUM - integerLen2]) < 10 ? integerLen1 + integerLen2 - 1 : integerLen1 + integerLen2; //����λ��
    int productDigitNum = integerLen1 + integerLen2; //����λ��
    productDigitNum+=2; //������λС����λ��
    Num* fac = num_1operand;
    Num* mul = num_2operand;
    Num* currentFac; //��������ǰλ
    Num* currentMul; //������ǰλ
    int outerAccumulationPointer=PLACES_NUM-productDigitNum; //1*nλ�˻��ۼӵĴ�λָ��
    int innerAccumulationPointer; // 1λ�˷��ۼӵĴ�λָ��
    Num product[PLACES_NUM]; //1λ�˷�֮��
    int _product;
    stringstream ss; //yet another string builder
    char oneToNExpressionStrInfo[20];
    //�˷�
    for (int i = 0; i < integerLen2 + 2; ++i,outerAccumulationPointer++) {  // 1*nλ�˷��Ļ���λ���
        currentMul = &mul[PLACES_NUM - (integerLen2 + 2) + i];
        if(oneToNumber(currentMul)==0){ //������ǰλΪ��
            if(i==integerLen2){ //��ǰλΪʮ��λ��û��Ҫ�����㷨�ˣ�out
                break;
            }else{ //��ǰλ������λ��������ǰѭ�����ټ���
                continue;
            }
        }
        innerAccumulationPointer = outerAccumulationPointer;

        ss << ochar_1operand << "*" << oneToNumber(currentMul);
        strcpy(oneToNExpressionStrInfo,ss.str().c_str());
        ss.clear();
        ss.str("");

        for (int j = 0; j < integerLen1 + 2; ++j,innerAccumulationPointer++) {  // 1λ�˷��Ļ���λ��� 5*7821
            currentFac = &fac[PLACES_NUM - (integerLen1 + 2) + j];
            //��1λ�˷�֮��
            _product = oneToNumber(currentFac)*oneToNumber(currentMul);
            if(_product==0){  //��������ǰλΪ0����������
                continue;
            }
            lookUpMultiplicationTable(_product, innerAccumulationPointer, product);
            //��ʾ�ۼ�֮��
            stringGenerator<<"��λ�ۼ�"<<oneToNumber(currentFac)<<"*"<<oneToNumber(currentMul)<<"=";
            if(_product<10){
                stringGenerator<<"0"<<_product;
            }
            else{
                stringGenerator<<_product;
            }
            strcpy(strInfo,stringGenerator.str().c_str());
            drawMultiplicationRhymeProduct(oneToNExpressionStrInfo,strInfo);
            stringGenerator.clear();
            stringGenerator.str("");


            //�����ۼӵ������
            _getch();
            for (int k = PLACES_NUM-productDigitNum; k < PLACES_NUM; k++){
                if(oneToNumber(&product[k]) != 0){
                    simulateAdditionPureVersion(result, product, k);
                }
            }

            clearAbacus(product);
            //show result
            drawNumOnAbacusOfMultiplication(result);
        }
    }
}

//��ʾ1*nλ�˷�������ʾ
void draw1toNExpression(const char* str){
    setFontSizeTo32();
    RECT r1 = {multiplicationRhymeDisplayLeftTop.x, multiplicationRhymeDisplayLeftTop.y,
               multiplicationRhymeDisplayRightBottom.x, multiplicationRhymeDisplayRightBottom.y};
    drawtext(str, &r1, DT_WORDBREAK | DT_VCENTER |  DT_LEFT);
    setFontSizeTo16();
}

//��ʾ1λ�˷�������ʾ
void drawMultiplicationRhymeProduct(const char* oneToNExpressionStrInfo,const char *str) {
    draw1toNExpression(oneToNExpressionStrInfo);
    drawRules(str);
}

void simulateMultiplicationPureVersion(Num *fac, Num* mul,Num *result, int integerLen1, int integerLen2) {
    int productDigitNum = integerLen1 + integerLen2; //����λ��
    productDigitNum+=2; //������λС����λ��
    Num* currentFac; //��������ǰλ
    Num* currentMul; //������ǰλ
    int outerAccumulationPointer=PLACES_NUM-productDigitNum; //1*nλ�˻��ۼӵĴ�λָ��
    int innerAccumulationPointer; // 1λ�˷��ۼӵĴ�λָ��
    Num product[PLACES_NUM]; //1λ�˷�֮��
    int _product;
    stringstream ss; //yet another string builder
    //�˷�
    for (int i = 0; i < integerLen2 + 2; ++i,outerAccumulationPointer++) {  // 1*nλ�˷��Ļ���λ���
        currentMul = &mul[PLACES_NUM - (integerLen2 + 2) + i];
        if(oneToNumber(currentMul)==0){ //������ǰλΪ��
            if(i==integerLen2){ //��ǰλΪʮ��λ��û��Ҫ�����㷨�ˣ�out
                break;
            }else{ //��ǰλ������λ��������ǰѭ�����ټ���
                continue;
            }
        }
        innerAccumulationPointer = outerAccumulationPointer;

        for (int j = 0; j < integerLen1 + 2; ++j,innerAccumulationPointer++) {  // 1λ�˷��Ļ���λ��� 5*7821
            currentFac = &fac[PLACES_NUM - (integerLen1 + 2) + j];
            //��1λ�˷�֮��
            _product = oneToNumber(currentFac)*oneToNumber(currentMul);
            if(_product==0){  //��������ǰλΪ0����������
                continue;
            }
            lookUpMultiplicationTable(_product, innerAccumulationPointer, product);
            //�����ۼӵ������
            _getch();
            for (int k = PLACES_NUM-productDigitNum; k < PLACES_NUM; k++){
                if(oneToNumber(&product[k]) != 0){
                    simulateAdditionPureVersion(result, product, k);
                }
            }
            clearAbacus(product);
            drawNumOnAbacusOfMultiplicationPureVersion(result);
        }
    }
}

//# pragma warning (disable:4819)