//
// Created by andy on 2022/7/18.
//

#include "multiplication.h"
//#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif

//��ʾ���ʽ����Ľ��
void drawExpressionOfMultiplication()
{
    setFontSizeTo32();
    double facNumber = atof(original_c_first_operand);
    double mulNumber = atof(original_c_second_operand);
    stringGenerator<<facNumber<<"+"<<mulNumber<<"="<<facNumber*mulNumber;
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
//    drawMnemonicRhymeOfMultiplication(); //���Ƽӷ��ھ���
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
    char charProduct[9];
    _itoa_s(_product,charProduct,10);

    if(_product<10){
        setNumToAbacus(_product, product, placeOfFirstDigit);
    }else{
        setNumToAbacus(_product, product, placeOfFirstDigit);
    }
}

//���ӻ�ģ��һλ�ӷ�
void simulateMultiplication(Num* fa, Num* mu, int n){
    Num* fac = &fa[n];
    Num* mul = &mu[n];
    Num* tmp = fac; //��ǰ��������ָ��
    int facNumber = toNumberForm(fac); //������
    int mulNumber = toNumberForm(mul); //����
}

//# pragma warning (disable:4819)