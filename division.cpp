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
    cleardevice(); //清空屏幕内容
    drawExpressionOfDivision(); // 绘制列式计算的结果
    drawAbacus(sa,AbacusParams()); //绘制算盘
}

void simulateDivision(size_t integerLen1, size_t integerLen2){
    Num* di = num_1operand;
    Num* div = num_2operand; //被除数的算盘形式
    double divNum = allToNumber(num_2operand); //被除数
    int diPtr = PLACES_NUM - integerLen1 - 2;
    int divPtr = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = -1;
    int qc; //当前所试出来的商（不带位的形态）
    double realQc; //当前所试出来的商（带位的形态，用于调商）
    int loc; //当前商所在(档位-1)
    size_t quoNum; //商总共有几位（整数部分）
    int qco; //当前商是第几个商
    int firstDi; //当前所比较的被除数
    int firstTwoDi; //被除数前两位
    int firstDiv; //当前所比较的除数
    int secondDiv; //除数下一档所代表的数
    int type; //商的位置：1代表够除，隔位商；0代表不够除，挨位商
    stringstream ss;

    //1.定位
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

    firstDiv = oneToNumber(&div[divPtr]); //除首
    secondDiv = oneToNumber(&div[divPtr + 1]); //次首
    do{
        firstDi =oneToNumber(&di[diPtr]); //被首
        firstTwoDi  = subNumber(di, diPtr, diPtr + 1); //被2

        type = 1;
        for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
            a = oneToNumber(&di[i]);
            b = oneToNumber(&div[j]);
            if(a>b){ //够除隔位商
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
            //2.估商（1位除）
            if (firstDi < firstDiv){
                qc = firstTwoDi / firstDiv;
            }else{
                qc = firstDi / firstDiv;
            }
        }else{
            //2.估商（多位除）
            if (firstDi < firstDiv){ //被首<除首
                if(secondDiv <= 4){
                    qc = firstTwoDi / firstDiv;
                }else{
                    qc = firstTwoDi / (firstDiv + 1);
                }
            }else if(firstDi>firstDiv){ //被首>除首
                if(secondDiv <= 4){
                    qc = firstDi / firstDiv;
                }else{
                    qc = firstDi / (firstDiv + 1);
                }
            }else{ //两首相等，采用”首同后大隔商1，首同后小挨商9“规则估商
                int i = 0;
                while((diPtr<=divPtr?divPtr+1+i<14:diPtr+1+i<14) && (oneToNumber(&di[diPtr+1]) == oneToNumber(&div[divPtr+1]))){
                    i++;
                }
                if(oneToNumber(&di[diPtr+1+i]) >= oneToNumber(&div[divPtr+1+i])){
                    qc = 1;
                }else{
                    qc = 9;
                }
            }
        }
        ss << "估得商为" << qc;
        strcpy(strInfo,ss.str().c_str());
        drawRules(strInfo);
        ss.str("");
        _getch();
        //3.置商
        loc = diPtr - 1 - type;
        if(headIndexOfQuo==-1){ //记录商的开始位置
            headIndexOfQuo = loc;
        }
        qco = loc - headIndexOfQuo + 1; //当前是第几个商数

        setNumToAbacusIndexVersion(qc, di, loc);
        drawNumOnAbacusOfDivision(di);
        _getch();

        Num product[PLACES_NUM];
        double _product;
        realQc = qc * pow(10, (int)quoNum - qco);

        //4.退商：估商过大，须退商 （原为心算，用比较代替）
        while (realQc * divNum > toNumber(di, diPtr, PLACES_NUM - 1)) { //余数不够减当前商数*除数
            ss<<"退商：估商过大，退1";
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

        //5.减积：1位乘法的积错位相减
        double mulDigit; //遍历乘数的每一位（带位的形态）
        char tmp[PLACES_NUM+1];
        for (int i = divPtr; i < PLACES_NUM; ++i) {
            if((realQc - 0.00001) < 0){ //估商都为0了还减个啥积啊，跳过完事了
                break;
            }
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = realQc * mulDigit;
            sprintf(tmp,"%.2f",_product);
            numberToAbacusV2(product,strtod(tmp,nullptr));

            ss<<"减积：减"<<_product;
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            for (int j = diPtr; j < PLACES_NUM; j++) {  // 只会在两档上进行减运算（不算上借位的情况，即使有借位的情况，减法中的递归调用也会解决此问题）
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
        }
        //6.补商：余数 > 除数*商所在位数（个位即为1，十位即为10，十分位即为0.1）
        while(toNumber(di, diPtr, PLACES_NUM - 1) > divNum * (pow(10, (int)quoNum - qco))){
            ss<<"补商：估商过小，加1";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            _getch();

            qc++;
            setNumToAbacusIndexVersion(qc, di, diPtr - 1 - type);
            drawNumOnAbacusOfDivision(di);
            _getch();
            ss<<"补商：减一个除数";
            strcpy(strInfo,ss.str().c_str());
            drawRules(strInfo);
            ss.str("");
            //（从当前商数的位置起）隔位减一个(除数*商所在位数)
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
        while(oneToNumber(&di[diPtr]) == 0){ //跳过被除数为0的位
            ++diPtr;
        }
    }while((toNumber(di, diPtr, PLACES_NUM - 1) - 0.00001) > 0 && (qco - (int)quoNum) < 2); //当余数不为0或未达到预设精度时，继续运算
}

int simulateDivisionImprovedVersion(Num *di, Num *div, size_t integerLen1, size_t integerLen2) {
    double divNum = allToNumber(num_2operand); //除数
    int diPtr = PLACES_NUM - integerLen1 - 2;
    int divPtr = PLACES_NUM - integerLen2 - 2;
    int headIndexOfQuo = -1;
    int qc; //当前所试出来的商（不带位的形态）
    double realQc; //当前所试出来的商（带位的形态，用于调商）
    int loc; //当前商所在(档位-1)
    size_t quoNum; //商总共有几位（整数部分）
    int qco; //当前商是第几个商
    int firstDi; //当前所比较的被除数
    int firstTwoDi; //被除数前两位
    int firstDiv; //当前所比较的除数
    int secondDiv; //除数下一档所代表的数
    int type; //商的位置：1代表够除，隔位商；0代表不够除，挨位商
    stringstream ss;

    //1.定位
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

    firstDiv = oneToNumber(&div[divPtr]); //除首
    secondDiv = oneToNumber(&div[divPtr + 1]); //次首
    do{
        firstDi =oneToNumber(&di[diPtr]); //被首
        firstTwoDi = subNumber(di, diPtr, diPtr + 1); //被2

        type = 1;
        for (int i = diPtr, j =divPtr; (i < PLACES_NUM)&&(j<PLACES_NUM);) {
            a = oneToNumber(&di[i]);
            b = oneToNumber(&div[j]);
            if(a>b){ //够除隔位商
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
            //2.估商（1位除）
            if (firstDi < firstDiv){
                qc = firstTwoDi / firstDiv;
                type = 0;
            }else{
                qc = firstDi / firstDiv;
            }
        }else{
            //2.估商（多位除）
            if (firstDi < firstDiv){ //被首<除首
                type = 0;
                if(secondDiv <= 4){
                    qc = firstTwoDi / firstDiv;
                }else{
                    qc = firstTwoDi / (firstDiv + 1);
                }
            }else if(firstDi>firstDiv){ //被首>除首
                if(secondDiv <= 4){
                    qc = firstDi / firstDiv;
                }else{
                    qc = firstDi / (firstDiv + 1);
                }
            }else{ //两首相等，采用”首同后大隔商1，首同后小挨商9“规则估商
                int i = 0;
                while((diPtr<=divPtr?divPtr+1+i<14:diPtr+1+i<14) && (oneToNumber(&di[diPtr+1]) == oneToNumber(&div[divPtr+1]))){
                    i++;
                }
                if(oneToNumber(&di[diPtr+1+i]) >= oneToNumber(&div[divPtr+1+i])){
                    qc = 1;
                }else{
                    qc = 9;
                }
            }
        }
        //3.置商
        loc = diPtr - 1 - type;

        if(headIndexOfQuo==-1){ //记录商的开始位置
            headIndexOfQuo = loc;
        }
        qco = loc - headIndexOfQuo + 1; //当前是第几个商数

        Num product[PLACES_NUM];
        double _product;
        realQc = qc * pow(10, (int)quoNum - qco);

        //4.退商：估商过大，须退商 （原为心算，用比较代替）
        while (realQc * divNum > toNumber(di, diPtr, PLACES_NUM - 1)) { //余数不够减当前商数*除数
            qc--;
            realQc = qc * pow(10, (int)quoNum - qco);
            setNumToAbacusIndexVersion(qc, di, diPtr - 1 - type);
        }

        //5.减积：1位乘法的积错位相减
        double mulDigit; //遍历乘数的每一位（带位的形态）
        char tmp[PLACES_NUM+1];
        for (int i = divPtr; i < PLACES_NUM; ++i) {
            if((realQc - 0.00001) < 0){ //估商都为0了还减个啥积啊，跳过完事了
                break;
            }
            mulDigit = toNumber(div,i,i);
            if((mulDigit-0.00001) < 0){ // mulDigit = 0
                continue;
            }
            _product = realQc * mulDigit;
            sprintf(tmp,"%.2f",_product);
            numberToAbacusV2(product,strtod(tmp,nullptr));

            for (int j = diPtr; j < PLACES_NUM; j++) {  // 只会在两档上进行减运算（不算上借位的情况，即使有借位的情况，减法中的递归调用也会解决此问题）
                if (oneToNumber(&product[j]) != 0) {
                    simulateSubtractionImprovedPureVersion(di, product, j);
                }
            }
            clearAbacus(product);
        }
        //6.补商：余数 > 除数*商所在位数（个位即为1，十位即为10，十分位即为0.1）
        while(toNumber(di, diPtr, PLACES_NUM - 1) > divNum * (pow(10, (int)quoNum - qco))){
            qc++;
            //（从当前商数的位置起）隔位减一个(除数*商所在位数)
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
        while(oneToNumber(&di[diPtr]) == 0){ //跳过被除数为0的位
            ++diPtr;
        }
    }while(false);
    return qc;
}
