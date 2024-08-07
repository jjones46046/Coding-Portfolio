// CMSC 341 - Fall 2020 - Project 1
#include "csrlist.h"

int main()
{
    try{
        CSR aCSR;
        int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        aCSR.compress(4,6,array1,24);//initialize object aCSR
        CSR bCSR(aCSR);//create bCSR using copy constructor
        CSR cCSR;
        int array2[] = {0,0,0,0,100,200,0,0,300};
        cCSR.compress(3,3,array2,9);//initialize object cCSR
        CSR dCSR(cCSR);//create dCSR using copy constructor

	cout << aCSR.getAt(1,1) << endl;
	cout << aCSR.getAt(0,0) << endl;
	cout << aCSR.getAt(5,7) << endl;

	
        CSRList aCSRList;//create aCSRList
        aCSRList.insertAtHead(aCSR);
        aCSRList.insertAtHead(cCSR);
        CSRList bCSRList;//create bCSRList
        bCSRList.insertAtHead(dCSR);
        bCSRList.insertAtHead(bCSR);
	
   
        cout << endl << "Dumping aCSRList:" << endl;
        aCSRList.dump();
        cout << endl << "Dumping bCSRList:" << endl;
        bCSRList.dump();

	cout << bCSRList.getAt(0,1,1) << endl;
	cout <<bCSRList.getAt(0,3,4) << endl;
    }
    catch (exception &e){
        cout << e.what() << endl;
    }
    return 0;
}
