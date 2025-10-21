


char *p 

int main () {
    int x = 5;          // main () owns x
                // Tiny C Compiler
                // MICRO
    int *p;     // 0x08
    p = &x;
    // Independent
    // Can be changed
    p = p + 4;  // 0x0C
    y = *p;     // NOT gonna be x - break the binding
    // Pointers are dangerous
    p = &z;

    int some_var = 6;

    int &ref = some_var;        // BIND to a variable
    // NOT independent
    ref = ref + 4;      // WRONG!!!
    // CANNOT BE CHANGED
    ref = z;        // WRONG! CANNOT change binding      
    
    
    #define BIT5 ...;
    x |= BIT5;      // SET TO 1


    #define BIT7 ...;
    x &= ~BIT7;         // SET TO 0

    #define BIT3 ...;
    x ^= BIT3;          // TOGGLE


}