//
// Created by andy on 2022/7/23.
//

#include "division.h"
//#define DEBUG
#ifdef DEBUG
#define _getch() ;
#endif
#include "subtraction.h"

void drawExpressionOfDivision(){
    setFontSizeTo32();
    double diNumber = atof(original_c_first_operand);
    double divNumber = atof(original_c_second_operand);
    stringGenerator<<diNumber<<"/"<<divNumber<<"=";
    stringGenerator.precision(3);
    stringGenerator<<diNumber/divNumber;
    stringGenerator.precision(2);
    strcpy(strInfo,stringGenerator.str().c_str());
    drawExpression(strInfo);
    stringGenerator.str("");
    setFontSizeTo16();
}

void drawNumOnAbacusOfDivision(Num* sa){
    cleardevice(); //�����Ļ����
//    AbacusParams  param;
//    clearAbacus(param);
    drawExpressionOfDivision(); // ������ʽ����Ľ��
    drawAbacus(sa,AbacusParams()); //��������
}

void simulateDivision(size_t integerLen1, size_t integerLen2){
    Num* di = a_first_operand;
    Num* div = a_second_operand; //��������������ʽ
    double divNum = allToNumber(a_second_operand); //������
    int headIndexOfDi = PLACES_NUM - integerLen1 - 2;
    int headIndexOfDiv = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = -1;
    int currentQuo; //��ǰ���Գ������̣�����λ����̬��
    double currentRealQuo; //��ǰ���Գ������̣���λ����̬�����ڵ��̣�
    int currentQuoLocation; //��ǰ������(��λ-1)
    size_t quoNum; //���ܹ��м�λ���������֣�
    int currentDigitOfQuo; //��ǰ���ǵڼ�����
    int currentDi; //��ǰ���Ƚϵı�����
    int firstTwoDi; //������ǰ��λ
    int currentDiv; //��ǰ���Ƚϵĳ���
    int nextDiv; //������һ�����������
//    int quoLocation = 1; //�̵�λ�ã�1����������λ�̣�0������������λ�� #���Ƶ�ѭ����
    stringstream ss;

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

        int quoLocation = 1;
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
        ss<<"������Ϊ"<<currentQuo;
        strcpy(strInfo,ss.str().c_str());
        drawRules(strInfo);
        ss.str("");
        _getch();
        //3.����
        currentQuoLocation = headIndexOfDi - 1 - quoLocation;
        if(headIndexOfQuo==-1){ //��¼�̵Ŀ�ʼλ��
            headIndexOfQuo = currentQuoLocation;
        }
        currentDigitOfQuo = currentQuoLocation-headIndexOfQuo+1; //��ǰ�ǵڼ�������

        setNumToAbacusMulVersion(currentQuo, di, currentQuoLocation);
        drawNumOnAbacusOfDivision(di);
        _getch();

        Num product[PLACES_NUM];
        double _product;
        currentRealQuo = currentQuo*pow(10, (int)quoNum - currentDigitOfQuo);

        //4.���̣����̹��������� ��ԭΪ���㣬�ñȽϴ��棩
        while (currentRealQuo*divNum > toNumber(di, headIndexOfDi, PLACES_NUM - 1)) { //������������ǰ����*����
            ss<<"���̣����̹�����1";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            currentQuo--;
            currentRealQuo = currentQuo*pow(10, (int)quoNum - currentDigitOfQuo);
            setNumToAbacusMulVersion(currentQuo, di, headIndexOfDi - 1 - quoLocation);
            drawNumOnAbacusOfDivision(di);
            _getch();
        }

        //5.������1λ�˷��Ļ���λ���
        double mulDigit; //����������ÿһλ����λ����̬��
        char tmp[PLACES_NUM+1];
        for (int i = headIndexOfDiv; i < PLACES_NUM; ++i) {
            if((currentRealQuo-0.00001)<0){ //���̶�Ϊ0�˻�����ɶ����������������
                break;
            }
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = currentRealQuo * mulDigit;
            sprintf(tmp,"%.2f",_product);
            convertToDecimal(tmp);
            numberToAbacus(product,tmp, strlen(tmp));

            ss<<"��������"<<_product;
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            for (int j = headIndexOfDi; j < PLACES_NUM; j++) {  // ֻ���������Ͻ��м����㣨�����Ͻ�λ���������ʹ�н�λ������������еĵݹ����Ҳ���������⣩
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
        }
        //6.���̣����� > ����*������λ������λ��Ϊ1��ʮλ��Ϊ10��ʮ��λ��Ϊ0.1��
        while(toNumber(di, headIndexOfDi, PLACES_NUM-1) > divNum*(pow(10, (int)quoNum - currentDigitOfQuo))){
            ss<<"���̣����̹�С����1";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            currentQuo++;
            setNumToAbacusMulVersion(currentQuo, di, headIndexOfDi - 1 - quoLocation);
            drawNumOnAbacusOfDivision(di);
            _getch();
            ss<<"���̣���һ������";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            //���ӵ�ǰ������λ���𣩸�λ��һ��(����*������λ��)
            double mei = divNum*(pow(10, (int)quoNum - currentDigitOfQuo));
            Num tmpDiv[PLACES_NUM];
            sprintf(tmp,"%.2f",mei);
            convertToDecimal(tmp);
            numberToAbacus(tmpDiv,tmp, strlen(tmp));
            for (int i = headIndexOfDi; i < PLACES_NUM; i++){
                if(oneToNumber(&tmpDiv[i]) != 0){
                    simulateSubtractionPureVersion(di, tmpDiv, i);
                }
            }
        }
        while(oneToNumber(&di[headIndexOfDi])==0){ //����������Ϊ0��λ
            ++headIndexOfDi;
        }
    }while((toNumber(di, headIndexOfDi, PLACES_NUM-1)-0.00001)>0 && (currentDigitOfQuo-(int)quoNum)<2); //��������Ϊ0��δ�ﵽԤ�辫��ʱ����������
}
