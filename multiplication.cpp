//
// Created by andy on 2022/7/18.
//

#include "multiplication.h"
#include "addition.h"

#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif

//��ʾ���ʽ����Ľ��
void drawExpressionOfMultiplication()
{
    setFontSizeTo32();
    double facNumber = atof(original_c_first_operand);
    double mulNumber = atof(original_c_second_operand);
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

/**
 * ģ���žſھ����س˻�
 * @param fac
 * @param mul
 * @param placeOfFirstDigit ���ĵ�һ���������ڵĵ�λ
 * @param product
 */
void lookUpMultiplicationTable(int fac, int mul, int placeOfFirstDigit, Num* product){
    int _product = fac * mul;
//    char charProduct[9];
//    _itoa_s(_product,charProduct,10);

    if(_product<10){
        setNumToAbacusMulVersion(_product, product, placeOfFirstDigit+1); //0���÷��ã�ֱ�ӷ��õڶ�λ
    }else{
        setNumToAbacusMulVersion(_product / 10, product, placeOfFirstDigit); //���õ�һλ
        setNumToAbacusMulVersion(_product % 10, product, placeOfFirstDigit+1); //���õڶ�λ
    }
}

/**
 *
 * @param result
 * @param len1 ������λ��
 * @param len2 ����λ����λ���ٵ���һ����
 */
void simulateMultiplication(Num* result, int len1, int len2){
    //��λ
    int productDigitNum = oneToNumber(&a_first_operand[PLACES_NUM - len1]) *
                          oneToNumber(&a_second_operand[PLACES_NUM - len2]) < 10 ? len1 + len2 - 1 : len1 + len2; //����λ��
    productDigitNum+=2; //������λС����λ��
    Num* fac = a_first_operand;
    Num* mul = a_second_operand;
    Num* currentFac = &fac[PLACES_NUM-len1];
    Num* currentMul = &mul[PLACES_NUM-len2];
    int outerAccumulationPointer=PLACES_NUM-productDigitNum; //1*nλ�˻��ۼӵĴ�λָ��
    int innerAccumulationPointer; // 1λ�˷��ۼӵĴ�λָ��
    Num product[PLACES_NUM]; //1λ�˷�֮��
    //�˷�
    for (int i = 0; i < len2; ++i,currentMul = &mul[PLACES_NUM-len2+i],outerAccumulationPointer++) {  // 1*nλ�˷��Ļ���λ���
        innerAccumulationPointer = outerAccumulationPointer;
        for (int j = 0; j < len1; ++j,currentFac = &fac[PLACES_NUM-len1+j],innerAccumulationPointer++) {  // 1λ�˷��Ļ���λ��� 5*7821
            //��1λ�˷�֮��
            lookUpMultiplicationTable(oneToNumber(currentFac), oneToNumber(currentMul), innerAccumulationPointer, product);
            //�����ۼӵ������
            for (int k = 0; k <= productDigitNum; k++){
                if(oneToNumber(&product[PLACES_NUM - k - 1]) != 0){
                    simulateAddition(result, product, PLACES_NUM - k - 1);
                }
            }
            clearAbacus(product);
            //show result
            drawNumOnAbacusOfMultiplication(result);
            double r = allToNumber(result);
            cout<<r<<endl;
        }
    }
}

//# pragma warning (disable:4819)