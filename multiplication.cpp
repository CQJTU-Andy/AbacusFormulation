//
// Created by andy on 2022/7/18.
//

#include "multiplication.h"
#include "addition.h"

//#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif

Coordinate multiplicationRhymeDisplayLeftTop={750,150},multiplicationRhymeDisplayRightBottom{1250,183};

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
void lookUpMultiplicationTable(int _product, int placeOfFirstDigit, Num* product){
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
    int _product;
    //�˷�
    for (int i = 0; i < len2; ++i,outerAccumulationPointer++) {  // 1*nλ�˷��Ļ���λ���
        currentMul = &mul[PLACES_NUM-len2+i],
        innerAccumulationPointer = outerAccumulationPointer;
        for (int j = 0; j < len1; ++j,innerAccumulationPointer++) {  // 1λ�˷��Ļ���λ��� 5*7821
            currentFac = &fac[PLACES_NUM-len1+j];
            //��1λ�˷�֮��
            _product = oneToNumber(currentFac)*oneToNumber(currentMul);
            lookUpMultiplicationTable(_product, innerAccumulationPointer, product);

            //��ʾ�ۼ�֮��
            /*stringGenerator<<"��λ�ۼ�";
            if(_product<10){
                stringGenerator<<"0"<<_product;
            }
            else{
                stringGenerator<<_product;
            }
            strcpy(strInfo,stringGenerator.str().c_str());
            drawMultiplicationRhymeProduct(strInfo);
            stringGenerator.clear();
            stringGenerator.str("");*/


            //�����ۼӵ������
            for (int k = PLACES_NUM-productDigitNum; k < PLACES_NUM; k++){
                if(oneToNumber(&product[k]) != 0){
                    simulateAddition(result, product, k);
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

void drawMultiplicationRhymeProduct(const char *str) {
    setFontSizeTo32();
    RECT r1 = {multiplicationRhymeDisplayLeftTop.x, multiplicationRhymeDisplayLeftTop.y,
               multiplicationRhymeDisplayRightBottom.x, multiplicationRhymeDisplayRightBottom.y};
    drawtext(str, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    setFontSizeTo16();
}

//# pragma warning (disable:4819)