#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

using namespace std;

typedef vector<int> vi;
vector<string> final_result;

vi currentState( 40 ), goalState( 40 );

int checkindex(int i){
    int j;
    for (j =0; j<40; j++){
        if (currentState[j]==i){break;}
    }
    return j;
}

int affectCube[][8] = {
    {  0,  1,  2,  3,  0,  1,  2,  3 },
    {  4,  7,  6,  5,  4,  5,  6,  7 },
    {  0,  9,  4,  8,  0,  3,  5,  4 },
    {  2, 10,  6, 11,  2,  1,  7,  6 },
    {  3, 11,  7,  9,  3,  2,  6,  5 },
    {  1,  8,  5, 10,  1,  0,  4,  7 },
};

vi appMove ( int move, vi state ) {
    int turns = move % 3 + 1;
    int face = move / 3;
    while( turns-- ){
        vi oldState = state;
        for( int i=0; i<8; i++ ){
            int isCorner = i > 3;
            int target = affectCube[face][i] + isCorner*12;
            int kill = affectCube[face][(i&3)==3 ? i-3 : i+1] + isCorner*12;;
            int orientationDelta = (i<4) ? (face>1 && face<4) : (face<2) ? 0 : 2 - (i&1);
            state[target] = oldState[kill];
            //state[target+20] = (oldState[kill+20] + orientationDelta) % (2 + isCorner);
            state[target+20] = oldState[kill+20] + orientationDelta;
            if( !turns )
                state[target+20] %= 2 + isCorner;
        }
    }
    return state;
}

void do_move(string face, string mvs){
    string s;
    for(char *p = &mvs[0]; *p !='\0'; p=p+2){
        s.push_back(*p);

        if(face=="left"){
            if(s=="L"){s="B";}
            else if(s=="R"){s="F";}
            else if(s=="F"){s="L";}
            else if(s=="B"){s="R";}
        }
        else if (face=="right"){
            if(s=="L"){s="F";}
            else if(s=="R"){s="B";}
            else if(s=="F"){s="R";}
            else if(s=="B"){s="L";}
        }
        else if (face=="back"){
            if(s=="L"){s="R";}
            else if(s=="R"){s="L";}
            else if(s=="F"){s="B";}
            else if(s=="B"){s="F";}
        }
        else if (face=="down"){
            if(s=="U"){s="D";}
            else if(s=="D"){s="U";}
            else if(s=="F"){s="B";}
            else if(s=="B"){s="F";}
        }

        else if (face=="downright"){
            if(s=="U"){s="D";}
            else if(s=="D"){s="U";}
            else if(s=="F"){s="R";}
            else if(s=="B"){s="L";}
            else if(s=="L"){s="B";}
            else if(s=="R"){s="F";}
        }

        else if (face=="downleft"){
            if(s=="U"){s="D";}
            else if(s=="D"){s="U";}
            else if(s=="F"){s="L";}
            else if(s=="B"){s="R";}
            else if(s=="L"){s="F";}
            else if(s=="R"){s="B";}
        }

        else if (face=="downback"){
            if(s=="U"){s="D";}
            else if(s=="D"){s="U";}
            else if(s=="L"){s="R";}
            else if(s=="R"){s="L";}
        }
        s.push_back(*(p+1));


        if     (s=="U1"){currentState = appMove(0, currentState); final_result.push_back("U");}
        else if(s=="U2"){currentState = appMove(1, currentState); final_result.push_back("U2");}
        else if(s=="U3"){currentState = appMove(2, currentState); final_result.push_back("U'");}
        else if(s=="D1"){currentState = appMove(3, currentState); final_result.push_back("D");}
        else if(s=="D2"){currentState = appMove(4, currentState); final_result.push_back("D2");}
        else if(s=="D3"){currentState = appMove(5, currentState); final_result.push_back("D'");}
        else if(s=="F1"){currentState = appMove(6, currentState); final_result.push_back("F");}
        else if(s=="F2"){currentState = appMove(7, currentState); final_result.push_back("F2");}
        else if(s=="F3"){currentState = appMove(8, currentState); final_result.push_back("F'");}
        else if(s=="B1"){currentState = appMove(9, currentState); final_result.push_back("B");}
        else if(s=="B2"){currentState = appMove(10, currentState); final_result.push_back("B2");}
        else if(s=="B3"){currentState = appMove(11, currentState); final_result.push_back("B'");}
        else if(s=="L1"){currentState = appMove(12, currentState); final_result.push_back("L");}
        else if(s=="L2"){currentState = appMove(13, currentState); final_result.push_back("L2");}
        else if(s=="L3"){currentState = appMove(14, currentState); final_result.push_back("L'");}
        else if(s=="R1"){currentState = appMove(15, currentState); final_result.push_back("R");}
        else if(s=="R2"){currentState = appMove(16, currentState); final_result.push_back("R2");}
        else if(s=="R3"){currentState = appMove(17, currentState); final_result.push_back("R'");}

        s.clear();
    }
}

vector<string> Cubik::solveCube() {
    //--- Define the goal.
    string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
                      "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };
    
    // for (auto iter = currentStatus.begin(); iter != currentStatus.end(); ++iter)
    //     std::cout << *iter << " ";
    // std::cout << std::endl;
    // for (int j = 0; j < 20; j++)
    //     std::cout << goal[j] << " ";
    // std::cout << std::endl;

    //UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR
    //0  1  2  3  4  5  6  7  8  9  10 11 12  13  14  15  16  17  18  19

    //--- Prepare current (start) and goal state.

    for( int i=0; i<20; i++ ){
        //--- Goal state.
        goalState[i] = i;

        //--- Current (start) state.
        string cubie = currentStatus[i];
        while( (currentState[i] = find( goal, goal+20, cubie ) - goal) == 20){
            cubie = cubie.substr( 1 ) + cubie[0];
            currentState[i+20]++;
        }
    }

    //if input is solved cube, return empty vector
    bool solved_input = true;
    for (int i=0; i<40; i++){
        if (currentState[i]!=goalState[i]) {solved_input = false;}
    }
    final_result.clear();
    if(solved_input == true){return final_result;}

    //step 1.0: first layer edge piece
    //look for "UF"(0),"UR"(1),"UL"(2),"UB"(3), return there index and directions

    bool first_edge = false;
    for (int i = 0; first_edge ==false ; i++){

        if (currentState[i]==i && currentState[i+20]==0){continue;}

        int index = checkindex(i);
        int dir = currentState[index+20];

        //if in first-layer
        if (index>=0 && index<=3){
            if(i==0 && currentState[i]!=i){
                while(i==0){
                    if (currentState[0]==0){break;}
                    currentState = appMove(0, currentState);
                    final_result.push_back("U");
                }
            }

            else if(currentState[i]==i){
                if(dir==1){
                    if (i==0){do_move("front","F2");}
                    if (i==1){do_move("front","R2");}
                    if (i==2){do_move("front","B2");}
                    if (i==3){do_move("front","L2");}
                }
                else {
                    while(1){
                        if (currentState[i]==i){break;}
                        currentState = appMove(0, currentState);
                        final_result.push_back("U");
                    }
                }
            }

        }

        index = checkindex(i);
        dir = currentState[index+20];

        //if in mid-layer
        if (index==8){do_move("front", "R3");}
        if (index==9){do_move("front", "L1");}
        if (index==10){do_move("front","B3");}
        if (index==11){do_move("front", "L3");}

        index = checkindex(i);
        dir = currentState[index+20];

        //if in down-layer

        if (index>=4 && index<=7){
            while(1){
                if (currentState[i+4]==i){break;}
                currentState = appMove(3, currentState);
                final_result.push_back("D");
            }

            if(dir==0){
                if (i==0){do_move("front","F2");}
                if (i==1){do_move("front","R2");}
                if (i==2){do_move("front","B2");}
                if (i==3){do_move("front","L2");}
            }
            else {
                if (i==0){do_move("front","D1R1F3R3");}
                if (i==1){do_move("front","D1B1R3B3");}
                if (i==2){do_move("front","D1L1B3L3");}
                if (i==3){do_move("front","D1F1L3F3");}
            }
        }
        if (currentState[0]==0 && currentState[1]==1 && currentState[2]==2 && currentState[3]==3 && currentState[20]==0 && currentState[21]==0 && currentState[22]==0 && currentState[23]==0){
            first_edge = true;
        }

        if(i==3){i=-1;}

    }

    //check first cross
    /*if (currentState[0]==0 && currentState[1]==1 && currentState[2]==2 && currentState[3]==3 && currentState[20]==0 && currentState[21]==0 && currentState[22]==0 && currentState[23]==0){
        printf("First layer cross fixed!\n");
    }
    else {printf("First layer cross not fixed.\n");}

    cout<< final_result << "\n";*/

    //if their index are between 4-7(inclusive)(down layer)
    //do D until "UF" move to "DF"(0 move to index 4), "UR" move to "DR"(1 to 5), "UB" move to "DB"(2 to 6), "UL" mot to "DL"(3 to 7)
    //if direction is 0:
    //do F2 or R2 or B2 or L2 depending on which cubie moved
    //if direction is 1:
    //do DRF3R3 or DBR3B3 or DLB3L3 or DFL3F3

    //if their index are between 8-11(inclusive)(mid layer)
    //if index 8 or 9, apply F or F3 to get it to down side , do above
    //if index 10 or 11, apply B or B3 to get it to down side, do above
    //if their index are between 0-3(inclusive)(first layer)
    //if direction is 1,
    //F2 or R2 or B2 or L2 depending on which cubie moved
    //check if index 0 1 2 3 have piece 0 1 2 3, if index 20 21 22 23 have direction 0, if not, continue the above process


    //step 1.1: first layer corner piece
    //
    bool first_corner = false;
    for (int i = 12;  first_corner == false; i++){

        if (i==16){i=12;}

        if (currentState[i]==i && currentState[i+20]==0){continue;}

        int index = checkindex(i);
        int dir = currentState[index+20];


        //in down-layer
        if (index>=16 && index<=19){

            while(1){
                if (i==12 && currentState[16]==12){break;}
                else if (i==13 && currentState[19]==13){break;}
                else if (i==14 && currentState[18]==14){break;}
                else if (i==15 && currentState[17]==15){break;}
                currentState = appMove(3, currentState);
                final_result.push_back("D");
            }

            index = checkindex(i);
            dir = currentState[index+20];

            string face;
            if (index == 16){face = "front";}
            else if(index == 17){face="left";}
            else if(index == 18){face="back";}
            else if(index == 19){face = "right";}

            if(dir==0){do_move(face,"R3D2R1D1R3D3R1");}
            else if(dir==2){do_move(face,"F1D1F3");}
            else if(dir==1){do_move(face,"R3D3R1");}

        }

        index = checkindex(i);
        dir = currentState[index+20];

        //in first-layer correct position
        if (currentState[i]==i && currentState[i+20]!=0){
            string moves;
            if (currentState[i+20]==1){moves = "F1D1F3D3F1D1F3";}
            else if (currentState[i+20]==2){moves = "R3D3R1D1R3D3R1";}
            if (i==12){do_move("front",moves);}
            else if (i==13){do_move("right",moves);}
            else if (i==14){do_move("back",moves);}
            else if(i==15){do_move("left",moves);}
        }
        index = checkindex(i);
        dir = currentState[index+20];


        if (currentState[12]==12 && currentState[13]==13 && currentState[14]==14 && currentState[15]==15 && currentState[32]==0 && currentState[33]==0 && currentState[34]==0 && currentState[35]==0){
            first_corner = true;
        }

        if (first_corner ==false && currentState[12]>=12 && currentState[12]<16 && currentState[13]>=12 && currentState[13]<16 && currentState[14]>=12 && currentState[14]<16 && currentState[15]>=12 && currentState[15]<16){
            if(currentState[12]!=12){
                if(currentState[32]==1){do_move("right", "L1D1L3");}
                else if(currentState[32]==2){do_move("front", "R3D3R1");}
            }
            else if (currentState[13]!=13){
                if(currentState[32]==1){do_move("back", "L1D1L3");}
                else if(currentState[32]==2){do_move("right", "R3D3R1");}

            }
            else if (currentState[14]!=14){
                if(currentState[32]==1){do_move("left", "L1D1L3");}
                else if(currentState[32]==2){do_move("back", "R3D3R1");}
            }
        }



    }

    //check first layer corner
    /* if (currentState[12]==12 && currentState[13]==13 && currentState[14]==14 && currentState[15]==15 && currentState[32]==0 && currentState[33]==0 && currentState[34]==0 && currentState[35]==0){
        printf("First layer corners fixed!\n");
    }
    else {printf("First layer corners not fixed.\n");}

    cout<< final_result << "\n";*/

    //step 2: middle layer edge piece
    for (int i = 8; i<12; i++){
        if (currentState[i]==i && currentState[i+20]==0){continue;}

        int index = checkindex(i);
        int dir = currentState[index+20];
        string face;
        //if in the middle layer, switch with a third layer edge piece
        if (index>=8 && index<=11){
            for(int j =4; j<8; j++){
                int cs = currentState[j];
                if(cs<8 && cs>=4){
                    while(1){
                        if(index==8 && currentState[6]==cs){face = "front";do_move(face, "F1D1F3D3R3D3R1");break;}
                        else if(index==9&& currentState[5]==cs){face = "left";do_move(face, "F1D1F3D3R3D3R1");break;}
                        else if(index==10&& currentState[7]==cs){face = "right";do_move(face, "F1D1F3D3R3D3R1");break;}
                        else if(index==11&& currentState[4]==cs){face = "back";do_move(face, "F1D1F3D3R3D3R1");break;}
                        currentState = appMove(3, currentState);
                        final_result.push_back("D");
                    }
                    break;
                }
            }
        }

        //update information
        index = checkindex(i);
        dir = currentState[index+20];

        //if in the third layer, do something

        if (index>=4 && index<=7){
            while(1){
                if (i==8 && dir==0 && currentState[6]==8){face = "front";do_move(face, "F1D1F3D3R3D3R1");break;}
                else if(i==8 && dir==1 && currentState[7]==8){face = "right";do_move(face,"F3D3F1D1L1D1L3" );break;}
                else if(i==9 && dir==0 && currentState[6]==9){face = "front"; do_move(face, "F3D3F1D1L1D1L3");break;}
                else if(i==9 && dir==1 && currentState[5]==9){face = "left";do_move(face,"F1D1F3D3R3D3R1"  ); break;}
                else if(i==10 && dir==0 && currentState[4]==10){face = "back";do_move(face, "F3D3F1D1L1D1L3"); break;}
                else if(i==10 && dir==1 && currentState[7]==10){face = "right";do_move(face, "F1D1F3D3R3D3R1");break;}
                else if(i==11 && dir==0 && currentState[4]==11){face = "back";do_move(face, "F1D1F3D3R3D3R1");break;}
                else if(i==11 && dir==1 && currentState[5]==11){face = "left";do_move(face, "F3D3F1D1L1D1L3"); break;}
                currentState = appMove(3, currentState);
                final_result.push_back("D");
            }
        }
    }

    //check second layer
    /*if (currentState[8]==8 && currentState[9]==9 && currentState[10]==10 && currentState[11]==11 && currentState[28]==0 && currentState[29]==0 && currentState[30]==0 && currentState[31]==0){
        printf("Second layer fixed!\n");
    }
    else {printf("Second layer not fixed!\n");}

    cout<< final_result << "\n";*/

    //step 3: form cross on last layer
    vi last_cross;
    for (int i = 4; i<8; i++){
        if (currentState[i+20]==0){last_cross.push_back(i);}
    }

    if(last_cross.size()==2){
        //opposite edge piece
        if (last_cross[0]==4 && last_cross[1]==6){
            do_move("downright", "B1L1U1L3U3B3");
        }
        else if(last_cross[0]==5 && last_cross[1]==7){
            do_move("down", "B1L1U1L3U3B3");
        }
        //adjacent edge piece
        else if(last_cross[0]==4 && last_cross[1]==5){
            do_move("downright", "B1U1L1U3L3B3");
        }
        else if(last_cross[0]==5 && last_cross[1]==6){
            do_move("down", "B1U1L1U3L3B3");
        }
        else if(last_cross[0]==4 && last_cross[1]==7){
            do_move("downback", "B1U1L1U3L3B3");
        }
        else if(last_cross[0]==6 && last_cross[1]==7){
            do_move("downleft", "B1U1L1U3L3B3");
        }
    }
    else if(last_cross.size()==0){do_move("down", "B1L1U1L3U3B3F1U1R1U3R3F3");}

    //check third layer cross direction
    /*if (currentState[24]==0 && currentState[25]==0 && currentState[26]==0 && currentState[27]==0){
        printf("Third layer cross direction fixed!\n");
    }
    else {printf("Third layer cross direction not fixed!\n");}

    cout<< final_result << "\n";*/


    //step 4: rotate U face edge pieces
    vector< vi > vec(4, vi(4));
    vi sum(4);
    for (int i =0; i<4; i++){
        for (int j = 0; j<4; j++){
            if (currentState[j+4]==(j+4)){vec[i][j]++; sum[i]++;}

        }
        currentState = appMove(3, currentState);
    }


    int turn = max_element(sum.begin(), sum.end())-sum.begin();
    int matcub = *max_element(sum.begin(), sum.end());

    for (int p = 0; p<turn; p++){
        currentState = appMove(3, currentState);
        final_result.push_back("D");
    }

    if (matcub ==2){
        //opposite
        if((vec[turn][0]==1 && vec[turn][2]==1) || (vec[turn][1]==1 && vec[turn][3]==1)){
            currentState = appMove(3, currentState);
            final_result.push_back("D");
            matcub = 0;
            turn++;
        }
        //adjacent
        else {
            currentState = appMove(3, currentState);
            final_result.push_back("D");
            matcub = 1;
            //prepare for matcub = 1
            if (turn<3){turn++;}
            else if(turn == 3){turn = 0;}
        }

    }

    if (matcub==1){
        string face;
        if(vec[turn][0]==1){
            face = "downleft";
        }
        else if(vec[turn][1]==1){
            face = "downback";
        }
        else if(vec[turn][2]==1){
            face = "downright";
        }
        else if(vec[turn][3]==1){
            face = "down";
        }
        currentState = appMove(3, currentState);
        int match_index=0;
        for (int k = 4; k < 8; k++){
            if (currentState[k]==k){match_index++;}
        }
        currentState = appMove(5, currentState);
        //do R2D3U2R3L1F2R1L3D1R2 for clockwise
        if (match_index==2){
            do_move(face,"R2D3U2R3L1F2R1L3D1R2" );
        }
        //for counter-clockwise
        else if (match_index==0){do_move(face,"R2D3R3L1F2R1L3U2D1R2" );}

    }
    if (matcub ==0){
        while(1){
            if (currentState[6]==5) {break;}
            currentState = appMove(3, currentState);
            final_result.push_back("D");
        }
        do_move("down","R2D2B2D1L2F2L2F2L2F2D3B2D2R2" );
    }

    //check third layer cross direction
    /*  if (currentState[4]==4 && currentState[5]==5 && currentState[6]==6 && currentState[7]==7 && currentState[24]==0 && currentState[25]==0 && currentState[26]==0 && currentState[27]==0){
        printf("Third layer cross fixed!\n");
    }
    else {printf("Third layer cross not fixed!\n");}

    cout<< final_result << "\n";*/


    //step 5: position U face corner pieces
    vi thirdcornerindex;
    string thirdcornerface="NULL";
    for (int i = 16; i<20; i++){
        thirdcornerindex.push_back(checkindex(i));
    }

    if(thirdcornerindex[0]==16 && thirdcornerindex[1]!=17 && thirdcornerindex[2]!=18 && thirdcornerindex[3]!=19){thirdcornerface="downright";}
    else if (thirdcornerindex[0]!=16 && thirdcornerindex[1]==17 && thirdcornerindex[2]!=18 && thirdcornerindex[3]!=19){thirdcornerface="downback";}
    else if (thirdcornerindex[0]!=16 && thirdcornerindex[1]!=17 && thirdcornerindex[2]==18 && thirdcornerindex[3]!=19){thirdcornerface="downleft";}
    else if (thirdcornerindex[0]!=16 && thirdcornerindex[1]!=17 && thirdcornerindex[2]!=18 && thirdcornerindex[3]==19){thirdcornerface="down";}

    if(thirdcornerface!="NULL"){
        currentState = appMove(3, currentState);
        int cornermatch_index=0;
        for (int k = 16; k < 20; k++){
            if (currentState[k]==k){cornermatch_index++;}
        }
        currentState = appMove(5, currentState);

        //rotateclockwise
        if(cornermatch_index==2){do_move(thirdcornerface,"L3U1R1U3L1U1R3U3");}
        //counterclockwise
        else {do_move(thirdcornerface,"U1R1U3L3U1R3U3L1");}
    }

    if (thirdcornerindex[0]!=16 && thirdcornerindex[1]!=17 && thirdcornerindex[2]!=18 && thirdcornerindex[3]!=19){
        if(thirdcornerindex[0]==18 && thirdcornerindex[1]==19 && thirdcornerindex[2]==16 && thirdcornerindex[3]==17){
            {do_move("down","R3B2F1R1F3R3F1R1F3R3F1R1F3R3B2R1");}
        }
        if(thirdcornerindex[0]==17 && thirdcornerindex[1]==16 && thirdcornerindex[2]==19 && thirdcornerindex[3]==18){
            {do_move("down","B1L1U1L3U3L1U1L3U3L1U1L3U3B3");}
        }
        if(thirdcornerindex[0]==19 && thirdcornerindex[1]==18 && thirdcornerindex[2]==17 && thirdcornerindex[3]==16){
            {do_move("downleft","B1L1U1L3U3L1U1L3U3L1U1L3U3B3");}
        }
    }

    //check third layer corner face
    /* if (currentState[16]==16 && currentState[17]==17 && currentState[18]==18 && currentState[19]==19 && currentState[24]==0){
        printf("Third layer corner placed!\n");
    }
    else {printf("Third layer corner not placed!\n");}

    cout<< final_result << "\n";*/

    //step 6: rotate corner pieces
    string stringclockwise = "F1D1F3D3F1D1F3D3";
    string stringcounterclockwise = "D1F1D3F3D1F1D3F3";
    vi thirdcornerdir;
    for (int i = 36; i< 40; i++){
        thirdcornerdir.push_back(currentState[i]);
    }

    if(thirdcornerdir[0]!=0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[1]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[2]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[3]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
    }
    else if (thirdcornerdir[0]==0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[1]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U1");
        if (thirdcornerdir[1]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U1");
        if (thirdcornerdir[3]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U2");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]==0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U2");
        if (thirdcornerdir[2]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[3]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]==0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[1]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U2");
        if (thirdcornerdir[3]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]==0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[1]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[2]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U2");
    }
    else if (thirdcornerdir[0]==0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]==0 ){
        if (thirdcornerdir[1]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U1");
        if (thirdcornerdir[2]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U3");
    }
    else if (thirdcornerdir[0]==0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]==0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[1]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U2");
        if (thirdcornerdir[3]==1){do_move("downback",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downback",stringcounterclockwise);}
        do_move("downback","U2");
    }
    else if (thirdcornerdir[0]==0 && thirdcornerdir[1]==0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[2]==1){do_move("downleft",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downleft",stringcounterclockwise);}
        do_move("downleft","U1");
        if (thirdcornerdir[3]==1){do_move("downleft",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downleft",stringcounterclockwise);}
        do_move("downleft","U3");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]==0 && thirdcornerdir[2]==0 && thirdcornerdir[3]!=0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U3");
        if (thirdcornerdir[3]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[3]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]==0 && thirdcornerdir[2]!=0 && thirdcornerdir[3]==0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U2");
        if (thirdcornerdir[2]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[2]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U2");
    }
    else if (thirdcornerdir[0]!=0 && thirdcornerdir[1]!=0 && thirdcornerdir[2]==0 && thirdcornerdir[3]==0 ){
        if (thirdcornerdir[0]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[0]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U1");
        if (thirdcornerdir[1]==1){do_move("downright",stringclockwise);}
        else if (thirdcornerdir[1]==2){do_move("downright",stringcounterclockwise);}
        do_move("downright","U3");
    }

    //check entire cube
    /* bool flag = true;
    for (int i =0; i<20; i++){
        if(currentState[i]!=i){flag = false;}
    }
    for (int i =20; i<40; i++){
        if(currentState[i]!=0){flag = false;}
    }*/

    return final_result;

}

//test with main function
/*int main(int argc, char**argv){
    vector<string> current;
    vector<string> result;
    for (int i=0; i<20; i++){
      current.push_back(argv[i+1]);
    }
    result = solveCube(current);
    for (unsigned i=0; i<result.size(); i++){
      cout << result[i] << " ";
    }
    cout <<endl;
    return 0;
}*/
