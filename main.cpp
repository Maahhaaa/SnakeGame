#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
using namespace sf;


int N=30,M=20;
int size=16;
int w = size*N;
int h = size*M;
bool isAlive=true;
bool isFed;
//num is snake length
int dir,num=4;

struct Snake
{ int x,y;}  s[100];

struct Fruit
{ int x,y;} f;




void Tick()
 {
     isFed = false;
    for (int i=num;i>0;--i)
     {s[i].x=s[i-1].x; s[i].y=s[i-1].y;}
    //setting directions
    if (dir==0) s[0].y+=1;
    if (dir==1) s[0].x-=1;
    if (dir==2) s[0].x+=1;
    if (dir==3) s[0].y-=1;

    //ate fruit
    if ((s[0].x==f.x) && (s[0].y==f.y))
     {num++; f.x=rand()%N; f.y=rand()%M; isFed=true;}

    if (s[0].x>N) s[0].x=0;  if (s[0].x<0) s[0].x=N;
    if (s[0].y>M) s[0].y=0;  if (s[0].y<0) s[0].y=M;

    //when eat itself
    for (int i=1;i<num;i++)
    if (s[0].x==s[i].x && s[0].y==s[i].y)  {
        isAlive=false;
    }
//     if (s[0].x==s[i].x && s[0].y==s[i].y)  num=i;
 }

int main()
{

    SoundBuffer buf1;
    Sound eat;
    if(!buf1.loadFromFile("sounds/eat.wav")) return -1;
    eat.setBuffer(buf1);

    Music back, over;
    back.openFromFile("sounds/musicback.wav");
    over.openFromFile("sounds/over.wav");


    Text scrText, overText, repText;
    sf::Font font1,font2;
    if(!font1.loadFromFile("fonts/arial.ttf")) return -1;
    if(!font2.loadFromFile("fonts/PublicPixel.ttf")) return -1;
//scrText.setCharacterSize(24);
overText.setCharacterSize(48);
overText.setString("Game Over");
overText.setFillColor(Color::Red);
overText.setFont(font2);
overText.setOrigin(overText.getGlobalBounds().width/2,overText.getGlobalBounds().height);
overText.setPosition(w/2,h/2);
overText.setOutlineColor(Color::Black);
overText.setOutlineThickness(2);


scrText.setCharacterSize(24);
scrText.setString("Score: "+std::to_string(num));
scrText.setFillColor(Color::White);
scrText.setFont(font2);
scrText.setOrigin(scrText.getGlobalBounds().width/2,scrText.getGlobalBounds().height);
scrText.setPosition(w/2,h/2+overText.getGlobalBounds().height+size);
scrText.setOutlineColor(Color::Black);
scrText.setOutlineThickness(1);

repText.setCharacterSize(20);
repText.setString("Press R to replay!");
repText.setFillColor(Color::White);
repText.setFont(font2);
repText.setOrigin(repText.getGlobalBounds().width/2,repText.getGlobalBounds().height);
repText.setPosition(w/2,h/2+overText.getGlobalBounds().height+scrText.getGlobalBounds().height+2*size);
repText.setOutlineColor(Color::Black);
repText.setOutlineThickness(1);


    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake Game!");
    window.setFramerateLimit(60);
    Texture t1,t2,t3;
//    t1.loadFromFile("images/white.png");
    t1.loadFromFile("images/background2.png");
//    t2.loadFromFile("images/red.png");
    t2.loadFromFile("images/snake.png");
//    t3.loadFromFile("images/red1.png");
    t3.loadFromFile("images/apple.png");

    Sprite sprite1(t1);//background
    Sprite sprite2(t2);//snake
    Sprite sprite3(t3);//fruit

    Clock clock;
    float timer=0, delay=0.15;

    f.x=10;
    f.y=10;
    back.setLoop(true);
    back.play();
    while (window.isOpen())
    {
        delay=0.2-0.01*std::log(num);
//        std::cout<<"delay is: "<<delay<<"   "<<num<<std::endl;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)&&dir!=2) dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Right)&&dir!=1) dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Up)&&dir!=0) dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Down)&&dir!=3) dir=0;
        if (Keyboard::isKeyPressed(Keyboard::R)&&!isAlive) {
            isAlive=true;
            num=4;
            for (int i=num;i>0;--i)
     {s[i].x=s[i-1].x; s[i].y=s[i-1].y;}
            window.close();
            over.stop();
            main();
        }

        if (timer>delay) {timer=0; Tick();}
scrText.setString("Score: "+std::to_string(num-4));
   ////// draw  ///////
    window.clear();
    //background drawing
    for (int i=0; i<N; i++)
      for (int j=0; j<M; j++)
        { sprite1.setPosition(i*size, j*size);  window.draw(sprite1); }

if(isAlive){
    for (int i=0;i<num;i++)
        { sprite2.setPosition(s[i].x*size, s[i].y*size);  window.draw(sprite2); }

    sprite3.setPosition(f.x*size, f.y*size);  window.draw(sprite3);
    if(isFed) eat.play();
}else{

    window.draw(overText);
    window.draw(scrText);
    window.draw(repText);
//    back.setLoop(false);
    back.stop();
    if(!over.getStatus())
    over.play();

}
    window.display();
    }

    return 0;
}
