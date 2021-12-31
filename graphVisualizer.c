//Including libraries
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//Declaring Global Variables
int rows = 0;
char* fname;
char** names;

//Declaring functions
void menu4Selection();
void subMenuSelection();

//Defining max function
int max(int a,int b) {
	if(a>b){
        return a;
    }
	else{
        return b;
    }
}

//
void equiClasses(int arr[rows][rows]){
    int a[rows];
    int e[rows][rows];
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            e[i][j]=0;
        }
    }
    for(int i=0;i<rows;i++){
        a[i]=1;
    }
    for(int i=0;i<rows;i++){
        if(a[i]==0) continue;
        for(int j=i;j<rows;j++){
            if(arr[i][j]==1){
                e[i][j]=1;
                a[j]=0;
            }
        }
    }
    for(int i=0;i<rows;i++){
        if(e[i][i]==0) continue;
        else{
            printf("{ ");
            for(int j=i;j<rows;j++){
                if(e[i][j]==1){
                    printf("%s, ", names[j]);
                }
            }
            printf("}\n");
        }
    }
}

//Making the reflexive closure of a given matrix
void reflexiveClosure(int matrix[rows][rows], int refMat[rows][rows]){
    memcpy(refMat, matrix, rows * rows * sizeof(int));
    for(int i = 0; i < rows; i++){
        refMat[i][i] = 1;
    }
}

void transitiveClosure(int matrix[rows][rows], int transMat[rows][rows]){
    memcpy(transMat, matrix, rows * rows * sizeof(int));
    for(int k = 0; k < rows; k++){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < rows; j++){
                transMat[i][j] = max(transMat[i][j], transMat[i][k] && transMat[k][j]);
            }
        }
    }
}

void makeHasMat(int hasMat[rows][rows]){

    for(int i = 0; i < rows; i++){
        hasMat[i][i] = 0;
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(hasMat[i][j] == 1){
                for(int k = 0; k < rows; k++){
                    if(k != i && k != j && hasMat[i][k] && hasMat[k][j]){
                        hasMat[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }
}

//Plotting the graph
void plot(char * fname){
    int pid;
    if((pid = fork())==0){
        if(execlp("python", "python","visualise.py",fname,(char*)NULL)==-1){
        	execlp("python3", "python3","visualise.py",fname,(char*)NULL);
        };
    }
    exit(0);
}

//Generating the output file with the given matrix
void makeFile(int temp[rows][rows]){
    //Opening the files
    FILE* fileptr = fopen(fname, "r");
    FILE* tempFile = fopen("SampleOutput.csv", "w");

    //Writing the first line as it is
    char buffer[1024];
    fgets(buffer, 1024, fileptr);
    fwrite(buffer, sizeof(char), strlen(buffer), tempFile);

    //Tokenizing the buffered string and writing to the file
    char* token = strtok(buffer, ",");

    for(int i = 0; i < rows; i++){
        char tempBuffer[strlen(token) + 100];
        strcpy(tempBuffer, token);
        if(i == rows - 1){
            tempBuffer[strlen(tempBuffer) - 2] = '\0';
        }
        for(int j = 0; j < rows; j++){
            strcat(tempBuffer, ",");
            char tempStr[10]; 
            tempStr[0] = (char)(temp[i][j] + '0'); 
            strcat(tempBuffer, tempStr);
        }
        strcat(tempBuffer, "\n");
        fwrite(tempBuffer, sizeof(char), strlen(tempBuffer), tempFile);
        token = strtok(NULL, ",");
    }
    //Closing the files
    fclose(fileptr);
    fclose(tempFile);
}

void mainMenu(){
    printf("Main Menu\n");
    printf("1. Does every website has a link to itslef?\n");
    printf("2. Is it possible to always return back to the previous website from the current website in one step?\n");
    printf("3. Does every website has all the links to the websites which are reachable to it?\n");
    printf("4. Does there exist any website that contains a link to itself?\n");
    printf("5. Is it impossible to return to the previous website from the current website in one step?\n");
    printf("6. Is it impossible to return to the previous website from the current website in one step?(excluding the cases in where the current and the previous website is the same)\n");
    printf("7. Is it possible to divide the network into multiple pieces such that every website in a piece is reachable from every other website in that piece?\n");
    printf("8. Is this relation an example of poset?\n");
    printf("9. Exit\n");
}

void menu2(){
    printf("Do you want to visualise how the network will look if we add minimum links to satisfy the property?[y/n]\n");
}

void menu3(){
    printf("Do you want to know the nodes in each piece?\n");
}

void menu4(){
    printf("1. Display the hasse diagram.\n");
    printf("2. Display the website whose link is available in every website.\n");
    printf("3. Display the website which has the link for every website.\n");
    printf("4. Display the websites that do not have links to any other website except itself\n");
    printf("5. Display the websites which cant be reached from any other website except itself\n");
    printf("6. Given some websites, display the websites which are reachable from all of them\n");
    printf("7. Given some websites display the websites from which you can reach all of those websites\n");
    printf("8. Is this relation an example of a lattice.\n");
    printf("9. Return to Main Menu.\n");
}

void menu5(){
    printf("1. Given two websites A and B, display the website which is reachable by both A and B and can also reach to all such websites that both A and B can reach.\n");
    printf("2. Given two websites A and B, display the website which can reach to both A and B and is also reachable from all such websites which can reach to both A and B.\n");
    printf("3. Is the lattice distributive\n");
    printf("4. Return to Menu 4\n");
}

void countRows(char *fname){
    //Counting the number of rows
    FILE *fileptr;
    fileptr = fopen(fname, "r");
    char chr = getc(fileptr);
    while (chr != EOF)
    {
        if (chr == '\n')
        {
            rows++;
        }
        chr = getc(fileptr);
    }
    fclose(fileptr);
    rows--;
}

void loadFile(char *fname, int matrix[rows][rows]){
    //Loading the file contents into the matrix
    names = (malloc(sizeof(char*) * rows));
    for(int i = 0; i < rows; i++){
        names[i] = (char*)(malloc(100));
    }
    FILE *fileptr;
    char buffer[1000];
    fileptr = fopen(fname, "r");
    fgets(buffer, 1000, fileptr);
    for(int i = 0; i < rows; i++){
        fgets(buffer, 1000, fileptr);
        char *token = strtok(buffer, ",");
        strcpy(names[i], token);
        for(int j = 0; j < rows; j++){
            token = strtok(NULL, ",");
            matrix[i][j] = (int) (token[0] - '0');
        }
    }  
    fclose(fileptr); 
}

void mainChoice1(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        if(matrix[i][i] == 0){
            printf("No\n");
            menu2();
            char select;
            scanf(" %c", &select);
            if(select == 'y'){
                int refMat[rows][rows];
                reflexiveClosure(matrix, refMat);
                makeFile(refMat);
                plot("SampleOutput.csv");
            }
            return;
        }
    }
    printf("Yes\n");
}

void mainChoice2(int matrix[rows][rows]){
    //Making a transpose of the given matrix
    int transpose[rows][rows];
    memcpy(transpose, matrix, sizeof(int) * rows * rows);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            transpose[i][j] = matrix[j][i];
        }
    }

    //Checking if the transpose is equal to the given matrix
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            //If transpose is not equal bring up menu2
            if(matrix[i][j] != transpose[i][j]){
                printf("No\n");
                menu2();
                char select;
                scanf(" %c", &select);
                if(select == 'y'){
                    //If user wants to see then generate and plot a symmetric matrix
                    for(int a = 0; a < rows; a++){
                        for(int b = 0; b < rows; b++){
                            if(transpose[a][b] == 1){
                                transpose[b][a] = 1;
                            }
                        }
                    }
                    makeFile(transpose);
                    plot("SampleOutput.csv");
                }
                return;
            }
        }
    }
    printf("Yes\n");
}

void mainChoice3(int matrix[rows][rows]){
    int transMat[rows][rows];
    transitiveClosure(matrix, transMat);
    int flag = 1;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(matrix[i][j] != transMat[i][j]){
                flag--;
                break;
            }
        }
        if(!flag){
            break;
        }
    }
    if(flag){
        printf("Yes\n");
    }
    else{
        menu2();
        char choice;
        scanf(" %c", &choice);
        if(choice == 'y'){
            makeFile(transMat);
            plot("SampleOutput.csv");
        }
    }
}

void mainChoice4(int matrix[rows][rows]){
    //Checking is any diagonal element is 1
    for(int i = 0; i < rows; i++){
        if(matrix[i][i] == 1){
            printf("Yes\n");
            return;
        }
    }
    printf("No\n");
}

void mainChoice5(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
                if(matrix[i][j] == 1 && matrix[j][i] == 1){
                    printf("No\n");
                    return;
                }
        }
    }
    printf("Yes\n");
}

void mainChoice6(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
                if(matrix[i][j] == 1 && matrix[j][i] == 1 && i != j){
                    printf("No\n");
                    return;
                }
        }
    }
    printf("Yes\n");
}

void mainChoice7(int matrix[rows][rows]){
    int score = 1;
    int transMat[rows][rows];
    transitiveClosure(matrix, transMat);
    for(int i = 0; i < rows; i++){
        if(!matrix[i][i]){
            score--;
            goto out;
        }
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(matrix[i][j] != matrix[j][i]){
                score--;
                goto out;
            }
        }
    }

    
    int flag = 1;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(matrix[i][j] != transMat[i][j]){
                flag--;
                goto out;
                break;
            }
        }
    }

    out:
        if(score == 1){
            printf("Yes\n");
            menu3();
            char choice;
            scanf(" %c", &choice);
            if(choice == 'y'){
                equiClasses(matrix);
            }
        }
        else{
            printf("No\n");
            menu2();
            char choice;
            scanf(" %c", &choice);
            if(choice == 'y'){
                int equiMat[rows][rows];
                memcpy(equiMat, matrix, sizeof(int) * rows * rows);
                for (int i = 0; i < rows; i++){
                    equiMat[i][i] = 1;
                }
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < rows; j++){
                        if(equiMat[i][j] == 1){
                            equiMat[j][i] = 1;
                        }
                    }
                }
                transitiveClosure(matrix, equiMat);
                makeFile(equiMat);
                plot("SampleOutput.csv");
            }
        }

}

void mainChoice8(int matrix[rows][rows]){
    int score = 1;
    //Check if reflexive
    for(int i = 0; i < rows; i++){
        if(matrix[i][i] == 0){
            score--;
            goto out;
        }
    }

    //Check if transitive
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(matrix[i][j] == 1){
                for(int k = 0; k < rows; k++){
                    if(matrix[j][k] == 1 && matrix[i][k] == 0){
                            score--;
                            goto out;
                    }
                }
            }
        }
    }

    //Check if antisymmetric
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(matrix[i][j] == 1 && matrix[j][i] == 1 && i != j){
                score--;
                goto out;
            }
        }
    }
    out:
        if(score == 0){
            printf("No\n");
            return;
        }
        else{
            printf("Yes\n");
            menu4Selection(matrix);
            return;
        }
    
}

void menu4Choice1(int matrix[rows][rows]){
    int hasMat[rows][rows];
    memcpy(hasMat, matrix, sizeof(int) * rows * rows);
    makeHasMat(hasMat);
    makeFile(hasMat);
    plot("SampleOutput.csv");
}

void menu4Choice2(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        if(matrix[0][i] == 1){
            int flag = 1;
            for(int j = 1; j < rows; j++){
                if(matrix[j][i] == 0){
                    flag = 0;
                    break;
                }
            }
            if(flag == 1){
                printf("%s\n", names[i]);
            }
        }
    }
}

void menu4Choice3(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        if(matrix[i][0] == 1){
            int flag = 1;
            for(int j = 1; j < rows; j++){
                if(matrix[i][j] == 0){
                    flag = 0;
                    break;
                }
            }
            if(flag == 1){
                printf("%s\n", names[i]);
            }
        }
    }
}

void menu4Choice4(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        int flag = 1;
        for(int j = 1; j < rows; j++){
            if(matrix[i][j] == 1 && i != j){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            printf("%s\n", names[i]);
        }
    }
}

void menu4Choice5(int matrix[rows][rows]){
    for(int i = 0; i < rows; i++){
        int flag = 1;
        for(int j = 0; j < rows; j++){
            if(matrix[j][i] == 1 && i != j){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            printf("%s\n", names[i]);
        }
    }
}

void menu4Choice6(int matrix[rows][rows]){
    //Taking site input, validating it and storing the indices
    int num = 0;
    printf("Enter the number of websites: ");
    scanf("%d", &num);
    char sites[num][20];
    int ind[num];
    for(int i = 0; i < num; i++){
        int flag = 0;
        printf("Enter site number %d: ", i + 1);
        scanf("%s", sites[i]);
        for(int j = 0; j < rows; j++){
            if(!strcmp(sites[i], names[j])){
                flag = 1;
                ind[i] = j;
                break;
            }
        }
        if(flag == 0){
            i--;
            printf("Please enter a valid website.\n");
        }
    }

    //Printing the sites reachable from it
    for(int i = 0; i < num; i++){
        printf("%s: ", names[ind[i]]);
        for(int j = 0; j < rows; j++){
                if(matrix[ind[i]][j] == 1){
                    printf("%s, ", names[j]);
                }
        }
        printf("\n");
    }
}

void menu4Choice7(int matrix[rows][rows]){
    //Taking site input, validating it and storing the indices
    int num = 0;
    printf("Enter the number of websites: ");
    scanf("%d", &num);
    char sites[num][20];
    int ind[num];
    for(int i = 0; i < num; i++){
        int flag = 0;
        printf("Enter site number %d: ", i + 1);
        scanf("%s", sites[i]);
        for(int j = 0; j < rows; j++){
            if(!strcmp(sites[i], names[j])){
                flag = 1;
                ind[i] = j;
                break;
            }
        }
        if(flag == 0){
            i--;
            printf("Please enter a valid website.\n");
        }
    }

    //Printing the sites reachable from it
    for(int i = 0; i < num; i++){
        printf("%s: ", names[ind[i]]);
        for(int j = 0; j < rows; j++){
                if(matrix[j][ind[i]] == 1){
                    printf("%s, ", names[j]);
                }
        }
        printf("\n");
    }
}

void menu4Choice8(int matrix[rows][rows]){
    int hasMat[rows][rows];
    memcpy(hasMat, matrix, sizeof(int) * rows * rows);
    makeHasMat(hasMat);
    int lubScore = 0;
    int glbScore = 0;

    for(int i = 0; i < rows; i++){
        int flag = 1;
        for(int j = 0; j < rows; j++){
            if(hasMat[i][j] == 1){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            lubScore++;
        }
    }

    for(int i = 0; i < rows; i++){
        int flag = 1;
        for(int j = 0; j < rows; j++){
            if(hasMat[j][i] == 1){
                flag = 0;
                break;
            }
        }
        if(flag == 1){
            glbScore++;
        }
    }

    if(glbScore == 1 && lubScore == 1){
        printf("Yes\n");
        subMenuSelection(matrix);
        return;
    }
    else{
        printf("No\n");
    }
}

void subMenuChoice1(int matrix[rows][rows]){
    char sites[2][20];
    int ind[2];
    //Inputting and validating websites
    for(int i = 0; i < 2; i++){
        int flag = 0;
        printf("Enter website %c: ", 'A' + i);
        scanf("%s", sites[i]);
        for(int j = 0; j < rows; j++){
            if(!strcmp(sites[i], names[j])){
                flag = 1;
                ind[i] = j;
                break;
            }
        }
        if(flag == 0){
            i--;
            printf("Please enter a valid website.\n");
        }
    }

    int transMat[rows][rows];
    transitiveClosure(matrix, transMat);
    
    for(int i = 0; i < rows; i++){
        //If it is reachable by both A and B
        if(transMat[ind[0]][i] && transMat[ind[1]][i]){
            //Then check if it can reach all such websites that both A and B can reach
            int flag = 1;
            for(int j = 0; j < rows; j++){
                if(transMat[ind[0]][j] && transMat[ind[1]][j] && !transMat[i][j]){
                   flag--;
                   break;
                }
            }
            if(flag == 1){
                 printf("%s\n", names[i]);
                 break;
            }
        }
    }
}

void subMenuChoice2(int matrix[rows][rows]){
    char sites[2][20];
    int ind[2];
    //Inputting and validating websites
    for(int i = 0; i < 2; i++){
        int flag = 0;
        printf("Enter website %c: ", 'A' + i);
        scanf("%s", sites[i]);
        for(int j = 0; j < rows; j++){
            if(!strcmp(sites[i], names[j])){
                flag = 1;
                ind[i] = j;
                break;
            }
        }
        if(flag == 0){
            i--;
            printf("Please enter a valid website.\n");
        }
    }

    int transMat[rows][rows];
    transitiveClosure(matrix, transMat);

    for(int i = 0; i < rows; i++){
        //If it can reach both A and B
        if(transMat[i][ind[0]] && transMat[i][ind[1]]){
            //Then check if it is reachable by all such websites that can reach to both A and B
            int flag = 1;
            for(int j = 0; j < rows; j++){
                if(transMat[j][ind[0]] && transMat[j][ind[1]] && !transMat[j][i]){
                   flag--;
                   break;
                }
            }
            if(flag == 1){
                 printf("%s\n", names[i]);
                 break;
            }
        }
    }
}

void subMenuChoice3(int matrix[rows][rows]){

}

void mainSelection(int choice, int matrix[rows][rows]){
    switch(choice){
            case 1:
                mainChoice1(matrix);
                break;
            case 2:
                mainChoice2(matrix);
                break;
            case 3:
                mainChoice3(matrix);
                break;
            case 4:
                mainChoice4(matrix);
                break;
            case 5:
                mainChoice5(matrix);
                break;
            case 6:
                mainChoice6(matrix);
                break;
            case 7:
                mainChoice7(matrix);
                break;
            case 8:
                mainChoice8(matrix);
                break;
        }
}

void menu4Selection(int matrix[rows][rows]){
    int selection = 0;
    while(selection != 9){
        menu4();
        scanf("%d", &selection);   
        switch(selection){
            case 1:
                menu4Choice1(matrix);
                break;
            case 2:
                menu4Choice2(matrix);
                break;
            case 3:
                menu4Choice3(matrix);
                break;
            case 4:
                menu4Choice4(matrix);
                break;
            case 5:
                menu4Choice5(matrix);
                break;
            case 6:
                menu4Choice6(matrix);
                break;
            case 7:
                menu4Choice7(matrix);
                break;
            case 8:
                menu4Choice8(matrix);
                break;
        }
    }
}

void subMenuSelection(int matrix[rows][rows]){
    int selection = 0;
    while(selection != 4){
        menu5();
        scanf("%d", &selection);   
        switch(selection){
            case 1:
                subMenuChoice1(matrix);
                break;
            case 2:
                subMenuChoice2(matrix);
                break;
            case 3:
                subMenuChoice3(matrix);
                break;
        }
    }
}

int main(){
    //Load file into a matrix
    fname = "SampleInput.csv";
    printf("Enter file name: ");
    scanf(" %s", fname);
    countRows(fname);
    int matrix[rows][rows];
    loadFile(fname, matrix);

    //Opening the main menu
    int choice = 0;
    while(choice != 9){
        mainMenu();
        scanf(" %d", &choice);
        mainSelection(choice, matrix);
    }
}
