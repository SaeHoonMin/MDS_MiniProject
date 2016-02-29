#ifndef __BASEBALL_H__
#define __BASEBALL_H__

void Pin_Init(void);
int Key_Num_Allocation(void);
int Wait_Num();
void Input_Nums(int *Input_Num);	
void Compared_Num(int *Input_Num, int *Ball_Num, int *scnt, int *bcnt, int *ocnt);
void Output_strike_Result(int *scnt);
void Output_Ball_Result(int *bcnt);

#endif