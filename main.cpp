#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

IntRect BLACK(0,0,20,20);
IntRect RED(20,0,20,20);
IntRect ORANGE(40,0,20,20);
IntRect YELLOW(60,0,20,20);
IntRect GREEN(80,0,20,20);
IntRect BLUE(100,0,20,20);
IntRect INDIGO(120,0,20,20);
IntRect PURPLE(140,0,20,20);
IntRect NONE(160,0,20,20);

enum operation {LEFT,RIGHT,RR,LR};

pair <int, int> start;
vector <int> cube_queue;
int line;
int cnt;
int random;
pair <int, int> current[4];
int current_cube[4][4];
int hold;
int height;
deque <int> data[10];
Texture frame;
Sprite s_frame;
Texture cube;
vector <Sprite> s_cube;
Sprite show[8][4];
Text score[2];
Clock cube_clock;
bool holden;

// bool checkLeft(vector <int> &temp){
//     for(int i=0; i<4; i++){
//         if(data[temp[i].first - 1][temp[i].second] != 0 || data[temp[i].first - 1][temp[i].second] != -1)
//             return false;
//     }
//     return true;
// }
//
// bool checkRight(vector <int> &temp){
//     for(int i=0; i<4; i++){
//         if(data[temp[i].first + 1][temp[i].second] != 0 || data[temp[i].first + 1][temp[i].second] != -1)
//             return false;
//     }
//     return true;
// }

void randcube(){
    int a[7] = {1, 2, 3, 4, 5, 6, 7};
    for(int i=0; i<7; i++)
    {
        int num = rand() % 6;
        int temp = a[num];
        a[num] = a[num + 1];
        a[num + 1] = temp;
    }
    for(int i=0 ;i<7; i++)
        cube_queue.push_back(a[i]);
}
void cubetemplate(Sprite matrix[8][4], int color, int num){
    //Matrix for show matrix.
    //Color stands for red to purple which is numbered from 1 to 7.
    //Num means which show box(0 ~ 3).
    if(color == 1){
        matrix[0 + num*2 ][0].setTextureRect(RED);
        matrix[0 + num*2 ][1].setTextureRect(RED);
        matrix[1 + num*2 ][1].setTextureRect(RED);
        matrix[1 + num*2 ][2].setTextureRect(RED);
    }
    else if(color == 2){
        matrix[0 + num*2 ][2].setTextureRect(ORANGE);
        matrix[1 + num*2 ][0].setTextureRect(ORANGE);
        matrix[1 + num*2 ][1].setTextureRect(ORANGE);
        matrix[1 + num*2 ][2].setTextureRect(ORANGE);
    }
    else if(color == 3){
        matrix[0 + num*2 ][1].setTextureRect(YELLOW);
        matrix[0 + num*2 ][2].setTextureRect(YELLOW);
        matrix[1 + num*2 ][1].setTextureRect(YELLOW);
        matrix[1 + num*2 ][2].setTextureRect(YELLOW);
    }
    else if(color == 4){
        matrix[0 + num*2 ][1].setTextureRect(GREEN);
        matrix[0 + num*2 ][2].setTextureRect(GREEN);
        matrix[1 + num*2 ][0].setTextureRect(GREEN);
        matrix[1 + num*2 ][1].setTextureRect(GREEN);
    }
    else if(color == 5){
        matrix[1 + num*2 ][0].setTextureRect(BLUE);
        matrix[1 + num*2 ][1].setTextureRect(BLUE);
        matrix[1 + num*2 ][2].setTextureRect(BLUE);
        matrix[1 + num*2 ][3].setTextureRect(BLUE);
    }
    else if(color == 6){
        matrix[0 + num*2 ][0].setTextureRect(INDIGO);
        matrix[1 + num*2 ][0].setTextureRect(INDIGO);
        matrix[1 + num*2 ][1].setTextureRect(INDIGO);
        matrix[1 + num*2 ][2].setTextureRect(INDIGO);
    }
    else if(color == 7){
        matrix[0 + num*2 ][1].setTextureRect(PURPLE);
        matrix[1 + num*2 ][0].setTextureRect(PURPLE);
        matrix[1 + num*2 ][1].setTextureRect(PURPLE);
        matrix[1 + num*2 ][2].setTextureRect(PURPLE);
    }
}

void showNext(){
    for(int i=2; i<8; i++)
        for(int j=0; j<4; j++){
            //Refresh the box ,otherwise the last cube will remain.
            show[i][j].setTextureRect(NONE);
        }
    // Set color.
    cubetemplate(show, cube_queue[0], 1);
    cubetemplate(show, cube_queue[1], 2);
    cubetemplate(show, cube_queue[2], 3);
}

void init_cube(){
    line = 0;
    height = 0;
    hold = 0;
    //Let sizeof queue >7,otherwise the "next" show box will go wrong.
    randcube();
    randcube();

    for(int i=0; i<10; i++)
        for(int j=0; j<23; j++)
            data[i].push_back(0);
    for(int i=0 ; i<20; i++)
        for(int j=0; j<10; j++){
            Sprite item;
            item.setTexture(cube);
            item.setTextureRect(BLACK);
            item.setPosition(20*j + 150, 20*i + 100);
            s_cube.push_back(item);
        }
    //Set box.
    for(int i=0; i<2; i++)
        for(int j=0; j<4; j++){
            show[i][j].setTexture(cube);
            show[i][j].setTextureRect(NONE);
            show[i][j].setScale(0.75, 0.75);
            show[i][j].setPosition(80 + 15*j,120 + 15*i);
        }
    for(int i=2; i<8; i++)
        for(int j=0; j<4; j++){
            show[i][j].setTexture(cube);
            show[i][j].setTextureRect(NONE);
            show[i][j].setScale(0.75, 0.75);
            if(i < 4)
                show[i][j].setPosition(360 + 15*j, 120 + 15*(i - 2));
            else if(i >= 4 && i <= 5)
                show[i][j].setPosition(360 + 15*j, 120 + 15*(i - 2) + 45);
            else if(i >= 6)
                show[i][j].setPosition(360 + 15*j, 120 + 15*(i - 2) + 80);
        }
}
void clean_line(){
    int total_clean = 0;
    //Line 3~22 is the real data.
    for(int i=22; i>=3; i--){
        bool clean = true;
        for(int j=0; j<10; j++)
        {
            if(data[j][i] == 0){
                clean = false;
                break;
            }
        }
        //Clean line。
        if(clean == true)
        {
            total_clean++;
            line += 1;
            for(int j=0; j<10 ;j++){
                data[j].erase(data[j].begin() + i);
            }
        }
    }
    for(int i=0; i<total_clean; i++)
        for(int j=0 ;j<10; j++)
            data[j].push_front(0);

}
void empty(){
    for(int i=0; i<4; i++)
        data[current[i].first][current[i].second] = 0;
}
int  getrand(){
    if((int)cube_queue.size() <= 7)
        randcube();
    int num = cube_queue.front();
    cube_queue.erase(cube_queue.begin());
    return num;
}

void drop(){

    empty();
    //Replace the next layer。
    for(int i=0; i<4; i++)
        data[current[i].first][current[i].second + 1] = -1;

    //Change border。
    for(int i=0; i<4; i++)
        current[i].second += 1;
    start.second += 1;
}

void setPos(){
    bool one = true, two = true;
    for(int i=0; i<4; i++){
        if(data[current[i].first][current[i].second + 2] == 0 || data[current[i].first][current[i].second + 2] == -1)
            continue;
        else
            two = false;
    }
    for(int i=0; i<4; i++){
        if(data[current[i].first][current[i].second + 1] == 0 || data[current[i].first][current[i].second + 1] == -1)
            continue;
        else
            one = false;
    }
    if(one && two){
        for(int i=0; i<2; i++)
            drop();
    }
    else if (one && !two){
        for(int i=0; i<1; i++)
            drop();
    }
}

void generate_cube(int num){
    random = num;
    cnt = 0;
    start.first = 3;
    start.second = 1;
    memset(current_cube, 0, sizeof(current_cube));
    if(random == 1){

        data[3][0 + 1] = -1;
        data[4][0 + 1] = -1;
        data[4][1 + 1] = -1;
        data[5][1 + 1] = -1;
        s_cube[3].setTextureRect(RED);
        s_cube[4].setTextureRect(RED);
        s_cube[14].setTextureRect(RED);
        s_cube[15].setTextureRect(RED);
        current_cube[0][0] = -1;
        current_cube[0][1] = -1;
        current_cube[1][1] = -1;
        current_cube[1][2] = -1;
        current[0] = make_pair(3, 0 + 1);
        current[1] = make_pair(4, 0 + 1);
        current[2] = make_pair(4, 1 + 1);
        current[3] = make_pair(5, 1 + 1);
    }
    else if(random == 2){

        data[5][0 + 1] = -1;
        data[3][1 + 1] = -1;
        data[4][1 + 1] = -1;
        data[5][1 + 1] = -1;
        s_cube[5].setTextureRect(ORANGE);
        s_cube[13].setTextureRect(ORANGE);
        s_cube[14].setTextureRect(ORANGE);
        s_cube[15].setTextureRect(ORANGE);
        current_cube[0][2] = -1;
        current_cube[1][0] = -1;
        current_cube[1][1] = -1;
        current_cube[1][2] = -1;
        current[0] = make_pair(5, 0 + 1);
        current[1] = make_pair(3, 1 + 1);
        current[2] = make_pair(4, 1 + 1);
        current[3] = make_pair(5, 1 + 1);

    }
    else if(random == 3){

        data[3][0 + 1] = -1;
        data[4][0 + 1] = -1;
        data[3][1 + 1] = -1;
        data[4][1 + 1] = -1;
        s_cube[3].setTextureRect(YELLOW);
        s_cube[4].setTextureRect(YELLOW);
        s_cube[13].setTextureRect(YELLOW);
        s_cube[14].setTextureRect(YELLOW);
        current[0] = make_pair(3, 0 + 1);
        current[1] = make_pair(4, 0 + 1);
        current[2] = make_pair(3, 1 + 1);
        current[3] = make_pair(4, 1 + 1);

    }
    else if(random == 4){

        data[4][0 + 1] = -1;
        data[5][0 + 1] = -1;
        data[3][1 + 1] = -1;
        data[4][1 + 1] = -1;
        s_cube[4].setTextureRect(GREEN);
        s_cube[5].setTextureRect(GREEN);
        s_cube[13].setTextureRect(GREEN);
        s_cube[14].setTextureRect(GREEN);
        current_cube[0][1] = -1;
        current_cube[0][2] = -1;
        current_cube[1][0] = -1;
        current_cube[1][1] = -1;
        current[0] = make_pair(4, 0 + 1);
        current[1] = make_pair(5, 0 + 1);
        current[2] = make_pair(3, 1 + 1);
        current[3] = make_pair(4, 1 + 1);

    }
    else if(random == 5){

        start.first = 3;
        start.second = -1;
        data[3][0 + 1] = -1;
        data[4][0 + 1] = -1;
        data[5][0 + 1] = -1;
        data[6][0 + 1] = -1;
        s_cube[3].setTextureRect(BLUE);
        s_cube[4].setTextureRect(BLUE);
        s_cube[5].setTextureRect(BLUE);
        s_cube[6].setTextureRect(BLUE);
        current_cube[1][0] = -1;
        current_cube[1][1] = -1;
        current_cube[1][2] = -1;
        current_cube[1][3] = -1;
        current[0] = make_pair(3, 0 + 1);
        current[1] = make_pair(4, 0 + 1);
        current[2] = make_pair(5, 0 + 1);
        current[3] = make_pair(6, 0 + 1);
    }
    else if(random == 6){

        data[3][0 + 1] = -1;
        data[3][1 + 1] = -1;
        data[4][1 + 1] = -1;
        data[5][1 + 1] = -1;
        s_cube[3].setTextureRect(INDIGO);
        s_cube[13].setTextureRect(INDIGO);
        s_cube[14].setTextureRect(INDIGO);
        s_cube[15].setTextureRect(INDIGO);
        current_cube[0][0] = -1;
        current_cube[1][0] = -1;
        current_cube[1][1] = -1;
        current_cube[1][2] = -1;
        current[0] = make_pair(3, 0 + 1);
        current[1] = make_pair(3, 1 + 1);
        current[2] = make_pair(4, 1 + 1);
        current[3] = make_pair(5, 1 + 1);

    }
    else if(random == 7){

        data[4][0 + 1] = -1;
        data[3][1 + 1] = -1;
        data[4][1 + 1] = -1;
        data[5][1 + 1] = -1;
        s_cube[4].setTextureRect(PURPLE);
        s_cube[13].setTextureRect(PURPLE);
        s_cube[14].setTextureRect(PURPLE);
        s_cube[15].setTextureRect(PURPLE);
        current_cube[0][1] = -1;
        current_cube[1][0] = -1;
        current_cube[1][1] = -1;
        current_cube[1][2] = -1;
        current[0] = make_pair(4, 0 + 1);
        current[1] = make_pair(3, 1 + 1);
        current[2] = make_pair(4, 1 + 1);
        current[3] = make_pair(5, 1 + 1);
    }
    setPos();
}

void update_cube(){
    //Set color。
    for(int i=3; i<23; i++)
        for(int j=0; j<10; j++)
        {
            if(data[j][i] == 0)
                s_cube[(i - 3)*10 + j].setTextureRect(BLACK);
            else if(data[j][i] == 1)
                s_cube[(i - 3)*10 + j].setTextureRect(RED);
            else if(data[j][i] == 2)
                s_cube[(i - 3)*10 + j].setTextureRect(ORANGE);
            else if(data[j][i] == 3)
                s_cube[(i - 3)*10 + j].setTextureRect(YELLOW);
            else if(data[j][i] == 4)
                s_cube[(i - 3)*10 + j].setTextureRect(GREEN);
            else if(data[j][i] == 5)
                s_cube[(i - 3)*10 + j].setTextureRect(BLUE);
            else if(data[j][i] == 6)
                s_cube[(i - 3)*10 + j].setTextureRect(INDIGO);
            else if(data[j][i] == 7)
                s_cube[(i - 3)*10 + j].setTextureRect(PURPLE);
            else if(data[j][i] == -1)
            {
                if(random == 1)
                    s_cube[(i - 3)*10 + j].setTextureRect(RED);
                else if(random == 2)
                    s_cube[(i - 3)*10 + j].setTextureRect(ORANGE);
                else if(random == 3)
                    s_cube[(i - 3)*10 + j].setTextureRect(YELLOW);
                else if(random == 4)
                    s_cube[(i - 3)*10 + j].setTextureRect(GREEN);
                else if(random == 5)
                    s_cube[(i - 3)*10 + j].setTextureRect(BLUE);
                else if(random == 6)
                    s_cube[(i - 3)*10 + j].setTextureRect(INDIGO);
                else if(random == 7)
                    s_cube[(i - 3)*10 + j].setTextureRect(PURPLE);
            }
        }
}

void adjust(int size){
    //Reset border。
    if(size == 3)
    {
        if(start.first < 0)
            start.first = 0;
        else if(start.first + 2 > 9)
            start.first = 7;
        if(start.second < 3)
            start.second = 3;
        else if(start.second + 2 > 22)
            start.second = 20;
    }
    else
    {
        if(start.first < 0)
            start.first = 0;
        else if(start.first + 3 > 9)
            start.first = 6;
        if(start.second < 3)
            start.second = 3;
        else if(start.second + 3 > 22)
            start.second = 19;
    }

}
void rotation90(int matrix[4][4],int size){
    if(size == 3)
    {
        int temp[3][3];
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                temp[i][j] = matrix[i][j];

        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                matrix[i][j] = temp[3-j-1][i];
    }
    else
    {
        int temp[4][4];
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                temp[i][j] = matrix[i][j];

        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                matrix[i][j] = temp[4-j-1][i];
    }
}

void operate_cube(enum operation oper){
    if(oper == LEFT)
    {
        for(int i=0; i<4; i++){
            //Border check.
            if(current[i].first - 1 < 0 )
                return ;
            //Cube check.
            if(data[current[i].first - 1][current[i].second] == 0 || data[current[i].first - 1][current[i].second] == -1)
                continue;
            else
                return ;
        }

        empty();
        //Replace left cube。
        for(int i=0; i<4; i++)
            data[current[i].first - 1][current[i].second] = -1;

        for(int i=0; i<4; i++)
            current[i].first -= 1;
       start.first -= 1;

    }
    else if(oper == RIGHT)
    {
        for(int i=0; i<4; i++){
            if(current[i].first + 1 > 9)
                return ;
            if(data[current[i].first + 1][current[i].second] == 0 || data[current[i].first + 1][current[i].second] == -1)
                continue;
            else
                return ;
        }

        empty();
        //Replace right cube。
        for(int i=0; i<4; i++)
            data[current[i].first + 1][current[i].second] = -1;

        for(int i=0; i<4; i++)
            current[i].first += 1;
        start.first += 1;
    }
    else if(oper == RR)
    {
        vector <pair<int , int> > temp;
        if(random != 5)
        {
            bool moveLeft = true, moveRight = true, avaliable = true;
            bool isLeft = false, isRight = false;
            if(start.first < 0 || start.first + 2 > 9 || start.second < 3 || start.second + 2 > 22)
                adjust(3);
            rotation90(current_cube,3);
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if(current_cube[i][j] != 0){
                        temp.push_back(make_pair(start.first + j,start.second + i));
                    }
                }
            }
            cout<< "start: " << start.first << " " << start.second << endl;
            //Check whether it can move left or right or not.
            for(int i=0; i<4; i++){
                if(temp[i].first - 1 < 0 ){
                    moveLeft = false;
                    break;
                }
                if(data[temp[i].first - 1][temp[i].second] > 0)
                    moveLeft = false;
            }
            for(int i=0; i<4; i++){
                if(temp[i].first + 1 > 9 ){
                    moveRight = false;
                    break;
                }
                if(data[temp[i].first + 1][temp[i].second] > 0)
                    moveRight = false;
            }
            //Check avaliable.
            for(int i=0; i<4; i++){
                cout<< "temp" << temp[i].first<< " " << temp[i].second <<endl;
                if(data[temp[i].first][temp[i].second] > 0)
                    avaliable = false;
            }
            //If the rotation is avaliable.
            if(avaliable == true){
                  //Empty cube。
                  puts("haha");
                  empty();
                  data[temp[0].first][temp[0].second] = -1;
                  data[temp[1].first][temp[1].second] = -1;
                  data[temp[2].first][temp[2].second] = -1;
                  data[temp[3].first][temp[3].second] = -1;
             }
             //The rotation is unavaliable ,
             //so we gonna search for a suitable position.
            else{
                puts("nonono");
                printf("moveLeft : %d moveRight: %d\n",moveLeft,moveRight);
                bool move;
                if(moveLeft == true){
                    for(int i=0; i<3; i++){
                        move = true;
                        isLeft = true;
                        for(int j=0; j<4; j++){
                            if(i == 0){
                                if(data[temp[j].first][temp[j].second + 1] > 0){
                                    cout << "data" << data[temp[j].first + 1][temp[j].second + 1] <<endl;
                                    move = false;
                                    break;
                                }
                                else if(data[temp[j].first - 1][temp[j].second + 1] > 0){
                                    isLeft = false;
                                    break;
                                }
                            }
                            else{
                                if(data[temp[j].first][temp[j].second - (i + 1)] > 0){
                                    cout << "data" << data[temp[j].first + 1][temp[j].second - (i + 1)] <<endl;
                                    move = false;
                                    break;
                                }
                                else if(data[temp[j].first - 1][temp[j].second - (i + 1)] > 0){
                                    isLeft = false;
                                    break;
                                }
                            }
                        }
                        if(move == true){
                            puts("success");
                            empty();
                            for(int j=0; j<4; j++){
                                if(i == 0)
                                    temp[j].second += 1;
                                else
                                    temp[j].second -= i+1;
                                data[temp[j].first][temp[j].second] = -1;
                            }
                            break;
                        }
                        else if(isLeft == true){
                            empty();
                            start.first -= 1;
                            for(int j=0; j<4; j++){
                                temp[i].first -= 1;
                                if(i == 0)
                                    temp[j].second += 1;
                                else
                                    temp[j].second -= i+1;
                                data[temp[j].first][temp[j].second] = -1;
                            }
                            break;
                        }
                    }
                    if(move == false && isLeft == false){
                        return ;
                    }
                }
                else if(moveRight == true){
                    for(int i=0; i<3; i++){
                        move = true;
                        isRight = true;
                        for(int j=0; j<4; j++){
                            if(i == 0){
                                // 不右移不行
                                if(data[temp[j].first][temp[j].second + 1] > 0){
                                    move = false;
                                    break;
                                }
                                // 右移不行
                                else if(data[temp[j].first + 1][temp[j].second + 1] > 0){
                                    isRight = false;
                                    break;
                                }
                            }
                            else{
                                if(data[temp[j].first][temp[j].second - (i + 1)] > 0){
                                    cout << "data" << data[temp[j].first + 1][temp[j].second - (i + 1)] <<endl;
                                    move = false;
                                    break;
                                }
                                else if(data[temp[j].first + 1][temp[j].second - (i + 1)] > 0){
                                    isRight = false;
                                    break;
                                }
                            }
                        }
                        if(move == true){
                            puts("success");
                            empty();
                            for(int j=0; j<4; j++){
                                if(i == 0)
                                    temp[j].second += 1;
                                else
                                    temp[j].second -= i+1;
                                data[temp[j].first][temp[j].second] = -1;
                            }
                            break;
                        }
                        else if(isRight == true){
                            empty();
                            start.first += 1;
                            for(int j=0; j<4; j++){
                                temp[j].first += 1;
                                if(i == 0)
                                    temp[j].second += 1;
                                else
                                    temp[j].second -= i+1;
                                data[temp[j].first][temp[j].second] = -1;
                            }
                            break;
                        }
                    }
                    if(move == false && isRight == false){
                        return ;
                    }

                }
                else
                    return ;
            }
        }
        //For 'I' cube。
        else
        {
            //Empty cube。
            empty();

            if(start.first < 0 || start.first + 3 > 9 || start.second < 3 || start.second + 3 > 22)
                adjust(4);
            rotation90(current_cube,4);
            //Replace cube。
            for(int i=0; i<4; i++)
                for(int j=0; j<4; j++){
                    if(current_cube[i][j] != 0){
                        data[start.first + j][start.second + i] = current_cube[i][j];
                        temp.push_back(make_pair(start.first + j,start.second + i));
                }
            }
        }
        for(int i=0; i<4; i++)
            current[i] = make_pair(temp[i].first, temp[i].second);
    }
}
void movebase(){
    int mn = 0x3f3f3f3f;
    for(int i=0 ;i<4; i++)
    {
        int temp = current[i].second;
        //Note that 22 is the base.
        while(temp + 1 <= 22 && (data[current[i].first][temp + 1] == 0 || data[current[i].first][temp + 1] == -1)){
            temp += 1;
        }
        //Get the min plus.
        mn = min(mn, temp - current[i].second);
    }
    empty();
    //Reset position.
    for(int i=0; i<4 ;i++){
        current[i] = make_pair(current[i].first, current[i].second + mn);
        data[current[i].first][current[i].second] = -1;
    }
}
void makehold(){
    //Empty cube。
    empty();

    if(hold == 0){
        hold = random;
        generate_cube(getrand());
    }
    else{
        int temp = hold;
        hold = random;
        generate_cube(temp);
    }

    for(int i=0; i<2; i++)
        for(int j=0; j<4; j++)
        {
            //Refresh the box,otherwise the last hold will remain.
            show[i][j].setTextureRect(NONE);
        }
        cubetemplate(show,hold,0);
}
int check(bool &over){
    //Check whether the cube over top.
    for(int i=0; i<10; i++)
        for(int j=0; j<=2; j++){
            if(data[i][j] != 0){
                over = true;
                return 2;
            }
        }
    for(int i=0; i<4; i++)
    {
        if(current[i].second + 1 > 22){
            return 1;
        }
        if(data[current[i].first][current[i].second + 1] == 0 || data[current[i].first][current[i].second + 1] == -1 )
            continue;
        else{
            return 1;
        }
    }
    return 0;
}

void end_line(){
    for(int i=0; i<4; i++)
        data[current[i].first][current[i].second] = random;
    holden = false;
    clean_line();
    generate_cube(getrand());
    for(int i=0; i<4; i++)
        printf("cube:%d %d \n",current[i].first,current[i].second);
    for(int i=3 ; i<=22; i++){
        for(int j=0; j<10; j++){
            // if(data[j][i] == -1)
            //     printf("%d",random);
            // else
            printf("%d",data[j][i]);
        }
        printf("\n");
    }
    puts("===========");
}
int main()
{
    RenderWindow window(VideoMode(500, 500), "Tetris");
    srand(time(NULL));
    if(!cube.loadFromFile("cube.png"))
        return EXIT_FAILURE;
    if(!frame.loadFromFile("frame.png"))
        return EXIT_FAILURE;

    s_frame.setTexture(frame);
    init_cube();
    generate_cube(getrand());

    bool pause = false;
    bool over = false;
    while (window.isOpen())
    {
        int condition = check(over);
        if(!pause && !over){
            if(cube_clock.getElapsedTime().asSeconds() > 0.9f)
            {
                if(condition == 0){
                    drop();

                    cout<< "sstart: " << start.first << " " << start.second << endl;
                    for(int i=0; i<4; i++)
                        printf("cube:%d %d \n",current[i].first,current[i].second);
                    for(int i=3 ; i<=22; i++){
                        for(int j=0; j<10; j++){
                            // if(data[j][i] == -1)
                            //     printf("%d",random);
                            // else
                            printf("%d",data[j][i]);
                        }
                        printf("\n");
                    }
                    puts("===========");
                    cube_clock.restart();
                }
                else if(condition == 1){
                    end_line();
                }
            }
        }
        //Get Event。
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
                if(event.type == Event::KeyPressed)
                {
                    if(Keyboard::isKeyPressed(Keyboard::Left)){
                        operate_cube(LEFT);
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::Right)){
                        operate_cube(RIGHT);
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::Up)){
                        if(random != 3)
                            operate_cube(RR);
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::Down)){
                        if(condition == 0)
                        drop();
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::Space)){
                        movebase();
                        end_line();
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::C)){
                        printf("%d\n",holden);
                        if(!holden){
                            makehold();
                            holden = true;
                        }
                    }
                    if(Keyboard::isKeyPressed(Keyboard::P))
                    pause = !pause;
                }
        }
        update_cube();
        Font font;
        if (!font.loadFromFile("brightfuture.ttf"))
            return EXIT_FAILURE;

        score[0] = Text("Line:",font, 40);
        score[0].setPosition(20,350);
        score[0].setColor(Color(255,88,0,255));
        stringstream ss;
        ss << line;
        string convert_str;
        ss >> convert_str;
        score[1] = Text(convert_str,font, 40);
        score[1].setPosition(40,385);
        score[1].setColor(Color(255,88,0,255));
        showNext();
        window.clear();
        window.draw(s_frame);
        for(int i=0; i<8; i++)
            for(int j=0; j<4; j++)
                window.draw(show[i][j]);
        window.draw(score[0]);
        window.draw(score[1]);
        for(int i=0; i<200; i++)
            window.draw(s_cube[i]);
        window.display();
    }

    return 0;
}
