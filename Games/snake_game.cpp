#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
using namespace std;
bool headcol(int x,int y,SDL_Rect head){
    if(head.x == x && head.y == y) return true;
    return false;
}
bool bodycol(int x,int y,deque<SDL_Rect>& body){
    for(auto &i:body) if(i.x == x && i.y == y) return true;
    return false;
}
vector<int> randomizer(SDL_Rect& head,deque<SDL_Rect>& body){
    int x,y;
    do x = ((rand()%60)+7)*10,y = ((rand()%60)+5)*10;
    while(headcol(x,y,head) || bodycol(x,y,body));
    return {x,y};
}
SDL_Texture* renderText(string a,TTF_Font* font,SDL_Renderer* render,SDL_Color c){
    auto surface = TTF_RenderText_Blended_Wrapped(font,a.c_str(),c,150);
    auto texture = SDL_CreateTextureFromSurface(render,surface);
    SDL_FreeSurface(surface);
    return texture;
}
int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout<<"Error in SDL linking"<<endl;
    if(TTF_Init() < 0) cout<<"Error in TTF linking"<<endl;
    auto window = SDL_CreateWindow("Snake Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,900,700,SDL_WINDOW_SHOWN);
    auto render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    auto font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf",20);
    SDL_Color textclr = {0,250,250,255};
    auto maintext = renderText(" Start\n Quit\n Box\n -\n +",font,render,textclr);
    int lheight = TTF_FontLineSkip(font);
    srand(time(0));

    SDL_Event e;
    int speed = 0;
    vector<int> dlay = {500,300,100,50};
    vector<int> color = {255,0};
    bool quit = false,grid = false,clr = false;
    SDL_Rect minusbtn {760,400,20,lheight};
    SDL_Rect plusbtn {810,400,20,lheight};
    SDL_Rect quitbtn {750,500,50,lheight};
    SDL_Rect ground {70,50,610,610};
    SDL_Rect startbtn {350,340,50,lheight};
    SDL_Rect gridbtn {700,300,50,lheight};
    SDL_Rect clrbtn {860,10,20,20};
    enum Dir {up,down,left,right};
    
    start:
    int score = 0,ltime = SDL_GetTicks();
    deque<SDL_Rect> body;
    SDL_Rect head {370,350,10,10};
    auto in_pos = randomizer(head,body);
    SDL_Rect fruit {in_pos[0],in_pos[1],10,10};
    bool playing = false;
    Dir cur = up,nxt = up;
    auto speedtext = renderText("Speed:     "+to_string(speed+1),font,render,textclr);
    auto scoretext = renderText("Score:"+to_string(score),font,render,textclr);

    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_UP && cur != down) nxt = up;
                else if(e.key.keysym.sym == SDLK_DOWN && cur != up) nxt = down;
                else if(e.key.keysym.sym == SDLK_LEFT && cur != right) nxt = left;
                else if(e.key.keysym.sym == SDLK_RIGHT && cur != left) nxt = right;
                else if(e.key.keysym.sym == SDLK_ESCAPE) playing = false;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                int x,y;
                SDL_GetMouseState(&x,&y);
                SDL_Point click = {x,y};
                if(SDL_PointInRect(&click,&plusbtn)) speed = min(3,speed+1); 
                else if(SDL_PointInRect(&click,&minusbtn)) speed = max(0,speed-1);
                else if(SDL_PointInRect(&click,&startbtn)) playing = true;
                else if(SDL_PointInRect(&click,&quitbtn)) quit = true;
                else if(SDL_PointInRect(&click,&gridbtn)) grid = !grid;
                else if(SDL_PointInRect(&click,&clrbtn)) clr = !clr;
                speedtext = renderText("Speed:     "+to_string(speed+1),font,render,textclr);
            }
        }
        SDL_SetRenderDrawColor(render,color[clr],color[clr],color[clr],255);
        SDL_RenderClear(render);
        if(grid){
            SDL_Rect box {65,45,620,620};
            SDL_SetRenderDrawColor(render,color[!clr],color[!clr],color[!clr],255);
            SDL_RenderFillRect(render,&box);
            SDL_SetRenderDrawColor(render,color[clr],color[clr],color[clr],255);
            SDL_RenderFillRect(render,&ground);
        }
        SDL_SetRenderDrawColor(render,color[!clr],color[!clr],color[!clr],255);
        SDL_RenderDrawRect(render,&ground);

        SDL_Rect srect {0,lheight*4,20,lheight};
        SDL_RenderCopy(render,maintext,&srect,&plusbtn);
        SDL_RenderDrawRect(render,&plusbtn);

        srect = {0,lheight*3,20,lheight};
        SDL_RenderCopy(render,maintext,&srect,&minusbtn);
        SDL_RenderDrawRect(render,&minusbtn);

        srect = {0,lheight,50,lheight};
        SDL_RenderCopy(render,maintext,&srect,&quitbtn);
        SDL_RenderDrawRect(render,&quitbtn);

        srect = {0,lheight*2,50,lheight};
        SDL_RenderCopy(render,maintext,&srect,&gridbtn);
        SDL_RenderDrawRect(render,&gridbtn);

        SDL_RenderDrawRect(render,&clrbtn);
        srect = {700,400,100,lheight};
        SDL_RenderCopy(render,speedtext,NULL,&srect);
        srect = {700,40,90,lheight};
        SDL_RenderCopy(render,scoretext,NULL,&srect);
        int ctime = SDL_GetTicks();

        if(!playing){
            srect = {0,0,55,lheight};
            SDL_RenderCopy(render,maintext,&srect,&startbtn);
            SDL_RenderDrawRect(render,&startbtn);
        }
        else{    
            SDL_SetRenderDrawColor(render,255,0,0,255);
            SDL_RenderFillRect(render,&fruit);
            SDL_SetRenderDrawColor(render,color[!clr],color[!clr],color[!clr],255);
            
            if(ctime - ltime > dlay[speed]){
                ltime = ctime;
                SDL_Rect temp {head.x,head.y,10,10};
                body.push_front(temp);
                cur = nxt;
                switch(cur){
                    case up: head.y -= 10;
                    break;
                    case down: head.y += 10;
                    break;
                    case left: head.x -= 10;
                    break;
                    case right: head.x += 10;
                    break;
                }
                
                if(!SDL_HasIntersection(&ground,&head)){
                    if(grid) goto start;
                    else{
                        if(head.x < 70) head.x = 670;
                        if(head.x > 670) head.x = 70;
                        if(head.y < 50) head.y = 650;
                        if(head.y > 650) head.y = 50;
                    }
                }
                if(!headcol(fruit.x,fruit.y,head)) body.pop_back();
                else{
                    score++;
                    scoretext = renderText("Score:"+to_string(score),font,render,textclr);
                    in_pos = randomizer(head,body);
                    fruit.x = in_pos[0],fruit.y = in_pos[1];
                }
                if(bodycol(head.x,head.y,body)) goto start;
            }
            SDL_RenderFillRect(render,&head);
            for(auto &i:body) SDL_RenderFillRect(render,&i);
        }
        SDL_RenderPresent(render);
        SDL_Delay(1);
    }
    return 0;
}