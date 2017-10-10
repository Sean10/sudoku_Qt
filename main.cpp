/*
 *  @file   %{Cpp:License:FileName}
 *  @brief  摘要
 *  Copyright (c) 2017
 */
#include <QCoreApplication>
#include <definition.h>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QVector<int> default_sudoku(81,0); //input the sudoku problem

    SudokuValue = new Sudoku;

    Input(default_sudoku);
    SudokuValue->GetNum(default_sudoku);

    OutPut(SudokuValue->Calculate(), SudokuValue->GetSudokuValue());

    return a.exec();
}
