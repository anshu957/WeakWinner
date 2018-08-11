## Info -- format for supplying coupling list, Parameter list and Network Edge List 

1. ** Network Edge List **
    *> csv format
    *> 1st column -- node number 
    *> (2nd,3rd,..) columns -- neighbors indices

2. ** Coupling List **
    *> csv format 
    *> same shape as Network Edge List
    *> 1st column -- always zero unless self coupling 
    *> (2nd,3rd,..) columns -- coupling weights between node i and j where i = row number, j = column number   

3. ** Parameter List **
    *> csv format
    *> rows represents no. of nodes
    *> 1st column -- Parameter1 -- kappa(shear) in this problem 
    *> 2nd column -- Parameter2 -- omega(frequency) in this problem.
