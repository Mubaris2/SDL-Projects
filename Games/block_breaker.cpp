#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL2_gfxPrimitives.h>
using namespace std;
void changeVel(SDL_Rect hit,SDL_Rect ball,int &vx,int &vy){
    if(ball.x-hit.x == hit.w+vx || hit.x-ball.x == ball.h-vx) vx = -vx;
    if(ball.y-hit.y == hit.h-2 || hit.y-ball.y == ball.w-2) vy = -vy;
}
int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout<<"Error in SDL linking"<<endl;
    auto window = SDL_CreateWindow("Block Breaker",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN);
    auto render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event e;
    SDL_Rect ground {20,20,760,560};
    SDL_Rect pad {390,560,50,10};
    SDL_Rect ball {385,500,10,10};
    vector<SDL_Rect> blocks;
    for(int i=0;i<10;i++) for(int j=0;j<5;j++){
        SDL_Rect temp {60+i*70,70+j*40,50,10};
        blocks.push_back(temp);
    }
    int vy = -2,vx = 0;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_MOUSEMOTION){
                int x,y;
                SDL_GetMouseState(&x,&y);
                pad.x = x-25;
            }
        }
        SDL_SetRenderDrawColor(render,255,255,255,255);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render,0,0,0,255);
        SDL_RenderDrawRect(render,&ground);

        SDL_RenderFillRect(render,&pad);
        SDL_SetRenderDrawColor(render,0,0,255,255);
        SDL_RenderFillRect(render,&ball);
        SDL_SetRenderDrawColor(render,0,255,0,255);
        for(auto &i:blocks) SDL_RenderFillRect(render,&i);

        ball.x += vx,ball.y += vy;
        if(SDL_HasIntersection(&ball,&pad)){
            changeVel(pad,ball,vx,vy);
            vx = ((ball.x+5-pad.x)/10)-2;
        }
        for(int i=0;i<blocks.size();i++) if(SDL_HasIntersection(&ball,&blocks[i])){
            changeVel(blocks[i],ball,vx,vy);
            blocks.erase(blocks.begin()+i);
        }
        if(ball.x < 20 || ball.x > 770) vx = -vx;
        if(ball.y < 20) vy = -vy;
        if(ball.y > 570 || blocks.empty()) quit = true;
        
        SDL_RenderPresent(render);
        SDL_Delay(10);
    }
    return 0;
}