#include "StringList.h"

int main(){
    cString a;
    cString b("abcd");
    cString c(b);
    char *str = new char[2];
    strcpy(str,"qd");
    
    
    printf("----------------------------------------------------------\n");
    printf("cString: ");
    a.Print();
    printf("\nlength: %d\n", a.Getlength());
    printf("is empty: %d\n", a.Isempty());
    printf("\n");
    
    printf("cString: ");
    b.Print();
    printf("\nlength: %d\n", b.Getlength());
    printf("is empty: %d\n", b.Isempty());
    printf("\n");
    
    printf("cString: ");
    c.Print();
    printf("\nlength: %d\n", c.Getlength());
    printf("is empty: %d\n", c.Isempty());
    
    c.Empty();
    printf("\nempty c: string = ");
    c.Print();
    printf("\n\tsize = %d\n\n", c.Getlength());
    
    printf("string = ");
    b.Print();
    printf("\nSetAt : ch = 'q' nindex = 2\n");
    b.SetAt(2, 'q');
    printf("string = ");
    b.Print();
    
    printf("\n\nCompare: ");
    b.Print();
    printf(" abcd = %d\n\n", b.Compare("abcd"));
    
    printf("Find ch = 'd', str = \"qd\":\n");
    printf("\tstring = ");
    b.Print();
    printf("\n\t'd' - %d, \"qd\" - %d\n\n", b.Find('d'), b.Find(str));
    
    printf("string = ");
    b.Print();
    printf("\n\tMid(2,100): ");
    a = b.Mid(2, 100);
    a.Print();
    printf("\n\tLeft(2): ");
    a = b.Left(2);
    a.Print();
    printf("\n\tRight(3): ");
    a = b.Right(3);
    a.Print();
    
    printf("\nb[1] = %c\n", b[1]);
    printf("\na = ");
    a.Print();
    printf("\nb = ");
    b.Print();
    
    c = a + b;
    printf("\nc = a + b = ");
    c.Print();
    
    a += b;
    printf("\na += b = ");
    a.Print();
    printf("\n");  
    
    printf("----------------------------------------------------------\n");
   
    return 0;
}