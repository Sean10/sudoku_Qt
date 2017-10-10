/*
 *  @file   definition
 *  @brief  摘要
 *  Copyright (c) 2017
 */
#ifndef DEFINITION_H
#define DEFINITION_H

#include <QString>
#include <sudoku.h>
#include <iostream>

Sudoku *SudokuValue;
QString text;
bool hasString;

void Input(QVector<int> &tmp_num)
{
    freopen("in.txt","r",stdin);
    for(int i = 0;i < 81; i++)
    {
        std::cin >> tmp_num[i];
    }

//    for(int i = 0; i < 81;i++)
//    {
//        std::cout << -tmp_num[i] << "\t";
//        if((i+1)%9 == 0)
//            std::cout << std::endl;
//    }
}

void OutPut(bool flag_succeed, QVector<int> tmp_num)
{
    if(flag_succeed == 1)
    {
        for(int i = 0; i < 81;i++)
        {
            std::cout << -tmp_num[i] << "\t";
            if((i+1)%9 == 0)
                std::cout << std::endl;
        }
    }else if(flag_succeed == 0){
        std::cout << "There is no answer." << std::endl;
    }
}

#endif // DEFINITION_H
