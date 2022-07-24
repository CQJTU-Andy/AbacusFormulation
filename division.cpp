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
    stringGenerator<<diNumber<<"/"<<divNumber<<"="<<diNumber/divNumber;
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
    Num* div = a_second_operand; //��������������ʽ
    double divNum = allToNumber(a_second_operand); //������
    int headIndexOfDi = PLACES_NUM - integerLen1 - 2;
    int headIndexOfDiv = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = PLACES_NUM - integerLen1 - 2 - 2;
    int currentQuo; //��ǰ���Գ������̣�����λ����̬��
    double currentRealQuo; //��ǰ���Գ������̣���λ����̬�����ڵ��̣�
    int currentQuoLocation; //��ǰ������(��λ-1)
    size_t quoNum; //���ܹ��м�λ���������֣�
    int CurrentDigitOfQuo=1; //��ǰ���ǵڼ�����
    int currentDi; //��ǰ���Ƚϵı�����
    int firstTwoDi; //������ǰ��λ
    int currentDiv; //��ǰ���Ƚϵĳ���
    int nextDiv; //������һ�����������
    int quoLocation = 1; //�̵�λ�ã�1����������λ�̣�0������������λ��

    //1.��λ
    int sameNumDivOfDi = toNumber(di, headIndexOfDi, PLACES_NUM - integerLen1 - 2 + integerLen2) / pow(10, integerLen1 - integerLen2); //�����ȡ��ͬλ��
    int integerPartOfDiv = toNumber(div, headIndexOfDiv, 12);
    quoNum = sameNumDivOfDi < integerPartOfDiv ? integerLen1 - integerLen2 : integerLen1 - integerLen2 + 1;
//    quoNum+=2; //������λС����λ��

    currentDiv = oneToNumber(&div[headIndexOfDiv]); //����
    nextDiv = oneToNumber(&div[headIndexOfDiv+1]); //����
    do{
        currentDi =oneToNumber(&di[headIndexOfDi]); //����
        firstTwoDi = subNumber(di, headIndexOfDi, headIndexOfDi + 1); //��2

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
        currentQuoLocation = headIndexOfDi - 1 - quoLocation;
        setNumToAbacusMulVersion(currentQuo, di, currentQuoLocation);
        drawNumOnAbacusOfDivision(di);

        Num product[PLACES_NUM];
        double _product;
        Num* currentMul;

//        currentMul = &div[headIndexOfDiv]; //Ԥ���˻�
//        _product = currentQuo * oneToNumber(currentMul);
//        lookUpMultiplicationTableDivisionVersion(_product, headIndexOfDi, product);
        currentRealQuo = currentQuo*pow(10,(int)quoNum-CurrentDigitOfQuo);

        //4.���̣����̹��������� ��ԭΪ���㣬�ñȽϴ��棩
        while (currentRealQuo*divNum > toNumber(di, headIndexOfDi, PLACES_NUM - 1)) { //������������ǰ����*����
            currentQuo--;
            currentRealQuo = currentQuo*pow(10,(int)quoNum-CurrentDigitOfQuo);
            setNumToAbacusMulVersion(currentQuo, di, headIndexOfDi - 1 - quoLocation);
            drawNumOnAbacusOfDivision(di);
        }

        //5.������1λ�˷��Ļ���λ���
//        int ptr = headIndexOfDi; //��λ���ָ��
        double mulDigit; //����������ÿһλ����λ����̬��
        char tmp[PLACES_NUM+1];
        for (int i = headIndexOfDiv; i < PLACES_NUM; ++i) {  //
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = currentRealQuo * mulDigit;
            sprintf(tmp,"%.2f",_product);
            convertToDecimal(tmp);
            numberToAbacus(product,tmp, strlen(tmp));

            for (int j = headIndexOfDi; j < PLACES_NUM; j++) {  // ֻ���������Ͻ��м����㣨�����Ͻ�λ���������ʹ�н�λ������������еĵݹ����Ҳ���������⣩
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
//            ptr++;
        }
        //6.���̣����� > ����*������λ������λ��Ϊ1��ʮλ��Ϊ10��ʮ��λ��Ϊ0.1��
        while(toNumber(di, headIndexOfDi, PLACES_NUM-1) > divNum*(pow(10,(int)quoNum-CurrentDigitOfQuo))){
            currentQuo++;
            setNumToAbacusMulVersion(currentQuo, di, headIndexOfDi - 1 - quoLocation);
            drawNumOnAbacusOfDivision(di);

            //���ӵ�ǰ������λ���𣩸�λ��һ������
            for (int i = headIndexOfDiv; i < headIndexOfDiv + 2; i++){
                if(oneToNumber(&div[i]) != 0){
                    simulateSubtractionPureVersion(di, div, i);
                }
            }
        }

        while(oneToNumber(&di[++headIndexOfDi])==0);
        CurrentDigitOfQuo++;
    }while(allToNumber(di)!=0);

}
