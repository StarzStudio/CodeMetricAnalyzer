Deal with cases:
1. Typedef or using (alias) 
2. Dependency analysis distinguish duplicated name by adding mechnism to distinguish namespace difference 
3. distinguish namespace in two cases: 1. ns1::X x1; or 2. using namespace ns1;
4. My code can deal with nested namespace 
5. I distinguish the member function and global function  by specify the type colomn in the type table: Member Function and Global Function
6. detect all the operator function and lambda function, especially deal with operator ()() case
7. detect and record lambda function, whose type is: lambda
8. special case: using A = B; if A is detected in the type table, it will not be considered as dependency evidence because A can be anything defined by the author of that file
9. When analyzing dependency, if a token is found in the type table and if its type is member function, it will be ignored becasue in another file, a symbol  with same name may be defined and it won't cause inconsistency.
10. when makeing type table, main() shouldn't been added into table considered as global function, otherwise every .cpp file will depend on all other .cpp files
