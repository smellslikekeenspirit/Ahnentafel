/*H
*
*File: ahnentafel.c
*Author: Prionti Nasir
*email: pdn3628@rit.edu
*
*H*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define USAGE_MSG_1 "1) description"
#define USAGE_MSG_2 "2) ahnentafel number (base 10)"
#define USAGE_MSG_3 "3) ahnentafel number (base 2)"
#define USAGE_MSG_4 "4) relation (e.g. mother's mother's father)"
#define USAGE_MSG_5 "5) female X chromosome inheritance"
#define USAGE_MSG_6 "6) exit"
#define MAX_FEMALE_ANCESTRY_SIZE 233


///converts a decimal(base-10) number to its binary(base-2) equivalent.
unsigned long int decimalToBinary(int decimal){

    int binary = 0;
    int remainder;
    int unitKeeper = 1;
    while (decimal != 0){
        remainder = decimal%2;
        decimal = decimal/2;
        binary += remainder * unitKeeper;
        unitKeeper *= 10;
    }
    return binary;
}


///converts a binary number(base-2) to its decimal(base-10) equivalent.
int binaryToDecimal(unsigned long int binary){

    int decimal = 0;
    int remainder = 0;
    int unitKeeper = 1;
    while (binary != 0){
        remainder = binary%10;
        binary = binary/10;
        decimal += remainder * unitKeeper;
        unitKeeper *= 2;
    }
    return decimal;
}

int isValidBinary(char number[]){
   unsigned int i;
   for (i = 0; i < strlen(number)-1; i++){
        if (number[i] != '0' && number[i] != '1')
            return 0;
    }
    return 1;
}

///computes the number of generations a given individual is behind
///the starting individual.
int computeGenerationsBack(int decimal){

    return floor(log2(decimal));
} 

unsigned long int relationToAhnen(char *string){
    char *token;
    char ahn[350];
    ahn[0] = '1';
    int i = 1;
    if (strcmp(string, "mother") == 0) return 11;
    if (strcmp(string, "father") == 0) return 10;
    token = strtok(string, " ");
    while (token != NULL){
        if (strcmp(token, "father") == 0 || strcmp(token, "father's") == 0){
            ahn[i] = '0';
        }
        else if (strcmp(token, "mother") == 0 || strcmp(token, "mother's") == 0){
            ahn[i] = '1';
        }
        else if (strcmp(token, "self") == 0){
             return 1;
        }
        else{
            return 0;
        }
        token = strtok(NULL, " ");
        i++;
    }
    return strtol(ahn,NULL,10);
}

char *ahnenToRelation(unsigned long int binary){
    if (binary == 1) return "self";
    if (binary == 10) return "father";
    if (binary == 11) return "mother";
    unsigned long int tmp = binary;
    int len = 0;
    int m;
    while(tmp != 0){
	tmp /= 10;
	len++;
    }
    char binaryString[len];
    sprintf(binaryString, "%ld", binary);
    char rln[400];
    rln[0] = '\0';
    char *relation = rln;
    for (m = 1; m < len; m++){
       if(m+1 == len){
           if(binaryString[m] == '0'){
                strcat(relation, "father");
           }
           if(binaryString[m] == '1'){
                strcat(relation, "mother");
           }

       }

       else{
           if(binaryString[m] == '0'){
	       strcat(relation,"father's ");
	   }
	   if(binaryString[m] == '1'){
		strcat(relation, "mother's ");
	   }
       }

    }
    return relation;

}




///determines if the person is female or not. 
///Used by traceFemaleChromosomes to determine gender of the current
///person and add their x chromosome sources to its array accordingly.
int isFemale(unsigned long int binary){
    return binary%2 != 0;
}


///determines the fibonacci of a given number. This corresponds
///to the number of female sources in the given generation level. 
int fib(int level){
    if (level == 1 || level == 2){
        return 1;
    }else{
        return fib(level-1) + fib(level-2);
    }
}


///determines the sum of fibonacci numbers from the second value
/// i.e. 1 to the given upper bound. That is because the first 1
///in the sequence is not relevant to our sequence of number of
///female x chromosome sources per generation. For 0th level, 
///corresponding to the 0th generation it returns 1.
int sumFib(int level){
    int i;
    int sum = 0;
    if (level == 0) return 1;
    for (i=2; i<= level+2; i++){
        sum += fib(i);
    }
    return sum;
}


///for the starting person, it computes and lists, for the last N generations,
///the ahnentafel numbers of the ancestors to contributed to their lineage
///of x chromosomes. Praise be in thy name, the female condition! 
void traceFemaleChromosomes(int startingPerson, int numberOfGenerations){

   unsigned long int xChromosomeList[MAX_FEMALE_ANCESTRY_SIZE];
   int currentPerson = 0;
   int newPeopleKeeper = 0;
   int size = 1;
   xChromosomeList[currentPerson]  = startingPerson;
   while (1){
      if (size == sumFib(numberOfGenerations)) break;
      if (isFemale(xChromosomeList[currentPerson])){
          unsigned long int father = xChromosomeList[currentPerson] * 10;
          unsigned long int mother = father + 1;
          //THIS. FIX THIS.
          xChromosomeList[newPeopleKeeper+1] = father;
          xChromosomeList[newPeopleKeeper+2] = mother;
          currentPerson++;
          newPeopleKeeper += 2;
          size += 2;
      }else{
          unsigned long int mother = xChromosomeList[currentPerson] * 10 + 1;
          xChromosomeList[newPeopleKeeper+1] = mother;
          currentPerson++;
          newPeopleKeeper++;
          size++;
      }
   }
   xChromosomeList[newPeopleKeeper+1] = '\0';
   unsigned long int *p = xChromosomeList;
   while(1){
       printf("%d", binaryToDecimal(*p++));
       if (*p != '\0'){
           printf(", ");
       }
       else{
           printf("\n");
           break;
       }
   }

}


///the main function, executes the program, handles error-checking 
///and command-line behavior.
int main(int argc, char *argv[]){
        //CASE 1: No command line parameters
        if (argc == 1){
            //loops 
            while(1){

                printf("\n%s\n%s\n%s\n%s\n%s\n%s\n\n> ", USAGE_MSG_1, USAGE_MSG_2,
                USAGE_MSG_3, USAGE_MSG_4, USAGE_MSG_5, USAGE_MSG_6);
                char arg[10];
                fgets(arg,10,stdin);
                arg[strlen(arg)-1] = '\0';
                int choice = strtol(arg, NULL, 10);
                //CASE 1.1: description
                if (choice == 1){
                    printf("The ahnentafel number is used to determine the relation\n");
                    printf("between an individual and each of his/her direct ancestors.\n");
                }
                //CASE 1.2: deci to ahnen and other information
                else if (choice == 2){
                    printf("Enter the ahnentafel number in base-10: ");
                    char buf[350];
                    fgets(buf,350,stdin);
                    buf[strlen(buf)-1] = '\0';
                    if (strtol(buf, NULL, 10) != 0){
                        int dec = strtol(buf, NULL, 10);
                        printf("ahnentafel number in binary: %lu\n", decimalToBinary(dec));
                        printf("family relation: %s\n", ahnenToRelation(decimalToBinary(dec)));
                        printf("generations back: %d\n", computeGenerationsBack(dec));
                    }

                }
                //CASE 1.3: binary to deci and other information
                else if (choice == 3){
                    printf("Enter the ahnentafel number in binary: ");
                    char buf[33];
                    fgets(buf,33,stdin);
                    buf[strlen(buf)-1] = '\0';

                    //CASE 1.1.1: User has entered a valid binary number
                    if (isValidBinary(buf)){
                            unsigned long int bin = strtol(buf, NULL, 10);
                            int dec = binaryToDecimal(bin);
                            printf("base-10 value: %d\n", dec);
                            printf("family relation: %s\n", ahnenToRelation(bin));
                            printf("generations back: %d\n", computeGenerationsBack(dec));
                    }

                    //CASE 1.1.2: User has entered an invalid binary number
                    else{
                        fprintf(stderr, "Invalid binary number!");
                    }
                }
                //CASE 1.4: relation to ahnen
                else if (choice == 4){
                    printf("Enter family relation (e.g.) \"father's mother\": ");
                    char buf[400];
                    fgets(buf,400,stdin);
                    buf[strlen(buf)-1] = '\0';
                    unsigned long int ahn = relationToAhnen(buf);
                    if (ahn == 0){
                        fprintf(stderr, "Invalid string!");
                    }else{
                        printf("ahnentafel number in binary: %lu\n", ahn);
                        printf("ahnentafel number in base-10: %d\n", binaryToDecimal(ahn));
                        printf("generations back: %d\n", computeGenerationsBack(binaryToDecimal(ahn)));
                    }


                 
                }
                //CASE 1.5: produce trace of female x chromosomes till n generations
                else if (choice == 5){
                    int gens;
                    while (1){
                  
                        printf("Number of generations back: ");
                        char buf[33];
                        fgets(buf,33,stdin);
                        buf[strlen(buf)-1] = '\0';
                        gens = strtol(buf, NULL, 10);
                        if (gens > 9 || gens<0){
                            fprintf(stderr, "Invalid input, number of generations must be between 0 and 9!");
                        }
                        else{
                            break;
                        }
                    }
                    printf("Showing %d generations back:\n", gens);
                    traceFemaleChromosomes(1, gens);
                }
                //CASE 1.6: Quit!
                else if (choice == 6){
                    fprintf(stdout,"Goodbye.\n");
                    return EXIT_SUCCESS;
                }
                else{
                    fprintf(stderr,"Unknown operation!\n");
                }

            }
        }

        //CASE 2: User has entered cli parameters
        else{
            //CASE 2.1: User has entered binary number
            if (argv[1][strlen(argv[1])-1] == 'b'){

                //CASE 2.1.1: User has entered a valid binary number
                if (isValidBinary(argv[1])){

                    unsigned long int bin = strtol(argv[1], NULL, 10);
                    int dec = binaryToDecimal(bin);
                    printf("base-10 value: %d\n", dec);
                    printf("family relation: %s\n", ahnenToRelation(bin));
                    printf("generations back: %d\n", computeGenerationsBack(dec));
                }

                //CASE 2.1.2: User has entered an invalid binary number
                else{
                    fprintf(stderr, "Invalid binary number!\n");
                    return EXIT_FAILURE;
                }
            }

            //CASE 2.2: User has entered a decimal number, fully or mixed with chars
            else if (strtol(argv[1], NULL, 10) != 0){
                int dec = strtol(argv[1], NULL, 10);
                printf("ahnentafel number in binary: %lu\n", decimalToBinary(dec));
                printf("family relation: %s\n", ahnenToRelation(decimalToBinary(dec)));
                printf("generations back: %d\n", computeGenerationsBack(dec));
            }
            //CASE 2.3: User has entered string i.e. a family relation
            else{
                int i;
                int invalid = 0;
                char ahn[350];
                if(strcmp("self", argv[1]) == 0){
                    ahn[0] = '1'; 
                }
                else{
                  ahn[0] = '1';
                  for (i = 1; i < argc; i++){
                    if(strcmp("father's",argv[i]) == 0){
			                  ahn[i] = '0';
                        invalid = 1;	
		    }
                    if(strcmp("father",argv[i]) == 0){
                        ahn[i] = '0';
                        invalid = 1;
                    }

		    if(strcmp("mother's", argv[i]) == 0){
                        ahn[i] = '1';
                        invalid = 1;
                    }
                    if(strcmp("mother", argv[i]) == 0){
                        ahn[i] = '1';
                        invalid = 1;
                    }
                    if (invalid == 0){
                        fprintf(stderr,"Invalid string!\n");
                        return EXIT_FAILURE;
                    }
                    invalid = 0;
                  }
                }
                unsigned long int bin = strtol(ahn, NULL, 10);
                printf("\nahnentafel number in binary: %lu\n", bin);
                printf("ahnentafel number in base-10: %d\n", binaryToDecimal(bin)); 
                printf("generations back: %d\n", 
                      computeGenerationsBack(binaryToDecimal(bin)));
           } 
       }
}
