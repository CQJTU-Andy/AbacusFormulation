//
// Created by andy on 2022/8/17.
//
#include <iostream>
#include <fstream>
#include "subtraction.h"
using namespace std;
fstream in, out;

bool subtraction();

void GenerateSubtractionTestFile(){
    const char* outF = "subtractionTestFile.txt";
    out.open(outF, ios::out); //���ļ����ӵ��������
    cout.rdbuf(out.rdbuf()); //������������ӵ���׼�������
    for (int mi = 1; mi < 10; ++mi) {
        for (int mei = 1; mei <= mi; ++mei) {
            cout<<mi<<endl<<mei<<endl;
        }
    }
}

void TestSubtraction(){
    const char* inF = "subtractionTestFile.txt";
    in.open(inF,ios::in);
    cin.rdbuf(in.rdbuf());
    while(!cin.eof()){
        subtraction();
    }
}
bool subtraction() {
    int errorHappened;
    int len1,len2;
    int maxLen;
    do{
        errorHappened = 0;
//        printf("�������������Ȳ�����14λ����(�������ͼ���,���������ڼ�����������λС��)��");
//        scanf("%s %s", char_1operand, char_2operand);
        cin>>char_1operand;
        cin>>char_2operand;
        strcpy(ochar_1operand, char_1operand);
        strcpy(ochar_2operand, char_2operand);
        convertToDecimal(char_1operand);
        convertToDecimal(char_2operand);
        len1 = strlen(char_1operand);
        len2 = strlen(char_2operand);
        maxLen = len1 > len2 ? len1 : len2;
        if(!assertCharNumber(ochar_1operand, 13, 2) || !assertCharNumber(ochar_2operand, 13, 2)
           || !assertNonNegative(ochar_1operand) || !assertNonNegative(ochar_2operand)
           || !assertEBigger(ochar_1operand, ochar_2operand)){
//            printf("�������ݲ����Ϲ淶������������\n");
            errorHappened = 1;
        }
    } while(errorHappened);

    numberToAbacusV2(num_1operand, strtod(ochar_1operand, nullptr));
    numberToAbacusV2(num_2operand, strtod(ochar_2operand, nullptr));

    for (int i = 0; i < maxLen; i++){ //�����Ұ�λ���μ���
        if(oneToNumber(&num_2operand[PLACES_NUM - i - 1]) != 0){ //
            simulateSubtractionImprovedPureVersion(num_1operand, num_2operand, PLACES_NUM - i - 1);
        }
    }

    int trueResult = atof(ochar_1operand)- atof(ochar_2operand);
    int finalResult =  allToNumber(num_1operand);
    cout<<ochar_1operand<<"-"<<ochar_2operand<<"="<<trueResult<<",�������"<<finalResult<<".";
    if(trueResult-finalResult==0){
        cout<<"�ɹ�"<<endl;
        return true;
    }
    cout<<"ʧ�ܣ�"<<endl;
    return false;

}
int main(){
//    GenerateSubtractionTestFile();
    TestSubtraction();
    return 0;
}
