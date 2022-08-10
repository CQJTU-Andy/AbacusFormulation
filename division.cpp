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
    double diNumber = atof(ochar_1operand);
    double divNumber = atof(ochar_2operand);
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
    drawExpressionOfDivision(); // ������ʽ����Ľ��
    drawAbacus(sa,AbacusParams()); //��������
}

void simulateDivision(size_t integerLen1, size_t integerLen2){
    Num* di = num_1operand;
    Num* div = num_2operand; //��������������ʽ
    double divNum = allToNumber(num_2operand); //������
    int diPtr = PLACES_NUM - integerLen1 - 2;
    int divPtr = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = -1;
    int qc; //��ǰ���Գ������̣�����λ����̬��
    double realQc; //��ǰ���Գ������̣���λ����̬�����ڵ��̣�
    int loc; //��ǰ������(��λ-1)
    size_t quoNum; //���ܹ��м�λ���������֣�
    int qco; //��ǰ���ǵڼ�����
    int firstDi; //��ǰ���Ƚϵı�����
    int firstTwoDi; //������ǰ��λ
    int firstDiv; //��ǰ���Ƚϵĳ���
    int secondDiv; //������һ�����������
    int type; //�̵�λ�ã�1����������λ�̣�0������������λ��
    stringstream ss;

    //1.��λ
    double a,b;
    quoNum = integerLen1 - integerLen2 + 1;
    for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
        a = oneToNumber(&di[i]);
        b = oneToNumber(&div[j]);
        if(a>b){
            quoNum = integerLen1 - integerLen2 + 1;
            break;
        }else if(a<b){
            quoNum = integerLen1 - integerLen2;
            break;
        }else{
            i++;j++;
        }
    }
    if(quoNum==0){
        quoNum=1;
    }

    firstDiv = oneToNumber(&div[divPtr]); //����
    secondDiv = oneToNumber(&div[divPtr + 1]); //����
    do{
        firstDi =oneToNumber(&di[diPtr]); //����
        firstTwoDi = subNumber(di, diPtr, diPtr + 1); //��2

        type = 1;
        for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
            a = oneToNumber(&di[i]);
            b = oneToNumber(&div[j]);
            if(a>b){ //������λ��
                type = 1;
                break;
            }else if(a<b){
                type = 0;
                break;
            }else{
                i++;j++;
            }
        }
        if(integerLen2==1){
            //2.���̣�1λ����
            if (firstDi < firstDiv){
                qc = firstTwoDi / firstDiv;
            }else{
                qc = firstDi / firstDiv;
            }
        }else{
            //2.���̣���λ����
            if (firstDi < firstDiv){ //����<����
                if(secondDiv <= 4){
                    qc = firstTwoDi / firstDiv;
                }else{
                    qc = firstTwoDi / (firstDiv + 1);
                }
            }else{
                if(secondDiv <= 4){
                    qc = firstDi / firstDiv;
                }else{
                    qc = firstDi / (firstDiv + 1);
                }
            }
        }
        ss << "������Ϊ" << qc;
        strcpy(strInfo,ss.str().c_str());
        drawRules(strInfo);
        ss.str("");
        _getch();
        //3.����
        loc = diPtr - 1 - type;
        if(headIndexOfQuo==-1){ //��¼�̵Ŀ�ʼλ��
            headIndexOfQuo = loc;
        }
        qco = loc - headIndexOfQuo + 1; //��ǰ�ǵڼ�������

        setNumToAbacusIndexVersion(qc, di, loc);
        drawNumOnAbacusOfDivision(di);
        _getch();

        Num product[PLACES_NUM];
        double _product;
        realQc = qc * pow(10, (int)quoNum - qco);

        //4.���̣����̹��������� ��ԭΪ���㣬�ñȽϴ��棩
        while (realQc * divNum > toNumber(di, diPtr, PLACES_NUM - 1)) { //������������ǰ����*����
            ss<<"���̣����̹�����1";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            qc--;
            realQc = qc * pow(10, (int)quoNum - qco);
            setNumToAbacusIndexVersion(qc, di, diPtr - 1 - type);
            drawNumOnAbacusOfDivision(di);
            _getch();
        }

        //5.������1λ�˷��Ļ���λ���
        double mulDigit; //����������ÿһλ����λ����̬��
        char tmp[PLACES_NUM+1];
        for (int i = divPtr; i < PLACES_NUM; ++i) {
            if((realQc - 0.00001) < 0){ //���̶�Ϊ0�˻�����ɶ����������������
                break;
            }
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = realQc * mulDigit;
            sprintf(tmp,"%.2f",_product);
            numberToAbacusV2(product,strtod(tmp,nullptr));

            ss<<"��������"<<_product;
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            for (int j = diPtr; j < PLACES_NUM; j++) {  // ֻ���������Ͻ��м����㣨�����Ͻ�λ���������ʹ�н�λ������������еĵݹ����Ҳ���������⣩
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
        }
        //6.���̣����� > ����*������λ������λ��Ϊ1��ʮλ��Ϊ10��ʮ��λ��Ϊ0.1��
        while(toNumber(di, diPtr, PLACES_NUM - 1) > divNum * (pow(10, (int)quoNum - qco))){
            ss<<"���̣����̹�С����1";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            qc++;
            setNumToAbacusIndexVersion(qc, di, diPtr - 1 - type);
            drawNumOnAbacusOfDivision(di);
            _getch();
            ss<<"���̣���һ������";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            //���ӵ�ǰ������λ���𣩸�λ��һ��(����*������λ��)
            double mei = divNum*(pow(10, (int)quoNum - qco));
            Num tmpDiv[PLACES_NUM];
            sprintf(tmp,"%.2f",mei);
//            convertToDecimal(tmp);
            numberToAbacusV2(tmpDiv,strtod(tmp,nullptr));
            for (int i = diPtr; i < PLACES_NUM; i++){
                if(oneToNumber(&tmpDiv[i]) != 0){
                    simulateSubtractionPureVersion(di, tmpDiv, i);
                }
            }
        }
        while(oneToNumber(&di[diPtr]) == 0){ //����������Ϊ0��λ
            ++diPtr;
        }
    }while((toNumber(di, diPtr, PLACES_NUM - 1) - 0.00001) > 0 && (qco - (int)quoNum) < 2); //��������Ϊ0��δ�ﵽԤ�辫��ʱ����������
}

int simulateDivisionImprovedVersion(Num *di, Num *div, size_t integerLen1, size_t integerLen2) {
    double divNum = allToNumber(num_2operand); //����
    int diPtr = PLACES_NUM - integerLen1 - 2;
    int divPtr = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = -1;
    int qc; //��ǰ���Գ������̣�����λ����̬��
    double realQc; //��ǰ���Գ������̣���λ����̬�����ڵ��̣�
    int loc; //��ǰ������(��λ-1)
    size_t quoNum; //���ܹ��м�λ���������֣�
    int qco; //��ǰ���ǵڼ�����
    int firstDi; //��ǰ���Ƚϵı�����
    int firstTwoDi; //������ǰ��λ
    int firstDiv; //��ǰ���Ƚϵĳ���
    int secondDiv; //������һ�����������
    int type; //�̵�λ�ã�1����������λ�̣�0������������λ��
    stringstream ss;

    //1.��λ
    double a,b;
    quoNum = integerLen1 - integerLen2 + 1;
    for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
        a = oneToNumber(&di[i]);
        b = oneToNumber(&div[j]);
        if(a>b){
            quoNum = integerLen1 - integerLen2 + 1;
            break;
        }else if(a<b){
            quoNum = integerLen1 - integerLen2;
            break;
        }else{
            i++;j++;
        }
    }
    if(quoNum==0){
        quoNum=1;
    }

    firstDiv = oneToNumber(&div[divPtr]); //����
    secondDiv = oneToNumber(&div[divPtr + 1]); //����
    do{
        firstDi =oneToNumber(&di[diPtr]); //����
        firstTwoDi = subNumber(di, diPtr, diPtr + 1); //��2

        type = 1;
        for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
            a = oneToNumber(&di[i]);
            b = oneToNumber(&div[j]);
            if(a>b){ //������λ��
                type = 1;
                break;
            }else if(a<b){
                type = 0;
                break;
            }else{
                i++;j++;
            }
        }
        if(integerLen2==1){
            //2.���̣�1λ����
            if (firstDi < firstDiv){
                qc = firstTwoDi / firstDiv;
                type = 0;
            }else{
                qc = firstDi / firstDiv;
            }
        }else{
            //2.���̣���λ����
            if (firstDi < firstDiv){ //����<����
                type = 0;
                if(secondDiv <= 4){
                    qc = firstTwoDi / firstDiv;
                }else{
                    qc = firstTwoDi / (firstDiv + 1);
                }
            }else{
                if(secondDiv <= 4){
                    qc = firstDi / firstDiv;
                }else{
                    qc = firstDi / (firstDiv + 1);
                }
            }
        }
        //3.����
        loc = diPtr - 1 - type;

        if(headIndexOfQuo==-1){ //��¼�̵Ŀ�ʼλ��
            headIndexOfQuo = loc;
        }
        qco = loc - headIndexOfQuo + 1; //��ǰ�ǵڼ�������

        Num product[PLACES_NUM];
        double _product;
        realQc = qc * pow(10, (int)quoNum - qco);

        //4.���̣����̹��������� ��ԭΪ���㣬�ñȽϴ��棩
        while (realQc * divNum > toNumber(di, diPtr, PLACES_NUM - 1)) { //������������ǰ����*����
            qc--;
            realQc = qc * pow(10, (int)quoNum - qco);
            setNumToAbacusIndexVersion(qc, di, diPtr - 1 - type);
        }

        //5.������1λ�˷��Ļ���λ���
        double mulDigit; //����������ÿһλ����λ����̬��
        char tmp[PLACES_NUM+1];
        for (int i = divPtr; i < PLACES_NUM; ++i) {
            if((realQc - 0.00001) < 0){ //���̶�Ϊ0�˻�����ɶ����������������
                break;
            }
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = realQc * mulDigit;
            sprintf(tmp,"%.2f",_product);
            numberToAbacusV2(product,strtod(tmp,nullptr));

            for (int j = diPtr; j < PLACES_NUM; j++) {  // ֻ���������Ͻ��м����㣨�����Ͻ�λ���������ʹ�н�λ������������еĵݹ����Ҳ���������⣩
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionImprovedPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
        }
        //6.���̣����� > ����*������λ������λ��Ϊ1��ʮλ��Ϊ10��ʮ��λ��Ϊ0.1��
        while(toNumber(di, diPtr, PLACES_NUM - 1) > divNum * (pow(10, (int)quoNum - qco))){
            qc++;
            //���ӵ�ǰ������λ���𣩸�λ��һ��(����*������λ��)
            double mei = divNum*(pow(10, (int)quoNum - qco));
            Num tmpDiv[PLACES_NUM];
            sprintf(tmp,"%.2f",mei);
            numberToAbacusV2(tmpDiv,strtod(tmp,nullptr));
            for (int i = diPtr; i < PLACES_NUM; i++){
                if(oneToNumber(&tmpDiv[i]) != 0){
                    simulateSubtractionImprovedPureVersion(di, tmpDiv, i);
                }
            }
        }
        while(oneToNumber(&di[diPtr]) == 0){ //����������Ϊ0��λ
            ++diPtr;
        }
    }while(false);
    return qc;
}
