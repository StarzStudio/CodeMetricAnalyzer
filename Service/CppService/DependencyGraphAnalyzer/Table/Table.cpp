///////////////////////////////////////////////////////////////////////////
// Table.cpp - define the table to store type information            //
// ver 1.5                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include "Table.h"

//----< test stub >--------------------------------------------------

#ifdef TEST_TABLE

int main()
{
    ////////////////////////////////////
    // test TypeValue class
    ////////////////////////////////////

    TypeValue typeValue1;
    std::vector<std::string> namespaceCollection{ "namespaceTest1" };
    std::string type = "global function";
    std::string fileName = "firstFile.h";
    TypeValue typeValue2(type, namespaceCollection, fileName);

    std::cout << "  test print header : \n";
    std::cout << "  -------------------- \n\n";
    typeValue1.printHeader();

    std::cout << "  the second value is: \n\n";
    typeValue2.print();
    std::cout << "  -------------------- \n\n";

    std::cout << "  after add sth to first value : \n";
    typeValue1.addNamespace("namespace_new");
    typeValue1.setFileName("FirstValueTestFile.h");
    typeValue1.setType("class");
    typeValue1.print();
    std::cout << "  -------------------- \n\n";

    std::cout << "  after add another namespace to first value : \n";
    typeValue1.addNamespace("namespace_new2");
    typeValue1.print();
    std::cout << "  -------------------- \n\n";

    std::cout << "  the namespace size of first value :";
    std::cout << typeValue1.size_Namespace() << std::endl;
    std::cout << "  -------------------- \n\n";

    ////////////////////////////////////
    // test ResultValue class
    ////////////////////////////////////

    ResultValue result1;

    result1.add("ParentFile1");
    result1.add("ParentFile2");
    result1.add("ParentFile3");
    result1.print();
    std::cout << "  -------------------- \n\n";

    ////////////////////////////////////
    // test TypeTable class
    ////////////////////////////////////

    Table<TypeValue> typeTable1;
    // print empty table;
    std::cout << "  test print empty table: empty typetable1 is:\n";
    typeTable1.print();
    std::cout << "  -------------------- \n\n";
    typeTable1.addRecord("gfunc1", typeValue1);
    std::cout << "  new added record's value is: \n";
    typeTable1.getValue("gfunc1").print();
    std::cout << "  new added record's value is(show again using []): ";
    typeTable1["gfunc1"].print();
    std::cout << "  current table content:  \n";
    typeTable1.print();
    std::cout << "  -------------------- \n\n";
    typeTable1.addRecord("gfunc2", typeValue1);
    typeTable1.addRecord("gfunc3", typeValue1);
    typeTable1.addRecord("gfunc4", typeValue1);
    typeTable1.addRecord("gfunc5", typeValue1);
    std::cout << "  after adding new record, table content:  \n";
    typeTable1.print();
    std::cout << "  -------------------- \n\n";
    std::cout << "  delete record with gfunc5, table content:  \n";
    std::cout << "  the size of table: " << typeTable1.size() << std::endl;
    typeTable1.print();
    std::cout << "  -------------------- \n\n";

    //// test merge table
    //ITable<TypeValue>* typeTable2 = new Table<TypeValue>();
    //typeTable2->addRecord("gfunc6", typeValue1);
    //typeTable2->addRecord("gfunc7", typeValue1);
    //typeTable2->addRecord("gfunc8", typeValue1);
    //typeTable2->addRecord("gfunc9", typeValue1);
    ////typeTable1.mergeTables(typeTable2);
    //std::cout << "  merge table2 into table1:  \n";
    //typeTable1.print();

    // test find
    std::cout << "  test find\n";
    std::string TobeFound = "gfunc11";
    if (typeTable1.find(TobeFound))
        std::cout << "  find " + TobeFound + "\n";
    else
        std::cout << "  nothing found : (\n";
    // test copy constructor
    std::cout << "  test copy constructor: \n";
    std::cout << "  -------------------- \n\n";

    ITable<TypeValue>*typeTable3 = new  Table<TypeValue>(*(dynamic_cast<Table<TypeValue>*>(&typeTable1)));
    typeTable3->print();
}

#endif