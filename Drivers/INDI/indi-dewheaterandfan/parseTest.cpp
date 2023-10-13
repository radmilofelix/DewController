#include "StringParse.h"

int main()
{
//    StringParse stringParse("zero-+one-+two-+three-+four-+five", "-+");
//    StringParse stringParse("zero,one,two,three,four,five", ",");
    StringParse stringParse("zero,one,two,three,four,five");
//    StringParse stringParse("zero-+one-+two-+three-+four-+five");

//*
    printf("\nAll fields\n");
    for (auto i : stringParse.split()) std::cout << i << std::endl;
//*/

//*
    std::vector<std::string> vect = stringParse.split();
    printf("\n3 fields from vector\n");
    printf( "%s\n",vect[0].c_str() );
    printf( "%s\n",vect[2].c_str() );
    printf( "%s\n",vect[1].c_str() );
//*/


//*
    printf("\n3 fields\n");
    printf("%s\n", stringParse.GetElement(2).c_str() );
    printf( "%s\n",stringParse.GetElement(3).c_str() );
    printf( "%s\n",stringParse.GetElement(4).c_str() );
//*/

    return 0;
}
